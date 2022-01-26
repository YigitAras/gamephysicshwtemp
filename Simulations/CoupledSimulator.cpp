#include "CoupledSimulator.h"

// Include all wind fields
#include "WindField/ExampleWindField.h"

// Include all Obstacles Scenes
#include "ObstacleScene/ExampleObstacleScene.h"

/* 
TODO:
- Choose between a set of scene. On each scene be able to change the temperature of objects 
with the tweakbar
- Choose Timestep
- Choose Integrator for the cloth
- Choose Integrator for difussion
- Wrap around objects
- Wind Field that varies with time
- Draw windfield as a 3D grid of pyramids pointing in the direction of wind
*/

CoupledSimulator::CoupledSimulator()
{
	m_drawWind = true; 
	m_windEnabled = true;


	WindField* wf = new ExampleWindField();
	// Add all others wind fields
	m_windFields.push_back(wf);
	selectWindField(wf);

	ObstacleScene* os = new  ExampleObstacleScene();
	m_obstacleScenes.push_back(os);
	selectObstacleScene(os);
}


void CoupledSimulator::reset() {
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
}

void CoupledSimulator::initUI(DrawingUtilitiesClass* DUC)
{
	this->DUC = DUC;

	
	// XMFLOAT3 eye(0.0f, 0.0f, -m_gridWidth / 2.0);
	// XMFLOAT3 lookAt(0.0f, 0.0f, 0.0f);
	// DUC->g_camera.SetViewParams(XMLoadFloat3(&eye), XMLoadFloat3(&lookAt));

	// Added by Rafa
	// TwAddVarRW(DUC->g_pTweakBar, "Grid Width", TW_TYPE_UINT16, &m_gridWidth, "min=0");
	// TwAddVarRW(DUC->g_pTweakBar, "Grid Height", TW_TYPE_UINT16, &m_gridHeight, "min=0");
	// TwAddVarRW(DUC->g_pTweakBar, "Particle Radius", TW_TYPE_UINT16, &m_sphereRadius, "min=1");
	// TwAddVarRW(DUC->g_pTweakBar, "Particle Spacing", TW_TYPE_UINT16, &m_sphereSpacing, "min=1");
	// TwAddVarRW(DUC->g_pTweakBar, "Alpha", TW_TYPE_FLOAT, &alpha, "min=0 step=0.05");
	// TwAddVarCB(DUC->g_pTweakBar, "Grid Width", TW_TYPE_UINT16, setGridWidthCallback, getGridWidthCallback, nullptr, "min=0");
	// TwAddVarCB(DUC->g_pTweakBar, "Grid Height", TW_TYPE_UINT16, setGridHeightCallback, getGridHeightCallback, nullptr, "min=0");

	// Stop the simulation and then check step by step in console
	// TwAddButton(DUC->g_pTweakBar, "Single Step", stepForwardCallback, nullptr, "");
	// reset();


	TwAddVarRW(DUC->g_pTweakBar, "Enable Wind", TW_TYPE_BOOLCPP, &m_windEnabled, "");
	m_massSpringSimulator.initUI(DUC);
	m_difussionSimulator.initUI(DUC);
}


void CoupledSimulator::simulateTimestep(float timeStep)
{
	lastTimeStep = timeStep;
	m_massSpringSimulator.simulateTimestep(timeStep);
	m_difussionSimulator.simulateTimestep(timeStep);
}


void CoupledSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext)
{
	// This will be the only simulator that draws. The other two subsimulator will just compute a new state
	
}

void CoupledSimulator::onClick(int x, int y)
{
	m_trackmouse.x = x;
	m_trackmouse.y = y;
	//cout << *T << "\n\n";
}

void CoupledSimulator::onMouse(int x, int y)
{
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}
