#pragma once

#include "../DrawingUtilitiesClass.h"
#include "../util/vectorbase.h"
#include "Obstacle.h"

using namespace std;
using namespace GamePhysics;

/*

Abstract class for representing one set of obstacles. There should be a couple of implementations
of this class so there are different obstacles scenes to show in the simulation

*/


class ObstacleScene {
public:


	// in constructor, set name and add obstacles

	// Draw every contained obstacle and color each one according to their temperature
	void drawObstacles(DrawingUtilitiesClass* DUC) { return;  };

	// Use tweakbar to show a GUI where each obstacle temperature can be adjusted.
	// Implement here
	void initUI(DrawingUtilitiesClass* DUC) { return; };

	string getSceneName() { return m_sceneName; };


	// Project a point that inside inside inside the volume of a shape, to the surface of the shape. 
	// Use the sortest projection. If the point isnt inside any shape of the scene, just return the same inputPoint
	
	// Quick check to see if the point is inside one shape of the scene. For boxes is trivial,	
	// for shperes first check in the boundig box and if it inside, do a distance check to the center, 
	// which is more costly

	// If inside of a shape perform the projection explained earlier. This way the cloth wont enter the shapes
	
	Vec3 clipToSurface(Vec3 inputPoint) { return Vec3(0, 0, 0); };
	
	// if point is in surface or inside an obstacle, return the obstacle temperture.
	// otherwise, return currTemp
	float getTemperatureAtPoint(Vec3 point, float currTemp) { return 0.0;  };
	
protected:

	string m_sceneName;
	vector<Obstacle> m_obstacles;
	// Call this on contructor of subclass. Name will appear in the coupledSimulator list
	void setSceneName(string name) { m_sceneName = name; }; 
	void addObstacle(Obstacle obs) { m_obstacles.push_back(obs); };
};