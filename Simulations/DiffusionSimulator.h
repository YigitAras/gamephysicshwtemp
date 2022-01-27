#ifndef DIFFUSIONSIMULATOR_h
#define DIFFUSIONSIMULATOR_h

#include "Simulator.h"
#include "vectorbase.h"
#include "Grid.h"
#include <vector>
#include "pcgsolver.h"
#include "SimulationState.h"

#define EXPLICIT_SOLVER 0
#define IMPLICIT_SOLVER 1


class DiffusionSimulator:public Simulator{
public:
	// Construtors
	DiffusionSimulator(); 
	DiffusionSimulator(SimulationState* state);

	void initUI(DrawingUtilitiesClass * DUC);
	void reset();
	void drawFrame(ID3D11DeviceContext* pd3dImmediateContext);
	void simulateTimestep(float timeStep);
	void externalForcesCalculations(float timeElapsed) {};
	void onClick(int x, int y);
	void onMouse(int x, int y);
	// Specific Functions
	Grid* diffuseTemperatureExplicit(float timeStep);
	void diffuseTemperatureImplicit(float timeStep);
	
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


	void fillT(vector<Real>& x);
	void setupB(vector<Real>& b);
	void setupA(SparseMatrix<Real>& A, double factor);

	int m_sphereRadius;
	int m_sphereSpacing;
	int m_tickCount;
	int m_selectedSolver;
	SimulationState* m_simulationState;
};

#endif