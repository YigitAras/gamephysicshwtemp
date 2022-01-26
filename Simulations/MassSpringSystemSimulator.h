#ifndef MASSSPRINGSYSTEMSIMULATOR_h
#define MASSSPRINGSYSTEMSIMULATOR_h
#include "Simulator.h"
#include "WindField/WindField.h"
#include "ObstacleScene/ObstacleScene.h"

// Do Not Change
#define EULER 0
#define MIDPOINT 1
#define LEAPFROG 2
// Do Not Change

typedef struct MassPointStruct
{
	Vec3 pos;
	Vec3 vel;
	Vec3 accumForces;
	float mass;
	bool fixed;

	// Midpoint aux members
	Vec3 posTmp;
	Vec3 velTmp;
	Vec3 posBak;
	Vec3 velBak;
	Vec3 externalForceQueue; 

} MassPoint;

typedef struct SpringStruct
{
	unsigned int p1;
	unsigned int p2; 
	float initialLength;
	float currentLength;
} Spring;


class MassSpringSystemSimulator:public Simulator{
public:
	// Construtors
	MassSpringSystemSimulator();
	
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
	int addMassPoint(Vec3 position, Vec3 Velocity, bool isFixed);
	void addSpring(int masspoint1, int masspoint2, float initialLength) ;
	int getNumberOfMassPoints();
	int getNumberOfSprings();
	Vec3 getPositionOfMassPoint(int index);
	Vec3 getVelocityOfMassPoint(int index);
	void applyExternalForce(Vec3 force) ;
	
	// Do Not Change
	void setIntegrator(int integrator) {
		m_iIntegrator = integrator;
	}

	void setWindFieldEnabled(bool status) { m_windEnabled = status; };
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

	bool m_firstStep;

	WindField* m_windField;
	ObstacleScene* m_obstacleScene;
	
	std::vector<Spring> m_springs;
	std::vector<MassPoint> m_massPoints;

	bool m_gravityEnabled;
	bool m_windEnabled;
	bool m_groundEnabled;
	bool m_showSprings;

	void computeTotalForces();
	void TwoMassSetup();
	void ComplexSetup();
	MassPoint* getMassPoint(unsigned int idx);
}; 
#endif