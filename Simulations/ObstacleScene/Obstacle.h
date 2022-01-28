#pragma once

#include "../util/vectorbase.h"
#include "../DrawingUtilitiesClass.h"

using namespace std;
using namespace GamePhysics;


// Base class for the two types of obstacles that will be available in the simulation: Spheres and Boxes
class Obstacle
{
public:

	void drawObstacle(DrawingUtilitiesClass* DUC);

private:
	// Define whatever is necessary, center, temperature, et.
};

class SphereObstacle : public Obstacle {




};