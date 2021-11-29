#ifndef RIGIDBODYSYSTEMSIMULATOR_h
#define RIGIDBODYSYSTEMSIMULATOR_h
#include "Simulator.h"

//add your header for your rigid body system, for e.g.,
//#include "rigidBodySystem.h" 

#define TESTCASEUSEDTORUNTEST 2

typedef struct RIGID_BODY {
	float mass;
	Vec3 pos;
	Vec3 size;
	Vec3 w;
	Vec3 v;

	Mat4 I;
	Mat4 I_current;
	Quat r;
	Mat4 R;
	

	Mat4 scale;
	Mat4 trans;

	Vec3 L;
	Vec3 q;
	Vec3 force;
	Mat4 Obj2WorldMatrix;

}rigid_body;



class RigidBodySystemSimulator:public Simulator{
public:
	// Construtors
	RigidBodySystemSimulator();
	
	// Functions
	const char * getTestCasesStr();
	void initUI(DrawingUtilitiesClass * DUC);
	void reset();
	void drawFrame(ID3D11DeviceContext* pd3dImmediateContext);
	void notifyCaseChanged(int testCase);
	void externalForcesCalculations(float timeElapsed);
	void simulateTimestep(float timeStep);
	void onClick(int x, int y);
	void onMouse(int x, int y);
	void step(float timeStep);
	static void TW_CALL GetVec(void* value, void* clientData);
	static void TW_CALL SetVec(const void* value, void* clientData);
	// ExtraFunctions
	void setTime(float* timestep);
	int getNumberOfRigidBodies();
	Vec3 getPositionOfRigidBody(int i);
	Vec3 getLinearVelocityOfRigidBody(int i);
	Vec3 getAngularVelocityOfRigidBody(int i);
	void applyForceOnBody(int i, Vec3 loc, Vec3 force);
	void addRigidBody(Vec3 position, Vec3 size, int mass);
	void setOrientationOf(int i,Quat orientation);
	void setVelocityOf(int i, Vec3 velocity);

private:
	// Attributes
	// add your RigidBodySystem data members, for e.g.,
	// RigidBodySystem * m_pRigidBodySystem; 
	Vec3 m_externalForce;
	vector<rigid_body> bodies;
	rigid_body f;
	float *time;
	Vec3 gravity;
	// UI Attributes
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;
	Vec3 m_forceMouse;
	float scale=0;
	bool col = false;
	};
#endif