
#include "MassSpringSystemSimulator.h"
#include "finalProjectUtils.h"
	
Vec3 windField(Vec3 pos, float timeElapsed)
{
	float modulus = 40;
	// Wind on the X axis
	return modulus * getNormalized(Vec3(0, 0, cos(timeElapsed) + pos.z));
}

MassSpringSystemSimulator::MassSpringSystemSimulator()
{
	m_gravityEnabled = false;
	m_groundEnabled = false;
	m_showSprings = true;
	m_firstStep = true;
	setIntegrator(EULER);
}

void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass* DUC)
{
	TwType TW_TYPE_TESTCASE = TwDefineEnumFromString("Integrator", "Euler,Midpoint");
	this->DUC = DUC;
	TwAddVarRW(DUC->g_pTweakBar, "Show Springs", TW_TYPE_BOOLCPP, &m_showSprings, "");
	TwAddVarRW(DUC->g_pTweakBar, "Integrator", TW_TYPE_TESTCASE, &m_iIntegrator, "");
	TwAddVarRW(DUC->g_pTweakBar, "Enable Gravity", TW_TYPE_BOOLCPP, &m_gravityEnabled, "");
}

void MassSpringSystemSimulator::reset()
{
	cout << "Called Reset" << endl;
	m_springs.clear();
	m_massPoints.clear();
}

void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext)
{
	//if (m_groundEnabled) DUC->DrawFloor(pd3dImmediateContext);

	//float sphereScale = 1.0 / 15.0;
	//for (auto& mp : m_massPoints)
	//{
	//	DUC->drawSphere(mp.pos, Vec3(sphereScale, sphereScale, sphereScale));
	//}

	//if (!m_showSprings) return;
	//Vec3 lineColor = Vec3(0, 1, 0);
	//for (auto& sp : m_springs)
	//{
	//	DUC->beginLine();
	//	DUC->drawLine(getMassPoint(sp.p1)->pos, lineColor, getMassPoint(sp.p2)->pos, lineColor);
	//	DUC->endLine();
	//}
}

//void MassSpringSystemSimulator::notifyCaseChanged(int testCase)
//{	
//	switch (0)
//	{
//	case 0:
//		m_firstStep = true;
//		m_gravityEnabled = false;
//		m_groundEnabled = false;
//		m_showSprings = true;
//		m_windEnabled = false;
//		TwoMassSetup();
//		break;
//
//	case 1:
//		m_gravityEnabled = false;
//		m_groundEnabled = false;
//		m_showSprings = true;
//		m_windEnabled = false;
//		setIntegrator(EULER);
//		TwoMassSetup();
//		break;
//	case 2:
//		m_gravityEnabled = false;
//		m_groundEnabled = false;
//		m_showSprings = true;
//		m_windEnabled = false;
//		setIntegrator(MIDPOINT);
//		TwoMassSetup();
//		break;
//	case 3:
//		cout << "Demo4\n";
//		m_gravityEnabled = true;
//		m_groundEnabled = true;
//		m_windEnabled = false;
//		setIntegrator(MIDPOINT);
//		ComplexSetup();
//		break;
//	default:
//		cout << "Empty Test!\n";
//		break;
//	}
// }
void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
{
	if (!m_windEnabled) return;
	for (auto& mp : m_massPoints)
	{
		mp.externalForceQueue = windField(mp.pos, timeElapsed);
	}
}

void MassSpringSystemSimulator::computeTotalForces() 
{
	for (auto& mp : m_massPoints) {
		// Clear Forces
		mp.accumForces = Vec3(0.0f, 0.0f, 0.0f);
		if (m_windEnabled) mp.accumForces += mp.externalForceQueue;
		// Add External forces
		if (m_gravityEnabled) mp.accumForces += Vec3(0, -9.81 * mp.mass, 0);
		mp.accumForces += -m_fDamping * mp.vel;
	}

	for (auto& sp : m_springs) {
		MassPoint* mp1 = getMassPoint(sp.p1);
		MassPoint* mp2 = getMassPoint(sp.p2);
		sp.currentLength = computeDistance(mp1->pos, mp2->pos);
		Vec3 dir = mp1->pos - mp2->pos;
		// Hookes law
		Vec3 force = -m_fStiffness * (sp.currentLength - sp.initialLength) * dir / sp.currentLength;
		// Apply to both ends
		mp1->accumForces += force;
		mp2->accumForces += -force;
	}
}


