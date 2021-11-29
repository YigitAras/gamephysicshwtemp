#include "RigidBodySystemSimulator.h"


// for now nothing / constructor
RigidBodySystemSimulator::RigidBodySystemSimulator(){
	m_fAirFriction = 1;
	m_fBounciness = 1;
	m_trackmouse = { 0,0 };
	m_oldtrackmouse = {0,0};
	time_link = NULL;
}


// SIMULATION UI AND SETUP RELATED THINGS

const char* RigidBodySystemSimulator::getTestCasesStr()
{
	return "DEMO1, DEMO2, DEMO3, DEMO4";
}

void RigidBodySystemSimulator::initUI(DrawingUtilitiesClass* DUC)
{
	this->DUC = DUC;

	TwAddVarRW(DUC->g_pTweakBar, "Bounciness", TW_TYPE_FLOAT, &m_fBounciness, " min=0 max=1 group='Simulation Params' label='Co-efficient of object bounciness'");
	TwAddVarRW(DUC->g_pTweakBar, "Friction/Damping", TW_TYPE_FLOAT, &m_fAirFriction, " min=0 max=1 group='Simulation Params' label='Air friction'");
	TwAddVarCB(DUC->g_pTweakBar, "Gravity", TW_TYPE_DIR3F, SetGravityCallback, GetGravityCallback, &v_gravity, "group='Simulation Params' label='Gravity'");
}

void TW_CALL RigidBodySystemSimulator::GetGravityCallback(void* value, void* clientData)
{

	static_cast<float*> (value)[0] = static_cast<const Vec3*>(clientData)->x;
	static_cast<float*> (value)[1] = static_cast<const Vec3*>(clientData)->y;
	static_cast<float*> (value)[2] = static_cast<const Vec3*>(clientData)->z;
}

void TW_CALL RigidBodySystemSimulator::SetGravityCallback(const void* value, void* clientData)
{
	static_cast<Vec3*> (clientData)->x = static_cast<const float*> (value)[0];
	static_cast<Vec3*> (clientData)->y = static_cast<const float*> (value)[1];
	static_cast<Vec3*> (clientData)->z = static_cast<const float*> (value)[2];
}

void RigidBodySystemSimulator::setGravity(Vec3 force) {
	v_gravity = force;
}

