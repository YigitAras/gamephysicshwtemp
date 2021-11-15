#ifndef MASSSPRINGSYSTEMSIMULATOR_h
#define MASSSPRINGSYSTEMSIMULATOR_h
#include "Simulator.h"

// Do Not Change
#define EULER 0
#define LEAPFROG 1
#define MIDPOINT 2
// Do Not Change


class MassSpringSystemSimulator:public Simulator{
public:
	// Construtors
	MassSpringSystemSimulator();
	
	// UI Functions
	const char * getTestCasesStr();
	void initUI(DrawingUtilitiesClass * DUC);
	void reset();
	void drawFrame(ID3D11DeviceContext* pd3dImmediateContext);
	void notifyCaseChanged(int testCase);
	void externalForcesCalculations(float timeElapsed);
	void simulateTimestep(float timeStep);
	void onClick(int x, int y);
	void onMouse(int x, int y);

	// Specific Functions
	void setMass(float mass); //done
	void setStiffness(float stiffness); //done
	void setDampingFactor(float damping); //done
	int addMassPoint(Vec3 position, Vec3 Velocity, bool isFixed); //done
	void addSpring(int masspoint1, int masspoint2, float initialLength); //done
	int getNumberOfMassPoints(); //done
	int getNumberOfSprings(); //done
	Vec3 getPositionOfMassPoint(int index); //done
	Vec3 getVelocityOfMassPoint(int index); //done
	void applyExternalForce(Vec3 force); //done
	char* getIntegratorsNames(); //done
	void accumulateForces(); //done
	Vec3 MassSpringSystemSimulator::hookesLaw(Spring s); //done (does not work without the MassSpringSystemSimulator:: for some reason)???
	void integration(float timestep);
	void MassSpringSystemSimulator::timeLink(float* time);
	void MassSpringSystemSimulator::integratorLink(int* integrator);
	

	// Gravity
	static void TW_CALL GetGravity(void* value, void* clientData); //done
	static void TW_CALL SetGravity(const void* value, void* clientData); //done
	void groundCollision(); //done
	void wallCollision(); // will do later if time allows
	
	// Do Not Change
	void setIntegrator(int integrator) {
		m_iIntegrator = integrator;
	}

private:
	// Data Attributes
	float m_fMass;
	float m_fStiffness;
	float m_fDamping;
	int m_iIntegrator;
	bool m_firstTime = true;
	float m_TimeStep;
	float* time_link;
	int* integrator_link;
	

	// Added later
	int m_iTest = 0;
	std::vector<Point> m_Points;
	std::vector<Spring> m_Springs;
	Vec3 m_forceMouse;

	// UI Attributes
	Vec3 m_externalForce;
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;
};
#endif