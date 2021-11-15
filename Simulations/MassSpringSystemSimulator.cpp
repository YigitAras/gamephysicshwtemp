#include "MassSpringSystemSimulator.h"
#define POINT_SIZE 0.01




void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext)
{

	DUC->beginLine();
	for (int i = 0; i < m_Springs.size(); i++) {

		DUC->drawLine(m_Points[m_Springs[i].P1].position,
			Vec3(100, 100, 50),
			m_Points[m_Springs[i].P2].position,
			Vec3(100, 100, 50)
		);

	}
	DUC->endLine();
	for (int i = 0; i < m_Points.size(); i++)
	{
		DUC->drawSphere(m_Points[i].position, POINT_SIZE);
	}

}

void TW_CALL MassSpringSystemSimulator::GetGravity(void* value, void* clientData)
{

	static_cast<float*> (value)[0] = static_cast<const Vec3*>(clientData)->x;
	static_cast<float*> (value)[1] = static_cast<const Vec3*>(clientData)->y;
	static_cast<float*> (value)[2] = static_cast<const Vec3*>(clientData)->z;
	
}

void TW_CALL MassSpringSystemSimulator::SetGravity(const void* value, void* clientData)
{
	static_cast<Vec3*> (clientData)->x = static_cast<const float*> (value)[0];
	static_cast<Vec3*> (clientData)->y = static_cast<const float*> (value)[1];
	static_cast<Vec3*> (clientData)->z = static_cast<const float*> (value)[2];
}

