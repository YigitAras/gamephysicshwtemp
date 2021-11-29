#ifndef RIGIDBODYSYSTEMSIMULATOR_h
#define RIGIDBODYSYSTEMSIMULATOR_h
#include "Simulator.h"
#include "collisionDetect.h"

//add your header for your rigid body system, for e.g.,
//#include "rigidBodySystem.h" 

#define TESTCASEUSEDTORUNTEST 2

// Assuming stuff are boxes only, hopefully
class RigidBody {
	
public:
	RigidBody(Vec3 pos, Vec3 size, double mass) : vPos(pos), vSize(size), isFixed(true), fMass(mass), I_inv(I_Cube_inv(size, mass)) {}

	bool isFixed;
	float fMass;
	Vec3 vPos;
	Vec3 vSize;
	Vec3 vVel;
	Vec3 vForce;
	Vec3 vTorque;
	Vec3 vAngular_momentum;
	Vec3 vAngular_velocity;
	Quat qAng_ori{ 0,0,0,1 };
	Mat4d I_inv;

	Mat4d current_I_inv() {
		auto r4x4 = qAng_ori.getRotMat();

		// I_curr = rot * I_init * rot^-1
		// my god this is an ugly way of doing it
		auto updated = r4x4 * I_inv;
		r4x4.transpose();
		updated = (updated * r4x4);


		return updated;
	}

	XMMATRIX getObjToWorldMatrix() {


		const XMMATRIX Rotation = qAng_ori.getRotMat().toDirectXMatrix();

		Mat4f a;
		a.initScaling(vSize[0], vSize[1], vSize[2]);
		const XMMATRIX Scale = a.toDirectXMatrix();

		a.initTranslation(vPos[0], vPos[1], vPos[2]);
		const XMMATRIX Translation = a.toDirectXMatrix();
		const XMMATRIX ObjToWorld = Scale * Rotation * Translation;
		return ObjToWorld;
	}

	
	
private:
	Mat4d I_Cube_inv(Vec3 size ,float mass) {
		Mat4d I;
		I.initId();
		
		// size.y = H , size.x = W , size.z = D
		// Formula yoinked from Wikipedia
		I.value[0][0] = 1.0 / 12 * mass * (size.y * size.y + size.z * size.z);
		I.value[1][1] = 1.0 / 12 * mass * (size.x * size.x + size.z * size.z);
		I.value[2][2] = 1.0 / 12 * mass * (size.x * size.x + size.y * size.y);
		
		// diagonal entries inverted inverses the matrix
		I.value[0][0] = 1 / I.value[0][0];
		I.value[1][1] = 1 / I.value[1][1];
		I.value[2][2] = 1 / I.value[2][2];
		return I;
	}

};

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
	void timeLink(float* time);
	void resolveCollisions();
	void integrateLinearEuler(float timeStep);
	void integrateAngularEuler(float timeStep);
	void clearBodyForces();

	// ExtraFunctions
	int getNumberOfRigidBodies();
	Vec3 getPositionOfRigidBody(int i);
	Vec3 getLinearVelocityOfRigidBody(int i);
	Vec3 getAngularVelocityOfRigidBody(int i);
	void applyForceOnBody(int i, Vec3 loc, Vec3 force);
	int addRigidBody(Vec3 position, Vec3 size, double mass);
	void setOrientationOf(int i,Quat orientation);
	void setVelocityOf(int i, Vec3 velocity);
	void setGravity(Vec3 force);


	static void TW_CALL GetGravityCallback(void* value, void* clientData);
	static void TW_CALL SetGravityCallback(const void* value, void* clientData);
	int m_iTestCase = 1;
	bool b_firstTime = true;


private:
	// Attributes
	// add your RigidBodySystem data members, for e.g.,
	// RigidBodySystem * m_pRigidBodySystem; 
	Vec3 v_externalForce;
	Vec3 v_gravity;
	Vec3 v_mouse;
	std::vector<RigidBody> vBodies;
	

	// Object coeffs
	float m_fBounciness = 1;
	float m_fAirFriction = 1;
	float* time_link;

	// UI Attributes
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;
	};
#endif