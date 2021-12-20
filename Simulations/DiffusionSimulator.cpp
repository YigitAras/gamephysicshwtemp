#include "DiffusionSimulator.h"
#include "pcgsolver.h"
#include <cstdlib>
#include <ctime>
using namespace std;
bool positive = false;
int* Max_temp;
Grid::Grid(int x, int y) {
	grid = vector<vector<float>>(x, vector<float>(y));

}


float Grid::get(int x){
	return get(x / get_y(), x % get_y());

}

float Grid::get(int x, int y) {
	return (grid)[x][y];

}

int Grid::get_x() {
	return grid.size();
}
int Grid::get_y() {
	if(grid.size() > 0)
		return (grid)[0].size();
	return 0;
}

void Grid::set(int x, int y, float v) {
	(grid)[x][y] = v;


}




void Grid::reset(int x, int y) {
	grid = vector<vector<float>>(x, vector<float>(y));
}


void DiffusionSimulator::set_time_pointer(float* time){
	this->timestep = time;
}

DiffusionSimulator::DiffusionSimulator()
{
	m_iTestCase = 0;
	m_vfMovableObjectPos = Vec3();
	m_vfMovableObjectFinalPos = Vec3();
	m_vfRotate = Vec3();
	// to be implemented
	temp = 200;
	Max_temp = &temp;
	m = 10;
	n = 10;
	T = new Grid(m, n);
	alpha = 10;
	srand(time(NULL));
	init_grid();
	
}

void DiffusionSimulator::init_grid() {
	T->reset(m, n);
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			T->set(i, j, 0);
	
	for (int i = 1; i < m-1; i++)
		for (int j = 1; j < n - 1; j++) {
			if(!positive)
				T->set(i, j, rand() % temp - 100);
			else
				T->set(i, j, rand() % temp);
			
		}

}


const char * DiffusionSimulator::getTestCasesStr(){
	return "Explicit_solver, Implicit_solver";
}

void DiffusionSimulator::reset(){
		m_mouse.x = m_mouse.y = 0;
		m_trackmouse.x = m_trackmouse.y = 0;
		m_oldtrackmouse.x = m_oldtrackmouse.y = 0;


}

void TW_CALL DiffusionSimulator::get_value_x(void* value, void* clientData){
	*(int*)value = ((DiffusionSimulator*)clientData)->n;
}

void TW_CALL DiffusionSimulator::get_value_y(void* value, void* clientData) {
	*(int*)value = ((DiffusionSimulator*)clientData)->m;
}

void TW_CALL DiffusionSimulator::set_value_x(const void* value, void* clientData) {
	DiffusionSimulator* t = (DiffusionSimulator*)clientData;
	t->n = *(int*)value;
	t->init_grid();

}
void TW_CALL DiffusionSimulator::set_value_y(const void* value, void* clientData) {
	DiffusionSimulator* t = (DiffusionSimulator*)clientData;
	t->m = *(int*)value;
	t->init_grid();
}

void TW_CALL fill_corner(void *clientData){
	Grid* T = (Grid*)clientData;
	
	for (int i = 0; i < T->get_x(); i++)
		for (int j = 0; j < T->get_y(); j++)
			T->set(i, j, 0);
	
	for (int i = 0; i < T->get_x() / 2; i++)
		for (int j = 0; j < T->get_y()/2; j++) {
			T->set(i, j, *Max_temp);
		}

}

void DiffusionSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;
	TwAddVarCB(DUC->g_pTweakBar, "nx", TW_TYPE_INT32, set_value_x, get_value_x, this, "");
	TwAddVarCB(DUC->g_pTweakBar, "ny", TW_TYPE_INT32, set_value_y, get_value_y, this, "");

	TwAddVarRW(DUC->g_pTweakBar, "Negative values", TW_TYPE_BOOLCPP, &positive, "");
	TwAddVarRW(DUC->g_pTweakBar, "alpha", TW_TYPE_FLOAT, &alpha, "step=0.1");

	TwAddButton(DUC->g_pTweakBar, "Fill Corner", fill_corner , T, "");
	TwAddVarRW(DUC->g_pTweakBar, "Max Temp", TW_TYPE_INT32, &temp, "step = 1");
	// to be implemented
}