MassSpringSystemSimulator::MassSpringSystemSimulator()
{	
	// values in worksheet
	m_fStiffness = 40;
	m_fMass = 10;
	m_iTestCase = 0;
}

 char* MassSpringSystemSimulator::getIntegratorsNames() {
	return "EULER,LEAPFROG,MIDPOINT";
}

 void MassSpringSystemSimulator::reset()
 {
	 m_mouse.x = 0;
	 m_mouse.y = 0;
	 m_trackmouse.x = 0;
	 m_trackmouse.y = 0;
	 m_oldtrackmouse.x = 0;
	 m_oldtrackmouse.y = 0;
	 m_firstTime = true;
 }


 void MassSpringSystemSimulator::onClick(int x, int y)
 {
	 m_trackmouse.x = x;
	 m_trackmouse.y = y;
 }

 void MassSpringSystemSimulator::onMouse(int x, int y)
 {
	 m_oldtrackmouse.x = x;
	 m_oldtrackmouse.y = y;
	 m_trackmouse.x = x;
	 m_trackmouse.y = y;
 }

 void MassSpringSystemSimulator::groundCollision() {
	 const double ground_resistance = 0.60;
	 for (Point& point : m_Points) {
		 if (point.position.y < m_floorY && point.velocity.y < 0) // below floor and still falling
		 {
			 point.velocity.y *= -1 * ground_resistance; // lose %40 of speed when hit the ground
		 } 
		 point.position.y = std::max(point.position.y, (Real)m_floorY);
	 }
 }

 void MassSpringSystemSimulator::wallCollision() {
	// not available for now
 }

 void MassSpringSystemSimulator::simulateTimestep(float timeStep)
 {
	 accumulateForces();
	 integration(timeStep);
	 groundCollision();
	 // wallCollision();
 }

 void MassSpringSystemSimulator::accumulateForces() {
	 // update forces with damping and external
	 for (auto& point : m_Points) {
		 point.force = m_forceMouse + m_externalForce - m_fDamping * point.velocity;
	 }

	 // apply hookes law to springs
	 for (size_t i = 0; i < m_Springs.size(); i++) {
		 Vec3 F = hookesLaw(m_Springs[i]);
		 // P1 <-- P2 :  +F to P1 and -F to P2
		 m_Points[m_Springs[i].P1].force += F;
		 m_Points[m_Springs[i].P2].force -= F;
	 }
 }

 Vec3 MassSpringSystemSimulator::hookesLaw(Spring s)
 {
	 Vec3 P1 = m_Points[s.P1].position;
	 Vec3 P2 = m_Points[s.P2].position;
	 float l = sqrt(P1.squaredDistanceTo(P2));
	 Vec3 norm_dir = (P1 - P2) / l;
	 return -m_fStiffness * (l- s.initLenght) * norm_dir;
 }

 void MassSpringSystemSimulator::integration(float timestep) {
	 switch (m_iIntegrator) {
	 case EULER:
		 MidpointStep(timestep);
		 
		 if(m_iTestCase == 0 && m_firstTime) // basic one print to terminal
		 {
			 cout << "One Midpoint Step Later:" << endl;
			 for (int i = 0; i < this->m_Points.size(); i++)
			 {
				 std::cout << "Point " << i+1 << " X: " << this->m_Points[i].position << " V: " << this->m_Points[i].velocity << std::endl;
			 }
			 std::cout << "------------------------------------------------------------------------------" << std::endl;
			 
			 
			 notifyCaseChanged(0);
			 EulerStep(timestep);
			 cout << "One Euler Step Later:" << endl;
			 for (int i = 0; i < this->m_Points.size(); i++)
			 {
				 std::cout << "Point " << i+1 << " X: " << this->m_Points[i].position << " V: " << this->m_Points[i].velocity << std::endl;
			 }
			 std::cout << "------------------------------------------------------------------------------" << std::endl;
			 m_firstTime = false;
			 notifyCaseChanged(0);

		 }
		 break;
	 case MIDPOINT:
		 MidpointStep(timestep);
		 
		 break;
	 case LEAPFROG:
		 LeapFrogStep(timestep);
		 break;
	 default:
		 std::cout << "Unexpected integration method provided...\n";
		 throw "Unexpected integration method...";
		 break;
	 }
 }

 void MassSpringSystemSimulator::EulerStep(float ts) {
	 for (auto &ptr : m_Points) {
		 // first positions
		 ptr.position += ts * ptr.velocity;
		 // then update velocities
		 Vec3 a = ptr.force / m_fMass;
		 ptr.velocity +=  ts * a;
	 }
 }
 void MassSpringSystemSimulator::MidpointStep(float ts) {
	 std::vector<Point> prevPoints(m_Points);
	 
	 // take a half step
	 EulerStep(ts / 2);
	 // calculate the Forces
	 accumulateForces();

	 // write back old values but update with the half euler step ones
	 for (int i = 0; i < m_Points.size(); i++) {
			 m_Points[i].position = prevPoints[i].position;
			 m_Points[i].position += ts * m_Points[i].velocity;
			 m_Points[i].velocity = prevPoints[i].velocity;
			 m_Points[i].velocity += ts * (m_Points[i].force / m_fMass);
		 
	}
	 prevPoints.clear();
 }
 void MassSpringSystemSimulator::LeapFrogStep(float ts) {
	 for (auto& ptr : m_Points) {
		 Vec3 a = ptr.force / m_fMass;
		 ptr.velocity += ts * a;
		 // first positions
		 ptr.position += ts * ptr.velocity;
		 // then update velocities
	 }
 }

 void MassSpringSystemSimulator::setMass(float mass)
 {
	 m_fMass = mass;
 }
 void MassSpringSystemSimulator::setStiffness(float stiffness)
 {
	 m_fStiffness = stiffness;
 }
 void MassSpringSystemSimulator::setDampingFactor(float damping)
 {
	 m_fDamping = damping;
 }
 void MassSpringSystemSimulator::applyExternalForce(Vec3 force)
 {
	 m_externalForce = force;
 }

 void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
 {
	 Point2D mouseDiff;
	 mouseDiff.x = m_trackmouse.x - m_oldtrackmouse.x;
	 mouseDiff.y = m_trackmouse.y - m_oldtrackmouse.y;
	 if (mouseDiff.x != 0 || mouseDiff.y != 0)
	 {
		 Mat4 worldViewInv = Mat4(DUC->g_camera.GetWorldMatrix() * DUC->g_camera.GetViewMatrix());
		 worldViewInv = worldViewInv.inverse();
		 Vec3 inputView = Vec3((float)mouseDiff.x, (float)-mouseDiff.y, 0);
		 Vec3 inputWorld = worldViewInv.transformVectorNormal(inputView);
		 // find a proper scale!
		 float inputScale = 0.008f;
		 inputWorld = inputWorld * inputScale;
		 m_forceMouse = inputWorld;
	 }
	 else {
		 // dont touch
		 m_forceMouse = Vec3{};
	 }
 }

 int MassSpringSystemSimulator::addMassPoint(Vec3 position, Vec3 velocity, bool isFixed)
 {
	 m_Points.push_back(Point{position, velocity, {}}); // pos velo force
	 int idx = m_Points.size() - 1; // last point in the array
	 return idx;
 }

 void MassSpringSystemSimulator::addSpring(int masspoint1, int masspoint2, float initialLength)
 {
	 m_Springs.push_back(Spring{ masspoint1, masspoint2, initialLength }); // simply add the point
 }

 int MassSpringSystemSimulator::getNumberOfMassPoints()
 {
	 return m_Points.size();
 }
 int MassSpringSystemSimulator::getNumberOfSprings()
 {
	 return m_Springs.size();
 }

 Vec3 MassSpringSystemSimulator::getPositionOfMassPoint(int index)
 {
	 return m_Points[index].position;
 }
 Vec3 MassSpringSystemSimulator::getVelocityOfMassPoint(int index)
 { 
	 return m_Points[index].velocity;
 }

 const char* MassSpringSystemSimulator::getTestCasesStr() {
	 return "DEMO1,DEMO2,DEMO3,DEMO4";
 }

