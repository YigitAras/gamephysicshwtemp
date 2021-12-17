#ifndef DIFFUSIONSIMULATOR_h
#define DIFFUSIONSIMULATOR_h

#include "Simulator.h"
#include "vectorbase.h"
#include "Grid.h"
#include <vector>
#include "pcgsolver.h"

class DiffusionSimulator:public Simulator{
public:
	// Construtors
	DiffusionSimulator();

	// Functions
	const char * getTestCasesStr();
	void initUI(DrawingUtilitiesClass * DUC);
	void reset();
	void drawFrame(ID3D11DeviceContext* pd3dImmediateContext, float emissiveMult, float specMult, float specPower, float diffMult);
	void notifyCaseChanged(int testCase);
	void simulateTimestep(float timeStep);
	void externalForcesCalculations(float timeElapsed) {};
	void onClick(int x, int y);
	void onMouse(int x, int y);
	// Specific Functions
	void drawObjects(float emissiveMult, float specMult, float specPower, float diffMult);
	Grid* diffuseTemperatureExplicit(float timeStep);
	void diffuseTemperatureImplicit(float timeStep);
	int getGridWidth();
	int getGridHeight();
	Grid* getT();

	
private:
	// Attributes
	Vec3  m_vfMovableObjectPos;
	Vec3  m_vfMovableObjectFinalPos;
	Vec3  m_vfRotate;
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;
	Grid *T; //save results of every time step
	Grid *newT;
	float alpha;

	float computeNewExplicitU(int x, int y, float timeStep);

	void gridInitialSetup();

	void fillT(vector<Real>& x);
	void setupB(vector<Real>& b);
	void setupA(SparseMatrix<Real>& A, double factor);

	int m_gridWidth;
	int m_gridHeight;
	int m_sphereRadius;
	int m_sphereSpacing;
	int m_tickCount;
};

#endif