void DiffusionSimulator::notifyCaseChanged(int testCase)
{
	if (m_iTestCase == testCase) {
		init_grid();
	}
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

Grid* DiffusionSimulator::diffuseTemperatureExplicit() {//add your own parameters
	//Grid* newT = new Grid(T->get_x(),T->get_y());
	//Grid* newT = new Grid(1, 1);
	// to be implemented
	if (T == NULL)
		return NULL;
	Grid oT(*T);
	Grid* oldT = &oT;
	for(int i = 0; i<T->get_x();i++)
		for (int j = 0; j < T->get_y(); j++) {
			if (!(i == 0 || j == 0 || i >= T->get_x() - 1 || j >= T->get_y() - 1)) {
				float new_T = alpha * (*timestep) * (oldT->get(i + 1, j) - 4 * oldT->get(i, j) + oldT->get(i - 1, j) + oldT->get(i, j + 1) + oldT->get(i, j - 1)) + oldT->get(i, j);
				T->set(i, j, new_T);
			}
			else
				T->set(i, j, 0);


		}
	//make sure that the temperature in boundary cells stays zero
	return T;
}

void setupB(std::vector<Real>& b, Grid* T, float timestep) {//add your own parameters
	// to be implemented
	//set vector B[sizeX*sizeY]
	for (int i = 0; i < b.size(); i++) {
		b.at(i) =  T->get(i);
	}
}
int iter = 0;
void fillT(std::vector<Real>& x, Grid* T) {//add your own parameters
	// to be implemented
	//fill T with solved vector x
	int width = T->get_y();
	int length = T->get_x();
	int x_cord, y_cord;
	for (int i = 0; i < x.size(); i++) {
		x_cord = i / width;
		y_cord = i % width;

		if (!(x_cord == 0 || y_cord == 0 || x_cord == length - 1 || y_cord == width - 1)) {

			T->set(x_cord, y_cord, x[i]);
			
		}
		else
			T->set(x_cord, y_cord, 0);


	}


	//make sure that the temperature in boundary cells stays zero
}

void setupA(SparseMatrix<Real>& A, double factor, float timestep, int x_max, int y_max) {//add your own parameters
	// to be implemented
	//setup Matrix A[sizeX*sizeY*sizeZ, sizeX*sizeY*sizeZ]
	// set with:  A.set_element( index1, index2 , value );
	// if needed, read with: A(index1, index2);
	// avoid zero rows in A -> set the diagonal value for boundary cells to 1.0
	int x, y;
	for (int i = 0; i < A.n;i++) {
		x = i / y_max;
		y = i % y_max;
		if (!(x == 0 || y == 0 || x == x_max - 1 || y == y_max - 1)) {
			A.set_element(i, i, 4*timestep*factor +1);
			A.set_element(i, i + 1, -timestep*factor);
			A.set_element(i, i - 1, -timestep * factor);
			A.set_element(i, i - y_max, -timestep * factor);
			A.set_element(i, i + y_max, -timestep * factor);
			


		}
		else {
			A.set_element(i, i, 1);
		}



		}



}


void DiffusionSimulator::diffuseTemperatureImplicit() {//add your own parameters
	// solve A T = b
	// to be implemented
	const int N = n*m;//N = sizeX*sizeY*sizeZ
	SparseMatrix<Real> *A = new SparseMatrix<Real> (N);
	std::vector<Real> *b = new std::vector<Real>(N);

	setupA(*A, alpha,*timestep,n,m);
	setupB(*b,T,*timestep);

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
	fillT(x,T);//copy x to T
	delete A;
	delete b;
}



void DiffusionSimulator::simulateTimestep(float timeStep)
{
	// to be implemented
	// update current setup for each frame
	switch (m_iTestCase)
	{
	case 0:
		diffuseTemperatureExplicit();
		break;
	case 1:
		diffuseTemperatureImplicit();
		break;
	}
}

void DiffusionSimulator::drawObjects()
{
	// to be implemented
	//visualization
	
	float radx = 1.0 / m / 2.0;
	float d_x = 2.0 * radx;
	float rady = 1.0 / n / 2.0;
	float d_y = 2.0 * rady;
	
	for(float i = 0; i< m; i++)
		for (float j = 0; j < n; j++) {
			float value = T->get(i, j);
			

			DUC->setUpLighting(Vec3(value < 0 ? -value / 50 : value / 50, value < 0 ? 0 : value / 50, value < 0 ? 0 : value / 50), Vec3(), 0.1, Vec3());
			DUC->drawSphere(Vec3(i*d_x+radx-0.5,j*d_y+radx-0.5,0), radx);

		}
	

}


void DiffusionSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext)
{
	drawObjects();
}

void DiffusionSimulator::onClick(int x, int y)
{
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void DiffusionSimulator::onMouse(int x, int y)
{
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}
