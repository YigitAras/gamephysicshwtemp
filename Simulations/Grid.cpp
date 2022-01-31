#include "Grid.h"

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
	m_width = width;
	m_height = height;
	m_initialValue = 0.0;
	fillWith(m_initialValue);
}

Grid::Grid(int width, int height, float initialValue)
{
	m_width = width;
	m_height = height;
	m_initialValue = initialValue;
	fillWith(m_initialValue);
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

void Grid::setBorderToInitial() {
	setBorderTo(m_initialValue);
}

float Grid::get(int x, int y) {
	if (x >= m_width || y >= m_height) throw runtime_error("Out of range index");
	return matrix[y][x];
}

void Grid::set(int x, int y, float val) {
	if (x >= m_width || y >= m_height) throw runtime_error("Out of range index");
	matrix[y][x] = val;
}

void Grid::resize(int newWidth, int newHeight) {
	if (newWidth > m_width) {
		for (int y = 0; y < newHeight; y++)
		{
			for (int x = m_width; x < newWidth; x++)
			{
				matrix[y][x] = m_initialValue;
			}
		}
	}

	if (newHeight > m_height) {
		for (int y = m_height; y < newHeight; y++){
			for (int x = 0; x < newWidth; x++){
				matrix[y][x] = m_initialValue;
			}
		}
	}

	m_width = newWidth;
	m_height = newHeight;
}