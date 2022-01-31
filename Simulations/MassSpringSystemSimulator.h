#ifndef MASSSPRINGSYSTEMSIMULATOR_h
#define MASSSPRINGSYSTEMSIMULATOR_h
#include "Simulator.h"
#include "SimulationState.h"
#include "WindField/WindField.h"
#include "ObstacleScene/ObstacleScene.h"

using namespace std;

// Do Not Change
#define EULER 0
#define MIDPOINT 1
#define LEAPFROG 2
// Do Not Change



class MassSpringSystemSimulator:public Simulator{
public:
	// Construtors
	MassSpringSystemSimulator();
	MassSpringSystemSimulator(SimulationState* state);
	
	void initUI(DrawingUtilitiesClass * DUC);
	void reset();
	
	// Do not draw here!
	void drawFrame(ID3D11DeviceContext* pd3dImmediateContext);
	void externalForcesCalculations(float timeElapsed) ;
	void simulateTimestep(float timeStep) ;
	void onClick(int x, int y) ;
	void onMouse(int x, int y) ;

	// Specific Functions
	void setMass(float mass) ;
	void setStiffness(float stiffness) ;
	void setDampingFactor(float damping) ;
	
	void applyExternalForce(Vec3 force) ;
	
	// Do Not Change
	void setIntegrator(int integrator) {
		m_iIntegrator = integrator;
	}

	void setWindField(WindField* windField) { m_windField = windField;  };
	void setObstacleScene(ObstacleScene* obstacleScene) { m_obstacleScene = obstacleScene; };

private:
	// Data Attributes
	float m_fMass;
	float m_fStiffness;
	float m_fDamping;
	int m_iIntegrator;

	// UI Attributes
	Vec3 m_externalForce;
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;

	// Added by rafa

	SimulationState* m_simulationState;

	WindField* m_windField;
	ObstacleScene* m_obstacleScene;

	bool m_gravityEnabled;

	void computeTotalForces();
}; 
#endif