#ifndef COUPLEDSIMULATOR_h
#define COUPLEDSIMULATOR_h

#include "Simulator.h"
#include "vectorbase.h"
#include "Grid.h"
#include <vector>
#include "pcgsolver.h"


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
	void simulateTimestep(float timeStep);
	void externalForcesCalculations(float timeElapsed) {};
	void onClick(int x, int y);
	void onMouse(int x, int y);


	// Old
	// Specific Functions
	void drawObjects(float emissiveMult, float specMult, float specPower, float diffMult);
	Grid* diffuseTemperatureExplicit(float timeStep);
	void diffuseTemperatureImplicit(float timeStep);
	int getGridWidth();
	void setGridWidth(int width);
	int getGridHeight();
	void setGridHeight(int height);
	Grid* getT();
	float lastTimeStep; // Used for single step button


private:
	// Attributes
	Vec3  m_vfMovableObjectPos;
	Vec3  m_vfMovableObjectFinalPos;
	Vec3  m_vfRotate;
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;
	
	vector<WindField*> m_windFields;
	vector<ObstacleScene*> m_obstacleScenes;
	DiffusionSimulator m_difussionSimulator;
	MassSpringSystemSimulator m_massSpringSimulator;

	void selectWindField(WindField* wf) { m_selectedWindField = wf; };
	void selectObstacleScene(ObstacleScene* obsScene) { m_selectedObstacleScene = obsScene;  };

	bool m_drawWind;
	bool m_windEnabled;
	WindField* m_selectedWindField;
	ObstacleScene* m_selectedObstacleScene;
};

#endif