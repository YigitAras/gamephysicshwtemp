#include "CoupledSimulator.h"

// Include all wind fields
#include "WindField/ExampleWindField.h"
#include "WindField/ExampleWindField2.h"

// Include all Obstacles Scenes
#include "ObstacleScene/ExampleObstacleScene.h"
#include "ObstacleScene/ExampleObstacleScene2.h"

/* 
Objective:
Simulate a hanging cloth that is influenced by changing wind and collides with
a set of different objects (called Obstacles scenes). These objects may have 
different temperatures, which are configurable in the GUI. The cloth transfers
the heat when collides with the differnet objects, causing it to change color.


Main Tasks:
There are three main set of tasks. Each one should pick one:

1) Couple MassSpringSystemSimulator with DiffusionSimulator. For this, 
the class SimulationState should be implemented and populated with all member
fields that will hold the data (and aux data) that defines the current state 
of the cloth. An instance of this class is passed to each simulator on each 
timestep and they will change the state. Both simulators should be adapted for
working with the SimulationState class. Lastly, implement the rendering in 
CoupledSimulator. All this work is basically mix the branches Rafael1 and Rafael3 
together.

Note: MassSpringSystemSimulator applies the forces and clippings defined by
the selected scene and windfield. Those instances would be assigned as a member
of MassSpringSystemSimulator, so you should use them to update the system state
accordingly.

2) Implement the Obstacles Scenes. This means to implement the methods outlined 
in ObstacleScene.h, which involves defining the objects of each scene, rendering them,
creating a GUI to change the temperature of each one and defining the functions 
that will be called by MassSpringSystemSimulator, which consist in projecting a 
point inside a volume of an obstacle to its surface (this way the cloth will not overlap)
and getting the temperature of an arbitrary point in the obstacle scene.

Having at least 5 scenes would be nice

3) Implement the WindFields. Implement the already outlined WindField class, which
involves creating some time-dependent vector fields that will be callend by MassSpringSystemSimulator
and rendering the windfield in a grid. A nice way of doing this last thing may be with
pyramids, which should me implmented using DX11 primitives such as in DrawingUtilitiesClass.
Also, the magnitude of each vector should be shown in the render in some way 
(color, height of pyramid, etc).

At least 5 WindFields would be nice too.


-------
Thats it for me! if you have questions just tell me.

Have fun! 
-Rafa
*/

// CALLBACK DEFINITIONS
void TW_CALL getWindStatus(void* value, void* clientData)
{
	*(bool*)value = (bool)INSTANCE->m_selectedWindField->isEnabled();
}

void TW_CALL setWindStatus(const void* value, void* clientData)
{
	bool val = *(const bool*)value;
	cout << "Setting all wind field states to " << val << endl;
	for (auto& wf : INSTANCE->m_windFields) { wf->setEnabled(val); };
}

void TW_CALL getWindIntensity(void* value, void* clientData)
{
	*(float*)value = (float) INSTANCE->m_selectedWindField->getIntensity();
}

void TW_CALL setWindIntensity(const void* value, void* clientData)
{
	float val = *(const float*)value;
	INSTANCE->m_selectedWindField->setIntensity(val);
}

void TW_CALL getSelectedScene(void* value, void* clientData)
{
	*(int*)value = (int)INSTANCE->getCurrentSceneIndex();
}

void TW_CALL setSelectedScene(const void* value, void* clientData)
{
	int val = *(const int*)value;
	INSTANCE->setCurrentSceneFromIndex(val);
}

void TW_CALL getSelectedWindField(void* value, void* clientData)
{
	*(int*)value = (int)INSTANCE->getCurrentWindFieldIndex();
}

void TW_CALL setSelectedWindField(const void* value, void* clientData)
{
	int val = *(const int*)value;
	INSTANCE->setCurrentWindFieldFromIndex(val);
}
// END CALLBACK DEFINITONS


CoupledSimulator::CoupledSimulator()
{
	m_drawWind = true;
	m_simulationState = nullptr;

	WindField* wf1 = new ExampleWindField();
	WindField* wf2 = new ExampleWindField2();
	// Add all others wind fields
	m_windFields.push_back(wf1);
	m_windFields.push_back(wf2);
	selectWindField(wf1);

	ObstacleScene* os1 = new ExampleObstacleScene();
	ObstacleScene* os2 = new ExampleObstacleScene2();
	m_obstacleScenes.push_back(os1);
	m_obstacleScenes.push_back(os2);
	selectObstacleScene(os1);


	m_gridWidth = 20;
	m_gridHeight = 20;
	m_sphereRadius = 10;
	m_sphereSpacing = 8;
	alpha = 20;
	T = new Grid(m_gridWidth, m_gridHeight, 0);
	newT = new Grid(m_gridWidth, m_gridHeight, 0);
	gridInitialSetup();


	// We save the pointer for callback reference
	INSTANCE = this;

	reset();
}

// Set up a clean state
void CoupledSimulator::reset() {
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;

	if (m_simulationState != nullptr) delete m_simulationState;
	m_simulationState = new SimulationState();
	m_difussionSimulator = DiffusionSimulator(m_simulationState);
	m_massSpringSimulator = MassSpringSystemSimulator(m_simulationState);
	m_massSpringSimulator.setObstacleScene(m_selectedObstacleScene);
	m_massSpringSimulator.setWindField(m_selectedWindField);
}

