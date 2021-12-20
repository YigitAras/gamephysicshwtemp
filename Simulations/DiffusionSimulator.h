#ifndef DIFFUSIONSIMULATOR_h
#define DIFFUSIONSIMULATOR_h

#include "Simulator.h"
#include "vectorbase.h"

//impement your own grid class for saving grid data
class Grid {
public:
	// Construtors
	Grid(int x, int y);
	float get(int x, int y);
	int get_x();
	int get_y();
	void set(int x, int y, float v);
	void reset(int x, int y);
	float get(int x);

private:
	// Attributes
	//vector<vector<float>>* grid;
	vector<vector<float>> grid;
};



class DiffusionSimulator:public Simulator{
public:
	// Construtors
	DiffusionSimulator();

	// Functions
	const char * getTestCasesStr();
	void initUI(DrawingUtilitiesClass * DUC);
	void reset();
	void drawFrame(ID3D11DeviceContext* pd3dImmediateContext);
	void notifyCaseChanged(int testCase);
	void simulateTimestep(float timeStep);
	void externalForcesCalculations(float timeElapsed) {};
	void onClick(int x, int y);
	void onMouse(int x, int y);
	// Specific Functions
	void drawObjects();
	Grid* diffuseTemperatureExplicit();
	void diffuseTemperatureImplicit();
	void init_grid();
	void set_time_pointer(float* time);
	

	static void TW_CALL get_value_x(void* value, void* clientData);
	static void TW_CALL get_value_y(void* value, void* clientData);
	static void TW_CALL set_value_x(const void* value, void* clientData);
	static void TW_CALL set_value_y(const void* value, void* clientData);
private:
	// Attributes
	Vec3  m_vfMovableObjectPos;
	Vec3  m_vfMovableObjectFinalPos;
	Vec3  m_vfRotate;
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;
	Grid *T; //save results of every time step
	float alpha;
	float* timestep;
	int n;
	int m;
	int temp;

};

#endif