void RigidBodySystemSimulator::onClick(int x, int y)
{
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void RigidBodySystemSimulator::onMouse(int x, int y)
{
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void RigidBodySystemSimulator::reset()
{
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
}

void RigidBodySystemSimulator::timeLink(float* time) {
	time_link = time;
}

void RigidBodySystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext)
{
	
	for (int i = 0; i < vBodies.size(); i++)
	{
		
		DUC->drawRigidBody(vBodies[i].getObjToWorldMatrix());
	
	}
}

void RigidBodySystemSimulator::notifyCaseChanged(int testCase)
{
	
	setGravity(Vec3(0,0,0));
	vBodies.clear();
	switch (testCase)
	{
	case 0:
	{
		// set time similar to the demo
		m_iTestCase = 0;
		*time_link = 2;
		addRigidBody({ 0, 0, 0 }, { 1, 0.6, 0.5 }, 2);
		applyForceOnBody(0, { 0.3, 0.5, 0.25 }, { 1,1,0 });
		// around Z by 90 degrees
		setOrientationOf(0, Quat(Vec3(0.0f, 0.0f, 1.0f), (float)(M_PI) * 0.5f));
		break;
	}
	case 1:
		*time_link = 0.001f;
		m_iTestCase = 1;
		addRigidBody({ 0, 0, 0 }, { 1, 0.6, 0.5 }, 2);
		applyForceOnBody(0, { 0.3, 0.5, 0.25 }, { 1,1,0 });
		// around Z by 90 degrees
		setOrientationOf(0, Quat(Vec3(0.0f, 0.0f, 1.0f), (float)(M_PI) * 0.5f));
		break;

	case 2:
	{
		*time_link = 0.001f;
		m_iTestCase = 2;
		addRigidBody(Vec3(-0.1f, -0.2f, 0.1f), Vec3(0.4f, 0.2f, 0.2f), 4);
		addRigidBody(Vec3(0.0f, 0.2f, 0.0f), Vec3(0.4f, 0.2f, 0.2f), 2);
		setOrientationOf(1, Quat(Vec3(0.0f, 0.0f, 1.0f), (float)(M_PI) * 0.25f));
		setVelocityOf(1, Vec3(0.0f, -0.1f, 0.05f));
		
		
		break;
	}
	case 3:
	{
		*time_link = 0.001f;
		m_iTestCase = 3;
		addRigidBody(Vec3(-0.3f, -0.3f, 0.1f), Vec3(0.4f, 0.2f, 0.2f), 4);

		addRigidBody(Vec3(0.10f, 0.40f, 0.10f), Vec3(0.4f, 0.2f, 0.2f), 2);
		setOrientationOf(1, Quat(Vec3(0.0f, 0.0f, 1.0f), (float)(M_PI) * 0.25f));
		setVelocityOf(1, Vec3(0.0f, -0.1f, 0.05f));
		
		addRigidBody({ 0, 0, 0 }, { 0.4, 0.2, 0.2 }, 2);
		applyForceOnBody(2, { 0.3, 0.5, 0.25 }, { 1,1,0 });
		
		addRigidBody({ 0.5, 0.5, 0.5 }, { 0.1, 0.1, 0.1 }, 2);
		applyForceOnBody(3, { 0.3, 0.5, 0.25 }, { -1,-1,0 });
		break;
	}
	case 4:
	{
		break;
	}
	default:
		break;
	}

}

// METHODS ON RIGIDBODIES

int RigidBodySystemSimulator::addRigidBody(Vec3 position, Vec3 size, double mass) {
	vBodies.push_back(RigidBody(position, size, mass));
	return vBodies.size() - 1;
}

int RigidBodySystemSimulator::getNumberOfRigidBodies() {
	return vBodies.size();
}

Vec3 RigidBodySystemSimulator::getPositionOfRigidBody(int i) {
	return vBodies[i].vPos;
}
Vec3 RigidBodySystemSimulator::getLinearVelocityOfRigidBody(int i) {
	return vBodies[i].vVel;
}
Vec3 RigidBodySystemSimulator::getAngularVelocityOfRigidBody(int i) {
	return vBodies[i].vAngular_momentum;
}

void RigidBodySystemSimulator::setOrientationOf(int i, Quat orientation) {
	vBodies[i].qAng_ori = orientation;
}
void RigidBodySystemSimulator::setVelocityOf(int i, Vec3 velocity) {
	vBodies[i].vVel = velocity;
}


void RigidBodySystemSimulator::externalForcesCalculations(float timeElapsed)
{
	// TODO: add interactivity probably
	v_mouse = 0;

	// F = m*a boiiii
	for (auto& body : vBodies)
			body.vForce += (v_gravity + v_mouse) * body.fMass; // Assume gravitiy on center causing no torque
}

void RigidBodySystemSimulator::applyForceOnBody(int i, Vec3 loc, Vec3 force) {
	//simply add the force for linear
	vBodies[i].vForce += force;
	//for torque it is sum of (x_i  X  force)
	vBodies[i].vTorque += cross(loc - vBodies[i].vPos, force);
}

// METHODS ON INTEGRATION



void RigidBodySystemSimulator::simulateTimestep(float timeStep)
{
	
	collisionIntegration();
	integrateLinearEuler(timeStep);
	integrateAngularEuler(timeStep);
	clearBodyForces();
}

void RigidBodySystemSimulator::collisionIntegration() {
	for (int i = 0; i < vBodies.size() - 1; i++)
	{
		// check each body
		auto& a = vBodies[i];
		Mat4 mA = GamePhysics::Mat4(a.getObjToWorldMatrix());

		for (auto j = i + 1; j < vBodies.size(); j++)
		{
			auto& b = vBodies[j];
			Mat4 mB = GamePhysics::Mat4(b.getObjToWorldMatrix());
			CollisionInfo ci = checkCollisionSAT(mA, mB);
			if (ci.isValid) {
				auto coll_normal = ci.normalWorld;
				auto coll_pos_world = ci.collisionPointWorld;

				// Calculate velocities at collision point

				Vec3 colPosA = coll_pos_world - a.vPos; //relative a
				Vec3 colPosB = coll_pos_world - b.vPos; // relative b

				Vec3 veloPointA = a.vVel + cross(a.vAngular_velocity, colPosA);
				Vec3 veloPointB = b.vVel + cross(b.vAngular_velocity, colPosB);

				// Calculate relative velocity * n
				auto relativeVelocity = dot(veloPointA - veloPointB, coll_normal);

				if (relativeVelocity > 0) continue; // already separating
				// Fill in the Formula
				// J = numerator / denominator
				// numerator  = -(1 + c)V_rel * n
				// denominator = 1/M_a + 1/M_b + [(Ia_inv ( x_a X n)) X x_a + (Ib_inv ( x_b X n)) X x_b] * n
				auto numerator = -(1 + m_fBounciness) * relativeVelocity;
				
				auto reverseInvPartA = cross(a.current_I_inv() * cross(colPosA, coll_normal), colPosA);
				auto reverseInvPartB = cross(b.current_I_inv() * cross(colPosB, coll_normal), colPosB);

				// This is absolutely wrong :D:D:D:D
				//auto reverseInvPartA = cross(a.I_inv * cross(colPosA, coll_normal), colPosA);
				//auto reverseInvPartB = cross(b.I_inv * cross(colPosB, coll_normal), colPosB);

				auto denominator = 1 / (double)a.fMass + 1 / (double)b.fMass + dot((reverseInvPartA + reverseInvPartB), coll_normal);
				auto J = numerator / denominator;

				// Update with Impulse
				 a.vVel = a.vVel + J * coll_normal * 1 / a.fMass;
				 b.vVel = b.vVel - J * coll_normal * 1 / b.fMass;
				 a.vAngular_momentum = a.vAngular_momentum + cross(colPosA, J * coll_normal);
				 b.vAngular_momentum = b.vAngular_momentum - cross(colPosB, J * coll_normal);
				 
				
				

			}
		}
	}
}

void RigidBodySystemSimulator::integrateLinearEuler(float timeStep) {
	// simple Euler step
	for (auto& body : vBodies)
	{
		// update the position Euler step
		body.vPos += timeStep * body.vVel;
		// update the velocity with a = F/m Euler step
		body.vVel += timeStep * body.vForce / body.fMass;
		
	}
	if (m_iTestCase == 0 && b_firstTime) {
		std::cout << "Velocities after one step: " << std::endl;
		for (int i = 0; i < this->vBodies.size(); i++)
		{
			
			std::cout << "Point " << i + 1 << " Linear Velo: " << this->vBodies[i].vVel << std::endl;
		}
		

	}
}

void RigidBodySystemSimulator::integrateAngularEuler(float timeStep) {
	for (auto& body : vBodies) {
		// get Angular Velocity W
		Quat ang_velocity = Quat{ body.vAngular_velocity.x, body.vAngular_velocity.y, body.vAngular_velocity.z, 0 };
		// r' = r + t/2 * (0 W).transpose() * r
		body.qAng_ori += timeStep / 2 * ang_velocity * body.qAng_ori;
		// Quaternios need to be unit lenght
		body.qAng_ori = body.qAng_ori.unit();

		// integrate now Angular Momentum
		// L(t+h) = L(t) + h*q
		body.vAngular_momentum += timeStep * body.vTorque;
		// add dampening to rotational momentum if exists
		body.vAngular_momentum *= m_fAirFriction;

		//integrate Angular Velocity now
		// w(t+h) = I^(-1) * L(t+h)
		auto I_inv = body.current_I_inv();
		
		body.vAngular_velocity = I_inv * body.vAngular_momentum;
	}
	if (m_iTestCase == 0 && b_firstTime) {
		
		for (int i = 0; i < this->vBodies.size(); i++)
		{
			std::cout << "Angular Velo: " << this->vBodies[i].vAngular_velocity << std::endl;
			//velocity + cross(Angular, point - position)
			std::cout << "Velocity at Point (0.3, 0.5, 0.25): ";
			auto pos = Vec3(0.3, 0.5, 0.25);
			auto tmp_velo = this->vBodies[i].vVel + cross(vBodies[i].vAngular_velocity, -(pos - vBodies[i].vPos));
			std::cout << tmp_velo << std::endl;

		}
		std::cout << "------------------------------------------------------------------------------" << std::endl;
		b_firstTime = false;
	}
}

void RigidBodySystemSimulator::clearBodyForces()
{
	for (auto& body : vBodies)
	{
		// clear the forces for the next step
		body.vForce = body.vTorque = 0;
	}
}

