#include "DiffusionSimulator.h"
#include "pcgsolver.h"

using namespace std;

void Grid::createMatrix(int width, int height)
{
	m_width = width;
	m_height = height;
	matrix = new float* [height];

	for (int i = 0; i < m_height; i++)
	{
		matrix[i] = new float[width];
	}
}


void Grid::fillWith(float val) {
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			matrix[y][x] = val;
		}

	}
}

int Grid::getHeight() { return m_height; };
int Grid::getWidth() { return m_width; };


void  Grid::setRegionTo(int cornerX, int cornerY, int width, int height, float value) {
	if (cornerX + width - 1 >= m_width) return;
	if (cornerY + height - 1 >= m_height) return;

	for (int y = cornerY; y < cornerY + height; y++)
	{
		for (int x = cornerX; x < cornerX + width; x++)
		{
			matrix[y][x] = value;
		}
	}

}

std::ostream& operator<< (std::ostream& out, Grid const& grid) {
	for (int i = 0; i < grid.m_height; ++i)
	{
		for (int j = 0; j < grid.m_width; ++j)
		{
			out << grid.matrix[i][j] << ' ';
		}
		out << std::endl;
	}
	return out;
}


Grid::Grid(int width, int height) {
	createMatrix(width, height);
	fillWith(0.0);
}

Grid::Grid(int width, int height, float initialValue) 
{
	createMatrix(width, height);
	fillWith(initialValue);
}

void Grid::setBorderTo(float val) {
	// Simple and stupid border fill
	

	for (int x = 0; x < m_width; x++)
	{
		matrix[0][x] = val;
		matrix[m_height - 1][x] = val;
	}

	for (int y = 0; y < m_height; y++)
	{
		matrix[y][0] = val;
		matrix[y][m_width - 1] = val;
	}
}



DiffusionSimulator::DiffusionSimulator()
{
	m_iTestCase = 0;
	m_vfMovableObjectPos = Vec3();
	m_vfMovableObjectFinalPos = Vec3();
	m_vfRotate = Vec3();
	// to be implemented


	m_gridWidth = 20;
	m_gridHeight = 20;
	m_sphereRadius = 10;
	m_sphereSpacing = 8;
	T = new Grid(m_gridWidth, m_gridHeight, 0);

	// Initial condition

	T->setRegionTo(m_gridWidth / 2, m_gridHeight / 2, 2, 5, 100);

	//T->setRegionTo(m_gridWidth / 2 - 10, m_gridHeight / 2 - 10, 3, 3, 300);
	
	T->setRegionTo(0, m_gridHeight - 6, 5, 5, 700);


	prevT = new Grid(m_gridWidth, m_gridHeight, 0);
	cout << T->matrix[m_gridHeight - 1][m_gridWidth - 1] << endl;
}


int DiffusionSimulator::getSphereRadius() { return m_sphereRadius; };
void DiffusionSimulator::setSphereRadius(int radius) {
	if (radius <= 0) return;
	m_sphereRadius = radius;
};

int DiffusionSimulator::getSphereSpacing() { return m_sphereSpacing; };
void DiffusionSimulator::setSphereSpacing(int space) {
	if (space <= 0) return;
	m_sphereSpacing = space;
};


int DiffusionSimulator::getGridWidth() { return m_gridWidth; };
void DiffusionSimulator::setGridWidth(int width) {
	if (width <= 0) return;
	m_gridWidth = width;
};


int DiffusionSimulator::getGridHeight() { return m_gridHeight; };
void DiffusionSimulator::setGridHeight(int height) {
	if (height <= 0) return;
	m_gridHeight = height;
};


const char * DiffusionSimulator::getTestCasesStr(){
	return "Explicit_solver, Implicit_solver";
}

void DiffusionSimulator::reset(){
		m_mouse.x = m_mouse.y = 0;
		m_trackmouse.x = m_trackmouse.y = 0;
		m_oldtrackmouse.x = m_oldtrackmouse.y = 0;

}

void DiffusionSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;
	// to be implemented
}

void DiffusionSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;
	m_vfMovableObjectPos = Vec3(0, 0, 0);
	m_vfRotate = Vec3(0, 0, 0);
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
	float c = 0.4;
	
	float top = T->matrix[y - 1][x];
	float down = T->matrix[y + 1][x];
	float center = T->matrix[y][x];
	float left = T->matrix[y][x - 1];
	float right = T->matrix[y][x + 1];


	// Asuming deltaX = deltaY = 1
	// Simplified from class slides
	float firstTerm = pow(c, 2) * pow(timeStep, 2) * (
		right + left + top + down - 4.0 * center
		);
	return firstTerm + 2.0 * center - prevT->matrix[y][x];
}



Grid* DiffusionSimulator::diffuseTemperatureExplicit(float timeStep) {//add your own parameters
	// Create New matrix filled with zeroes
	Grid* newT = new Grid(m_gridWidth, m_gridHeight, 1);


	// Avoid boundaries as they should always be zero
	for (int y = 1; y < m_gridHeight - 1; y++)
	{
		for (int x = 1; x < m_gridWidth - 1; x++)
		{

			float val = computeNewExplicitU(x, y, timeStep);
			newT->matrix[y][x] = val;
		}
	}

	return newT;
}

void setupB(std::vector<Real>& b) {//add your own parameters
	// to be implemented
	//set vector B[sizeX*sizeY]
	for (int i = 0; i < 25; i++) {
		b.at(i) = 0;
	}
}

void fillT() {//add your own parameters
	// to be implemented
	//fill T with solved vector x
	//make sure that the temperature in boundary cells stays zero
}

void setupA(SparseMatrix<Real>& A, double factor) {//add your own parameters
	// to be implemented
	//setup Matrix A[sizeX*sizeY*sizeZ, sizeX*sizeY*sizeZ]
	// set with:  A.set_element( index1, index2 , value );
	// if needed, read with: A(index1, index2);
	// avoid zero rows in A -> set the diagonal value for boundary cells to 1.0
	for (int i = 0; i < 25; i++) {
			A.set_element(i, i, 1); // set diagonal
	}
}


void DiffusionSimulator::diffuseTemperatureImplicit() {//add your own parameters
	// solve A T = b
	// to be implemented
	const int N = 25;//N = sizeX*sizeY*sizeZ
	SparseMatrix<Real> *A = new SparseMatrix<Real> (N);
	std::vector<Real> *b = new std::vector<Real>(N);

	setupA(*A, 0.1);
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
	fillT();//copy x to T
}



void DiffusionSimulator::simulateTimestep(float timeStep)
{
	// to be implemented
	// update current setup for each frame
	switch (m_iTestCase)
	{
	case 0:
		prevT = T;
		//cout << *T << "\n\n";
		T = diffuseTemperatureExplicit(timeStep * 500);
		//cout << *T << "\n\n";

		//exit(0);
		break;
	case 1:
		diffuseTemperatureImplicit();
		break;
	}
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


	//cout << DUC->g_pTweakBar

	for (int y = 0; y < m_gridHeight; y++)
	{
		for (int x = 0; x < m_gridWidth; x++)
		{

			float input = T->matrix[y][x] / 10.0;
			// float input = x / (m_gridWidth - 1.0);
			DUC->setUpLighting(colorFunc(input), specMult * Vec3(1, 1, 1), specPower, diffMult * 0.05 * Vec3(1, 1, 1));
			DUC->drawSphere(Vec3(x * spaceScale, y * spaceScale, 0), Vec3(sphereScale, sphereScale, sphereScale));
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
	cout << *T << "\n\n";
}

void DiffusionSimulator::onMouse(int x, int y)
{
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}
