
#include "MassSpringSystemSimulator.h"
#include "finalProjectUtils.h"


MassSpringSystemSimulator::MassSpringSystemSimulator() {};

MassSpringSystemSimulator::MassSpringSystemSimulator(SimulationState* state)
{
	m_gravityEnabled = true;
	m_simulationState = state;
	setIntegrator(MIDPOINT);
}

void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass* DUC)
{
	TwType TW_TYPE_TESTCASE = TwDefineEnumFromString("Integrator", "Euler,Midpoint");
	this->DUC = DUC;
	TwAddVarRW(DUC->g_pTweakBar, "Integrator", TW_TYPE_TESTCASE, &m_iIntegrator, "");
	TwAddVarRW(DUC->g_pTweakBar, "Enable Gravity", TW_TYPE_BOOLCPP, &m_gravityEnabled, "");
	// TODO: Add GUI entries for configuring the Stiffness of springs, mass and damping
}

void MassSpringSystemSimulator::reset()
{
}

void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext){}

void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed){}

void MassSpringSystemSimulator::computeTotalForces()
{
	//for (auto& mp : m_simulationState->m_massPoints) {

	//	// Clear Forces
	//	mp.accumForces = Vec3(0.0f, 0.0f, 0.0f);

	//	// TODO: check if m_windfield is ennabled and add the force

	//	// if (m_windEnabled) mp.accumForces += mp.externalForceQueue;
	//	// Add External force
	//	if (!m_windField->isEnabled()) {
	//		mp.externalForceQueue = m_windField->getForceAtPointTime(mp.pos, 0.0);
	//	}

	//	if (m_gravityEnabled) mp.accumForces += Vec3(0, -9.81 * m_fMass, 0);
	//	mp.accumForces += -m_fDamping * mp.vel;
	//}

	//for (auto& sp : m_simulationState->m_springs) {
	//	MassPoint* mp1 = m_simulationState->getMassPoint(sp.p1);
	//	MassPoint* mp2 = m_simulationState->getMassPoint(sp.p2);
	//	sp.currentLength = computeDistance(mp1->pos, mp2->pos);
	//	Vec3 dir = mp1->pos - mp2->pos;
	//	// Hookes law
	//	Vec3 force = -m_fStiffness * (sp.currentLength - sp.initialLength) * dir / sp.currentLength;
	//	// Apply to both ends
	//	mp1->accumForces += force;
	//	mp2->accumForces += -force;
	//}
}


void MassSpringSystemSimulator::simulateTimestep(float timeStep)
{
	switch (m_iIntegrator)
	{

	case EULER:
		computeTotalForces();

		for (auto& mp : m_simulationState->m_massPoints) {
			if (mp.fixed) continue;
			mp.pos += timeStep * mp.vel;
			if (mp.pos.y < 0) mp.pos.y = -mp.pos.y;
			mp.vel += timeStep * mp.accumForces / m_fMass;
		}

		break;

	case MIDPOINT:

		for (auto& mp : m_simulationState->m_massPoints) {
			if (mp.fixed) continue;
			mp.posTmp = mp.pos + timeStep / 2.0 * mp.vel;
		}
		computeTotalForces();

		for (auto& mp : m_simulationState->m_massPoints) {
			if (mp.fixed) continue;

			mp.velTmp = mp.vel + timeStep / 2.0 * mp.accumForces / m_fMass;
			// Actually x(t + h)
			mp.posBak = mp.pos + timeStep * mp.velTmp;

			mp.velBak = mp.vel;
			mp.pos = mp.posTmp;
			mp.vel = mp.velTmp;
		}

		computeTotalForces();

		for (auto& mp : m_simulationState->m_massPoints) {
			if (mp.fixed) continue;
			mp.vel = mp.velBak + timeStep * mp.accumForces / m_fMass;
			mp.pos = mp.posBak;
			if (mp.pos.y < 0) mp.pos.y = -mp.pos.y;
		}

		break;

	default:
		break;
	}
}


void MassSpringSystemSimulator::onClick(int x, int y) {};
void MassSpringSystemSimulator::onMouse(int x, int y) {};

// Specific Functions
void MassSpringSystemSimulator::setMass(float mass) { m_fMass = mass; };

void MassSpringSystemSimulator::setStiffness(float stiffness) { m_fStiffness = stiffness; };

void MassSpringSystemSimulator::setDampingFactor(float damping) { m_fDamping = damping; };


void MassSpringSystemSimulator::applyExternalForce(Vec3 force)
{}