void CoupledSimulator::initUI(DrawingUtilitiesClass* DUC)
{
	this->DUC = DUC;

	// Adjust viewing angle
	XMFLOAT3 eye(0.0f, 4.0f, -6.0f);
	XMFLOAT3 lookAt(0.0f, 0.0f, 0.0f);
	DUC->g_camera.SetViewParams(XMLoadFloat3(&eye), XMLoadFloat3(&lookAt));

	// Added by Rafa

	TwAddVarCB(DUC->g_pTweakBar, "Enable Wind", TW_TYPE_BOOL8, setWindStatus, getWindStatus, nullptr, "");
	TwAddVarRW(DUC->g_pTweakBar, "Show Wind", TW_TYPE_BOOLCPP, &m_drawWind, "");
	TwAddVarCB(DUC->g_pTweakBar, "Wind Intensity", TW_TYPE_FLOAT, setWindIntensity, getWindIntensity, nullptr, "min=0");


	// Generate List of WindFields
	string windFieldNames;
	for (auto& wf : m_windFields) { windFieldNames += wf->getWindFieldName() + ","; }; 
	windFieldNames.pop_back(); // Remove leading comma
	const char* wfNames = windFieldNames.c_str();
	TwType TW_TYPE_WIND = TwDefineEnumFromString("Wind Type", wfNames);
	TwAddVarCB(DUC->g_pTweakBar, "Wind Type", TW_TYPE_WIND, setSelectedWindField, getSelectedWindField, nullptr, "");


	// Generate List of Obstacle scenes (Sorry for duplicated and dirty code xd)
	string obstacleSceneNames;
	for (auto& sc : m_obstacleScenes) { obstacleSceneNames += sc->getSceneName() + ","; };
	obstacleSceneNames.pop_back();
	const char* scNames = obstacleSceneNames.c_str();
	TwType TW_TYPE_SCENE = TwDefineEnumFromString("Obstacles", scNames);
	TwAddVarCB(DUC->g_pTweakBar, "Obstacles", TW_TYPE_SCENE, setSelectedScene, getSelectedScene, nullptr,  "");
	
	m_massSpringSimulator.initUI(DUC); // Add specific switches like gravity and integrators
	m_difussionSimulator.initUI(DUC);
	m_selectedObstacleScene->initUI(DUC); // Draw the menu to tune each objects temperature
}


void CoupledSimulator::simulateTimestep(float timeStep)
{
	
	m_massSpringSimulator.simulateTimestep(timeStep);
	m_difussionSimulator.simulateTimestep(timeStep);
}

Vec3 colorFunc(float x) {
	float y = pow(x, 2);
	return Vec3(y, 0, 1 - y);
}

void CoupledSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContextt)
{
	// This will be the only simulator that draws. The other two subsimulator will just compute a new state

	// Draw the wind field in case is enabled
	if (m_selectedWindField->isEnabled()) m_selectedWindField->drawWindField(0, DUC);

	m_selectedObstacleScene->drawObstacles(DUC);
	
	// TODO: Draw the cloth with respective colors! See branch Rafael1

	float sphereScale = m_sphereRadius / 100.0;
	float spaceScale = m_sphereSpacing / 50.0;
	float value;


	for (int y = 0; y < m_gridHeight; y++)
	{
		for (int x = 0; x < m_gridWidth; x++)
		{

			float input = T->get(x, y) / 10.0;
			// float input = x / (m_gridWidth - 1.0);
			DUC->setUpLighting(colorFunc(input), specMult * Vec3(1, 1, 1), specPower, diffMult * 0.05 * Vec3(1, 1, 1));
			DUC->drawSphere(Vec3(-1 * spaceScale * m_gridWidth / 2.0, -1 * spaceScale * m_gridHeight / 2.0, 0) + Vec3(x * spaceScale, y * spaceScale, 0), Vec3(sphereScale, sphereScale, sphereScale));
		}
	}

}

void CoupledSimulator::externalForcesCalculations(float timeElapsed)
{
}


void CoupledSimulator::onClick(int x, int y)
{
	m_trackmouse.x = x;
	m_trackmouse.y = y;
	// Propagate the event
	m_difussionSimulator.onClick(x, y);
	m_massSpringSimulator.onClick(x, y);

}

void CoupledSimulator::onMouse(int x, int y)
{
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
	// Propagate the event
	m_difussionSimulator.onMouse(x, y);
	m_massSpringSimulator.onMouse(x, y);
}

int CoupledSimulator::getCurrentSceneIndex() {
	int idx = 0;
	for (auto& sc : m_obstacleScenes) { 
		if (m_selectedObstacleScene == sc) return idx;
		idx++;
	};
	return -1;
}

void CoupledSimulator::setCurrentSceneFromIndex(int idx) {
	ObstacleScene* newScene = m_obstacleScenes[idx];
	m_selectedObstacleScene = newScene;
	m_massSpringSimulator.setObstacleScene(newScene);
}

// Sorry for repeated code, im tired 
int CoupledSimulator::getCurrentWindFieldIndex() {
	int idx = 0;
	for (auto& wf : m_windFields) {
		if (m_selectedWindField == wf) return idx;
		idx++;
	};
	return -1;
}

void CoupledSimulator::setCurrentWindFieldFromIndex(int idx) {
	WindField* newField = m_windFields[idx];
	m_selectedWindField = newField;
	m_massSpringSimulator.setWindField(newField);
}
