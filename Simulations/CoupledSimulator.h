#ifndef COUPLEDSIMULATOR_h
#define COUPLEDSIMULATOR_h

#include "Simulator.h"
#include "vectorbase.h"
#include <vector>

#include "SimulationState.h"
#include "DiffusionSimulator.h"
#include "MassSpringSystemSimulator.h"
#include "WindField/WindField.h"
#include "ObstacleScene/ObstacleScene.h"



class CoupledSimulator:public Simulator {
public:
	// Construtors
	CoupledSimulator();

	// Functions
	void initUI(DrawingUtilitiesClass* DUC);
	void reset();
	void drawFrame(ID3D11DeviceContext* pd3dImmediateContext);
	void externalForcesCalculations(float timeElapsed);
	void simulateTimestep(float timeStep);
	
	void onClick(int x, int y);
	void onMouse(int x, int y);

	int getCurrentSceneIndex();
	void setCurrentSceneFromIndex(int idx);

	int getCurrentWindFieldIndex();
	void setCurrentWindFieldFromIndex(int idx);

	// publicly privated: These should be private but otherwise i cant reference them easily on callbacks
	WindField* m_selectedWindField;
	ObstacleScene* m_selectedObstacleScene;
	vector<WindField*> m_windFields;
	vector<ObstacleScene*> m_obstacleScenes;

private:
	// Attributes
	Vec3  m_vfMovableObjectPos;
	Vec3  m_vfMovableObjectFinalPos;
	Vec3  m_vfRotate;
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;

	Grid* T; //save results of every time step
	Grid* newT;
	float alpha;

	int m_gridWidth;
	int m_gridHeight;
	int m_sphereRadius;
	int m_sphereSpacing;
	int m_tickCount;

	void gridInitialSetup();

	float emissiveMult = 1;
	float specMult = 5;
	float specPower = 70;
	float diffMult = 0.3;
	

	// Store the current state of the cloth and its temperature
	SimulationState* m_simulationState;

	// Update position of cloth
	MassSpringSystemSimulator m_massSpringSimulator;

	// Update temperature of cloth
	DiffusionSimulator m_difussionSimulator;

	void selectWindField(WindField* wf) { m_selectedWindField = wf; };
	void selectObstacleScene(ObstacleScene* obsScene) { m_selectedObstacleScene = obsScene;  };

	bool m_drawWind;
};

// Dirty singleton workaround for passing member function as callbacks
static CoupledSimulator* INSTANCE;

#endif