void MassSpringSystemSimulator::simulateTimestep(float timeStep)
{
	switch (m_iIntegrator)
	{

	case EULER:
		computeTotalForces();

		for (auto& mp : m_massPoints) {
			if (mp.fixed) continue;
			mp.pos += timeStep * mp.vel;
			if (m_groundEnabled && mp.pos.y < 0) mp.pos.y = -mp.pos.y;
			mp.vel += timeStep * mp.accumForces / mp.mass;
		}

		break;

	case MIDPOINT:

		for (auto& mp : m_massPoints) {
			if (mp.fixed) continue;
			mp.posTmp = mp.pos + timeStep / 2.0 * mp.vel;
		}
		computeTotalForces();

		for (auto& mp : m_massPoints) {
			if (mp.fixed) continue;

			mp.velTmp = mp.vel + timeStep / 2.0 * mp.accumForces / mp.mass;
			// Actually x(t + h)
			mp.posBak = mp.pos + timeStep * mp.velTmp;

			mp.velBak = mp.vel;
			mp.pos = mp.posTmp;
			mp.vel = mp.velTmp;
		}

		computeTotalForces();

		for (auto& mp : m_massPoints) {
			if (mp.fixed) continue;
			mp.vel = mp.velBak + timeStep * mp.accumForces / mp.mass;
			mp.pos = mp.posBak;
			if (m_groundEnabled && mp.pos.y < 0) mp.pos.y = -mp.pos.y;
		}

		break;

	case LEAPFROG:
		break;

	default:
		break;
	}

	if (m_firstStep) {
		m_firstStep = false;
		cout << "[State after step of h=" << timeStep << "]" << endl;
		for (int i = 0; i < m_massPoints.size(); i++)
		{
			MassPoint mp = m_massPoints[i];
			cout << "MP " << i << ": " << "Pos: " << mp.pos << ", Vel" << mp.vel << endl;
		}
	}
}


void MassSpringSystemSimulator::onClick(int x, int y)
{
	//cout << "Clickeed -> " << x << "," << y << endl;
}
void MassSpringSystemSimulator::onMouse(int x, int y) {
	

};

// Specific Functions
void MassSpringSystemSimulator::setMass(float mass) { m_fMass = mass; };

void MassSpringSystemSimulator::setStiffness(float stiffness) { m_fStiffness = stiffness; };

void MassSpringSystemSimulator::setDampingFactor(float damping) { m_fDamping = damping; };

int MassSpringSystemSimulator::addMassPoint(Vec3 position, Vec3 Velocity, bool isFixed) {
	MassPoint mp;
	mp.vel = Velocity;
	mp.pos = position;
	mp.mass = m_fMass;
	mp.fixed = isFixed;
	m_massPoints.push_back(mp);
	return m_massPoints.size() - 1; // We just return the index
}

void MassSpringSystemSimulator::addSpring(int masspoint1, int masspoint2, float initialLength)
{
	Spring sp;
	sp.p1 = masspoint1;
	sp.p2 = masspoint2;
	sp.initialLength = initialLength;
	sp.currentLength = computeDistance(getMassPoint(sp.p1)->pos, getMassPoint(sp.p2)->pos);
	m_springs.push_back(sp);
}

int MassSpringSystemSimulator::getNumberOfMassPoints() { return m_massPoints.size(); }; 

int MassSpringSystemSimulator::getNumberOfSprings() { return m_springs.size(); };

Vec3 MassSpringSystemSimulator::getPositionOfMassPoint(int index) { return m_massPoints[index].pos; }; 

Vec3 MassSpringSystemSimulator::getVelocityOfMassPoint(int index) { return m_massPoints[index].vel; };

void MassSpringSystemSimulator::applyExternalForce(Vec3 force)
{}

void MassSpringSystemSimulator::TwoMassSetup()
{
	setMass(10.0f);
	setDampingFactor(3.0f);
	setStiffness(40.0f);
	applyExternalForce(Vec3(0, 0, 0));
	int p0 = addMassPoint(Vec3(0.0, 0.0f, 0), Vec3(-1.0, 0.0f, 0), false);
	int p1 = addMassPoint(Vec3(0.0, 2.0f, 0), Vec3(1.0, 0.0f, 0), false);
	addSpring(p0, p1, 1.0);
}

void MassSpringSystemSimulator::ComplexSetup()
{

	setMass(1.0f);
	setDampingFactor(1.5f);
	setStiffness(9000.0f);
	int rowMassCount = 10;
	int columnMassCount = 7; 

	float posScale = 0.2;
	// Draw the cloth in the XY plane

	Vec3 basePosition = posScale * Vec3(-columnMassCount / 2.0, rowMassCount + 3, 0);
	// Vec3 baseVelocity = 0.1 * posScale * getNormalized(Vec3(1, 0, 0));


	int massCounter = 0;
	for (int row = 0; row < rowMassCount; row++)
	{
		for (int col = 0; col < columnMassCount; col++)
		{
			int idx = addMassPoint(basePosition + posScale * Vec3(col, 0, row), Vec3(0, 0, 0), row == 0);
			// Add spring to the left
			if (col > 0){ addSpring(massCounter - 1, massCounter, posScale); }
			// Add spring upwards
			if (row > 0) { addSpring(massCounter - columnMassCount, massCounter, posScale); }
			// Add spring left and up
			if (row > 0 && col > 0) addSpring(massCounter - columnMassCount - 1, massCounter, posScale * 1.4142);

			if (col < columnMassCount - 1 && row > 0)  addSpring(massCounter - columnMassCount + 1, massCounter, posScale * 1.4142);

			massCounter++;
		}
	}

	cout << "Finished setup" << endl;
}


MassPoint* MassSpringSystemSimulator::getMassPoint(unsigned int idx) { return &m_massPoints[idx]; }