void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass* DUC)
{
	this->DUC = DUC;

	TwAddVarCB(DUC->g_pTweakBar, "Gravity", TW_TYPE_DIR3F, SetGravity, GetGravity, &m_externalForce, "group='Sim Params' label='Gravity'");
	TwAddVarRW(DUC->g_pTweakBar, "Stiffness", TW_TYPE_FLOAT, &m_fStiffness, " min=0 group='Sim Params' label='Stiffness K'");
	TwAddVarRW(DUC->g_pTweakBar, "Mass", TW_TYPE_FLOAT, &m_fMass, " min=0 group='Sim Params' label='Point Masses'");
	TwAddVarRW(DUC->g_pTweakBar, "Damping", TW_TYPE_FLOAT, &m_fDamping, " min=0 group='Sim Params' label='Damping'");
	
}


void MassSpringSystemSimulator::notifyCaseChanged(int testCase)
{
	m_Points.clear();
	m_Springs.clear();
	m_iTest = testCase; // can be 0 1 2 3

	switch (m_iTest) {
	case 0: // two basic points
	{
		
		*integrator_link = EULER;
		*time_link = 0.1;
		setMass(10.0f);
		setDampingFactor(0);
		setStiffness(40.0f);
		applyExternalForce(Vec3(0, 0, 0)); // no gravitiy
		int p0 = addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false);
		int p1 = addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false);
		addSpring(p0, p1, 1);
		m_firstTime = true;
		break;
	}
	case 1:
	{
		
		*integrator_link = EULER;
		*time_link = 0.005;
		setMass(10.0f);
		setDampingFactor(0);
		setStiffness(40.0f);
		applyExternalForce(Vec3(0, 0, 0)); // no gravitiy
		int p0 = addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false);
		int p1 = addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false);
		addSpring(p0, p1, 1);
		m_firstTime = false;
		break;
	}
	case 2:
	{
		
		*integrator_link = MIDPOINT;
		*time_link = 0.005;
		setMass(10.0f);
		setDampingFactor(0);
		setStiffness(40.0f);
		applyExternalForce(Vec3(0, 0, 0)); // no gravitiy
		int p0 = addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), false);
		int p1 = addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), false);
		addSpring(p0, p1, 1);
		m_firstTime = false;
		break;
	}
	case 3:
	{	
		// assortment of random points
		*time_link = 0.005;
		m_firstTime = false;
		std::mt19937 eng;
		std::uniform_real_distribution<float> randPos(-0.5f, 0.5f);
		this->addMassPoint(Vec3(-0.5, 0, 1), Vec3(0, 0, 0), false);
		this->addMassPoint(Vec3(randPos(eng), randPos(eng), randPos(eng)), Vec3(0, 0, 0), false);
		this->addMassPoint(Vec3(randPos(eng), randPos(eng), randPos(eng)), Vec3(0, 0, 0), false);
		this->addMassPoint(Vec3(randPos(eng), randPos(eng), randPos(eng)), Vec3(0, 0, 0), false);
		this->addMassPoint(Vec3(randPos(eng), randPos(eng), randPos(eng)), Vec3(0, 0, 0), false);
		this->addMassPoint(Vec3(randPos(eng), randPos(eng), randPos(eng)), Vec3(0, 0, 0), false);
		this->addMassPoint(Vec3(randPos(eng), randPos(eng), randPos(eng)), Vec3(0, 0, 0), false);
		this->addMassPoint(Vec3(randPos(eng), randPos(eng), randPos(eng)), Vec3(0, 0, 0), false);
		this->addMassPoint(Vec3(randPos(eng), randPos(eng), randPos(eng)), Vec3(0, 0, 0), false);
		this->addMassPoint(Vec3(randPos(eng), randPos(eng), randPos(eng)), Vec3(0, 0, 0), false);


		this->addSpring(0, 1, 1);
		this->addSpring(1, 2, 1);
		this->addSpring(2, 3, 1);
		this->addSpring(3, 4, 1);
		this->addSpring(4, 1, 1);
		this->addSpring(5, 1, 1);
		this->addSpring(6, 5, 1);
		this->addSpring(7, 5, 1);
		this->addSpring(7, 8, 1);
		this->addSpring(8, 9, 1);

		m_fDamping = 0;
		m_iIntegrator = 0;
		m_fStiffness = 40;


		
		
		break;
	}
	default:
	{
		// no such test
		std::cout << "No such demo case" << std::endl;
		throw "Unexpected demo case received...";
		break;
	}
	}

}

void MassSpringSystemSimulator::timeLink(float *time) {
	time_link = time;
}

void MassSpringSystemSimulator::integratorLink(int* integrator) {
	integrator_link = integrator;
}

