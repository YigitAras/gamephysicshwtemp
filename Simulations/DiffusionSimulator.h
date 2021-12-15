#ifndef DIFFUSIONSIMULATOR_h
#define DIFFUSIONSIMULATOR_h

#include "Simulator.h"
#include "vectorbase.h"
#include <vector>

//impement your own grid class for saving grid data
class Grid {
public:
	// Construtors
	Grid(int width, int height);
	Grid(int width, int height, float initialValue);
	friend ostream& operator<< (std::ostream& out, Grid const& grid);
	
	float** matrix;

	int getHeight();
	int getWidth();
	
	
	void setBorderTo(float val);
	void setRegionTo(int cornerX, int cornerY, int width, int height, float value);




private:
	// Attributes
	int m_width;
	int m_height;
	

	void createMatrix(int width, int height);
	void fillWith(float val);
};



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
	void diffuseTemperatureImplicit();


	// Added by Rafa

	int getSphereRadius();
	void setSphereRadius(int radius);
	
	int getSphereSpacing();
	void setSphereSpacing(int spacing);

	int getGridWidth();
	void setGridWidth(int width);

	int getGridHeight();
	void setGridHeight(int height);

	
private:
	// Attributes
	Vec3  m_vfMovableObjectPos;
	Vec3  m_vfMovableObjectFinalPos;
	Vec3  m_vfRotate;
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;
	Grid *T; //save results of every time step
	Grid *prevT;


	// by Rafa

	float computeNewExplicitU(int x, int y, float timeStep);

	int m_gridWidth;
	int m_gridHeight;
	int m_sphereRadius;
	int m_sphereSpacing;
	int m_tickCount;
};

#endif