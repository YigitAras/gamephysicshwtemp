#include "DiffusionSimulator.h"

using namespace std;


DiffusionSimulator::DiffusionSimulator() {};

DiffusionSimulator::DiffusionSimulator(SimulationState* state)
{
	m_simulationState = state;
	m_selectedSolver = EXPLICIT_SOLVER;
}


void DiffusionSimulator::reset(){
	// This class does not hold state, no reset needed
}



void DiffusionSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;
	
	TwType TW_TYPE_SOLVER = TwDefineEnumFromString("T Solver", "Explicit Solver,Implicit Solver");
	TwAddVarRW(DUC->g_pTweakBar, "T Solver", TW_TYPE_SOLVER, &m_selectedSolver, "");

	//// Added by Rafa
	//// TwAddVarRW(DUC->g_pTweakBar, "Grid Width", TW_TYPE_UINT16, &m_gridWidth, "min=0");
	//// TwAddVarRW(DUC->g_pTweakBar, "Grid Height", TW_TYPE_UINT16, &m_gridHeight, "min=0");
	
	//TwAddVarRW(DUC->g_pTweakBar, "Particle Spacing", TW_TYPE_UINT16, &m_sphereSpacing, "min=1");
	//TwAddVarRW(DUC->g_pTweakBar, "Alpha", TW_TYPE_FLOAT, &alpha, "min=0 step=0.05");
	//TwAddVarCB(DUC->g_pTweakBar, "Grid Width", TW_TYPE_UINT16, setGridWidthCallback, getGridWidthCallback, nullptr, "min=0");
	//TwAddVarCB(DUC->g_pTweakBar, "Grid Height", TW_TYPE_UINT16, setGridHeightCallback, getGridHeightCallback, nullptr, "min=0");
}



float DiffusionSimulator::computeNewExplicitU(int x, int y, float timeStep) {
	// TODO: Use global system state
	
	float top = T->get(x, y - 1);
	float down = T->get(x, y + 1);
	float center = T->get(x, y);
	float left = T->get(x - 1, y);
	float right = T->get(x + 1, y);


	// Asuming deltaX = deltaY = 1
	// Simplified from class slides
	float firstTerm = alpha * timeStep * (
		right + left + top + down - 4.0 * center
		);
	return firstTerm + center;
}



Grid* DiffusionSimulator::diffuseTemperatureExplicit(float timeStep) {//add your own parameters
	// TODO: Use m_systemState
																	  
   /*for (int y = 1; y < m_gridHeight - 1; y++)
	{
		for (int x = 1; x < m_gridWidth - 1; x++)
		{
			float val = computeNewExplicitU(x, y, timeStep);
			newT->set(x, y, val);
		}
	}

	return newT;*/
	return nullptr;
}

void DiffusionSimulator::setupB(std::vector<Real>& b) {
	// TODO: use m_systemState

	// Just flatten the vector
	/*for (int i = 0; i < m_gridWidth * m_gridHeight; i++) {
		b.at(i) = T->get(i % m_gridWidth, i / m_gridWidth);
	}*/
}

void DiffusionSimulator::fillT(vector<Real>& x_vector) {//add your own parameters
	// TODO: Fill T in m_systemState

	//for (int i = 0; i < m_gridWidth * m_gridHeight; i++){
	//	int x = i % m_gridWidth;
	//	int y = i / m_gridWidth;

	//	float val = x_vector[i];

	//	// If it is boundary we fix it to zero
	//	if (x == 0 || x == m_gridWidth - 1 || y == 0 || y == m_gridHeight - 1) val = 0;
	//	newT->set(i % m_gridWidth, i / m_gridWidth, val);
	//}
}

void DiffusionSimulator::setupA(SparseMatrix<Real>& A, double factor) {//add your own parameters
	// to be implemented
	//setup Matrix A[sizeX*sizeY*sizeZ, sizeX*sizeY*sizeZ]
	// set with:  A.set_element( index1, index2 , value );
	// if needed, read with: A(index1, index2);
	// avoid zero rows in A -> set the diagonal value for boundary cells to 1.0
	//for (int i = 0; i < m_gridWidth * m_gridHeight; i++) {
	//	int x = i % m_gridWidth;
	//	int y = i / m_gridWidth;

	//	//cout << x << "," << y << endl;
	//	
	//	// Set diagonal of boundary cells to 1 
	//	if (x == 0 || x == m_gridWidth - 1 || y == 0 || y == m_gridHeight - 1) {
	//		A.set_element(i, i, 5.0);
	//		//cout << T->get(x, y) << endl;
	//		continue;
	//	}

	//	A.set_element(i, i, 4 * factor + 1);
	//	A.set_element(i + 1, i, -factor);
	//	A.set_element(i - 1, i, -factor);
	//	A.set_element(i + m_gridWidth, i, -factor);
	//	A.set_element(i - m_gridWidth, i, -factor);
	//}
}


void DiffusionSimulator::diffuseTemperatureImplicit(float timeStep) {//add your own parameters
	// solve A T = b
	// to be implemented
	//const int N = m_gridWidth * m_gridHeight;
	//SparseMatrix<Real> *A = new SparseMatrix<Real> (N);
	//std::vector<Real> *b = new std::vector<Real>(N);

	//float factor = timeStep * alpha;

	//setupA(*A, timeStep);
	//setupB(*b);

	//// perform solve
	//Real pcg_target_residual = 1e-05;
	//Real pcg_max_iterations = 1000;
	//Real ret_pcg_residual = 1e10;
	//int  ret_pcg_iterations = -1;

	//SparsePCGSolver<Real> solver;
	//solver.set_solver_parameters(pcg_target_residual, pcg_max_iterations, 0.97, 0.25);

	//std::vector<Real> x(N);
	//for (int j = 0; j < N; ++j) { x[j] = 0.; }

	//// preconditioners: 0 off, 1 diagonal, 2 incomplete cholesky
	//solver.solve(*A, *b, x, ret_pcg_residual, ret_pcg_iterations, 0);
	//// x contains the new temperature values
	//fillT(x);//copy x to T
	//delete A;
	//delete b;
}

void DiffusionSimulator::simulateTimestep(float timeStep)
{
	// to be implemented
	// update current setup for each frame
	switch (m_selectedSolver)
	{
	case EXPLICIT_SOLVER:
	{
		diffuseTemperatureExplicit(timeStep);
		break;
	}
	case IMPLICIT_SOLVER:
		diffuseTemperatureImplicit(timeStep);
		break;
	}


	// TODO: Replace this switches with the global simulation state object
	// 
	//Grid* tmp = newT;
	//newT = T;
	//T = tmp;
}



void DiffusionSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext)
{
	// This simulator should not draw, thats a task of CoupledSimulator
}

void DiffusionSimulator::onClick(int x, int y)
{
	m_trackmouse.x = x;
	m_trackmouse.y = y;
	//cout << *T << "\n\n";
}

void DiffusionSimulator::onMouse(int x, int y)
{
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}
