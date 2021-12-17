#include "DiffusionSimulator.h"

using namespace std;


DiffusionSimulator::DiffusionSimulator()
{
	m_iTestCase = 0;
	m_vfMovableObjectPos = Vec3(5, 5, 5);
	m_vfMovableObjectFinalPos = Vec3();
	m_vfRotate = Vec3();
	// to be implemented


	m_gridWidth = 50;
	m_gridHeight = 50;
	m_sphereRadius = 10;
	m_sphereSpacing = 8;
	alpha = 20;
	T = new Grid(m_gridWidth, m_gridHeight, 0);
	newT = new Grid(m_gridWidth, m_gridHeight, 0);
	gridInitialSetup();
}


void DiffusionSimulator::gridInitialSetup() {
	T->fillWith(0);
	newT->fillWith(0);
	T->setRegionTo(m_gridWidth / 2, m_gridHeight / 2, 2, 5, -100);
	T->setRegionTo(m_gridWidth / 2 - 10, m_gridHeight / 2 - 10, 3, 3, 300);
	T->setRegionTo(1, m_gridHeight - 6, 5, 5, 700);
	T->setBorderToInitial();
}


const char * DiffusionSimulator::getTestCasesStr(){
	return "Explicit_solver, Implicit_solver";
}

void DiffusionSimulator::reset(){
		m_mouse.x = m_mouse.y = 0;
		m_trackmouse.x = m_trackmouse.y = 0;
		m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
		gridInitialSetup();
}

void DiffusionSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;
	XMFLOAT3 eye(0.0f, 0.0f, -m_gridWidth / 2.0);
	XMFLOAT3 lookAt(0.0f, 0.0f, 0.0f);
	DUC->g_camera.SetViewParams(XMLoadFloat3(&eye), XMLoadFloat3(&lookAt));

	// Added by Rafa
	TwAddVarRW(DUC->g_pTweakBar, "Grid Width", TW_TYPE_UINT16, &m_gridWidth, "min=0");
	TwAddVarRW(DUC->g_pTweakBar, "Grid Height", TW_TYPE_UINT16, &m_gridHeight, "min=0");
	TwAddVarRW(DUC->g_pTweakBar, "Particle Radius", TW_TYPE_UINT16, &m_sphereRadius, "min=1");
	TwAddVarRW(DUC->g_pTweakBar, "Particle Spacing", TW_TYPE_UINT16, &m_sphereSpacing, "min=1");
	TwAddVarRW(DUC->g_pTweakBar, "Alpha", TW_TYPE_FLOAT, &alpha, "min=0 step=0.05");
	reset();
}

void DiffusionSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;
	//
	//to be implemented
	//
	switch (m_iTestCase)
	{
	case 0:
		cout << "Explicit solver!\n";
		break;
	case 1:
		cout << "Implicit solver!\n";
		break;
	default:
		cout << "Empty Test!\n";
		break;
	}
}



float DiffusionSimulator::computeNewExplicitU(int x, int y, float timeStep) {
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
	for (int y = 1; y < m_gridHeight - 1; y++)
	{
		for (int x = 1; x < m_gridWidth - 1; x++)
		{
			float val = computeNewExplicitU(x, y, timeStep);
			newT->set(x, y, val);
		}
	}

	return newT;
}

void DiffusionSimulator::setupB(std::vector<Real>& b) {//add your own parameters
	// Just flatten the vector
	for (int i = 0; i < m_gridWidth * m_gridHeight; i++) {
		b.at(i) = T->get(i % m_gridWidth, i / m_gridWidth);
	}
}

void DiffusionSimulator::fillT(vector<Real>& x_vector) {//add your own parameters
	// to be implemented
	//fill T with solved vector x
	//make sure that the temperature in boundary cells stays zero

	for (int i = 0; i < m_gridWidth * m_gridHeight; i++){
		int x = i % m_gridWidth;
		int y = i / m_gridWidth;

		float val = x_vector[i];

		// If it is boundary we fix it to zero
		if (x == 0 || x == m_gridWidth - 1 || y == 0 || y == m_gridHeight - 1) val = 0;
		newT->set(i % m_gridWidth, i / m_gridWidth, val);
	}
}

void DiffusionSimulator::setupA(SparseMatrix<Real>& A, double factor) {//add your own parameters
	// to be implemented
	//setup Matrix A[sizeX*sizeY*sizeZ, sizeX*sizeY*sizeZ]
	// set with:  A.set_element( index1, index2 , value );
	// if needed, read with: A(index1, index2);
	// avoid zero rows in A -> set the diagonal value for boundary cells to 1.0
	for (int i = 0; i < m_gridWidth * m_gridHeight; i++) {
		int x = i % m_gridWidth;
		int y = i / m_gridWidth;

		//cout << x << "," << y << endl;
		
		// Set diagonal of boundary cells to 1 
		if (x == 0 || x == m_gridWidth - 1 || y == 0 || y == m_gridHeight - 1) {
			A.set_element(i, i, 5.0);
			//cout << T->get(x, y) << endl;
			continue;
		}

		A.set_element(i, i, 4 * factor + 1);
		A.set_element(i + 1, i, -factor);
		A.set_element(i - 1, i, -factor);
		A.set_element(i + m_gridWidth, i, -factor);
		A.set_element(i - m_gridWidth, i, -factor);
	}
}


void DiffusionSimulator::diffuseTemperatureImplicit(float timeStep) {//add your own parameters
	// solve A T = b
	// to be implemented
	const int N = m_gridWidth * m_gridHeight;
	SparseMatrix<Real> *A = new SparseMatrix<Real> (N);
	std::vector<Real> *b = new std::vector<Real>(N);

	float factor = timeStep * alpha;

	setupA(*A, timeStep);
	setupB(*b);

	// perform solve
	Real pcg_target_residual = 1e-05;
	Real pcg_max_iterations = 1000;
	Real ret_pcg_residual = 1e10;
	int  ret_pcg_iterations = -1;

	SparsePCGSolver<Real> solver;
	solver.set_solver_parameters(pcg_target_residual, pcg_max_iterations, 0.97, 0.25);

	std::vector<Real> x(N);
	for (int j = 0; j < N; ++j) { x[j] = 0.; }

	// preconditioners: 0 off, 1 diagonal, 2 incomplete cholesky
	solver.solve(*A, *b, x, ret_pcg_residual, ret_pcg_iterations, 0);
	// x contains the new temperature values
	fillT(x);//copy x to T
}



void DiffusionSimulator::simulateTimestep(float timeStep)
{
	// to be implemented
	// update current setup for each frame
	switch (m_iTestCase)
	{
	case 0:
	{
		diffuseTemperatureExplicit(timeStep);
		break;
	}
	case 1:
		diffuseTemperatureImplicit(timeStep);
		break;
	}

	Grid* tmp = newT;
	newT = T;
	T = tmp;
}



Vec3 colorFunc(float x) {
	float y = pow(x, 2);
	return Vec3(y, 0, 1 - y);
}

void DiffusionSimulator::drawObjects(float emissiveMult, float specMult, float specPower, float diffMult)
{

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


void DiffusionSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext, float emissiveMult, float specMult, float specPower, float diffMult)
{
	drawObjects(emissiveMult, specMult, specPower, diffMult);
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
