#pragma once
#include <ostream>

//impement your own grid class for saving grid data

using namespace std;


constexpr int MAX_ARR_SIZE = 500;

class Grid {
public:
	// Construtors
	Grid(int width, int height);
	Grid(int width, int height, float initialValue);
	friend ostream& operator<< (std::ostream& out, Grid const& grid);

	float matrix[MAX_ARR_SIZE][MAX_ARR_SIZE];

	int getHeight();
	int getWidth();


	void setBorderTo(float val);
	void setBorderToInitial();
	void setRegionTo(int cornerX, int cornerY, int width, int height, float value);
	void fillWith(float val);

	float get(int x, int y);
	void set(int x, int y, float val);

	void resize(int newWidth, int newHeight);


private:
	// Attributes
	int m_width;
	int m_height;
	float m_initialValue;


	//void createMatrix(int width, int height);
	
};