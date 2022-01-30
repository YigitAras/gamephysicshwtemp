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
	void drawObstacles(DrawingUtilitiesClass* DUC) { 
		for (int i = 0; i < m_obstacles.size(); i++) {
			m_obstacles[i]->drawObstacle(DUC);
		}
	};

	// Use tweakbar to show a GUI where each obstacle temperature can be adjusted.
	// Implement here
	void initUI(DrawingUtilitiesClass* DUC) { 
		for (int i = 0; i < m_obstacles.size(); i++)
			m_obstacles[i]->initUI(DUC,i);
	
	};

	string getSceneName() { return m_sceneName; };


	// Project a point that inside inside inside the volume of a shape, to the surface of the shape. 
	// Use the sortest projection. If the point isnt inside any shape of the scene, just return the same inputPoint
	
	// Quick check to see if the point is inside one shape of the scene. For boxes is trivial,	
	// for shperes first check in the boundig box and if it inside, do a distance check to the center, 
	// which is more costly

	// If inside of a shape perform the projection explained earlier. This way the cloth wont enter the shapes
	
	Vec3 clipToSurface(Vec3 inputPoint) { 
		for (int i = 0; i < m_obstacles.size(); i++) {
			bool isinside = false;
			Vec3 new_point = m_obstacles[i]->isInsideorSurface(inputPoint, &isinside);
			if (isinside) {
				return new_point;
			}


		} 
		return inputPoint;
	};
	
	// if point is in surface or inside an obstacle, return the obstacle temperture.
	// otherwise, return currTemp
	float getTemperatureAtPoint(Vec3 point, float currTemp) { 
		for (int i = 0; i < m_obstacles.size(); i++) {
			bool isinside = false;
			Vec3 new_point = m_obstacles[i]->isInsideorSurface(point, &isinside);
			if (isinside) {
				return m_obstacles[i]->getTemp();
			}


		}
		return currTemp;
	
	};
	
protected:

	string m_sceneName;
	vector<Obstacle*> m_obstacles;
	// Call this on contructor of subclass. Name will appear in the coupledSimulator list
	void setSceneName(string name) { m_sceneName = name; }; 
	void addObstacle(Obstacle* obs) { m_obstacles.push_back(obs); };
};




class ObstacleScene1 : public ObstacleScene {
public:


	// in constructor, set name and add obstacles
	ObstacleScene1() {
		setSceneName("ObstacleScene 1");
		Obstacle* c = new ConeObstacle(Vec3(0.5, 0, 0.5), 1.25, 0.5, 1);
		addObstacle(c);
		c = new ConeObstacle(Vec3(-0.5, 0, 0.5), 1.25, 0.5, 1);
		addObstacle(c);
		c = new ConeObstacle(Vec3(0.5, 0, -0.5), 1.25, 0.5, 1);
		addObstacle(c);
		c = new ConeObstacle(Vec3(-0.5, 0, -0.5), 1.25, 0.5, 1);
		addObstacle(c);

	}



};


class ObstacleScene2 : public ObstacleScene {
public:
	ObstacleScene2() {
		setSceneName("ObstacleScene 2");
		Obstacle* o;
		o = new BoxObstacle(Vec3(0.5, 1.1, 0.25), Vec3(0.1, 0, 0.5), 1);
		addObstacle(o);
		o = new BoxObstacle(Vec3(0.2, 0.8, 0.25), Vec3(-0.1-0.5, 0, 0.5), 1);
		addObstacle(o);
		o = new BoxObstacle(Vec3(0.3, 1.5, 0.25), Vec3(0.1, 0, -1.5), 1);
		addObstacle(o);
		o = new BoxObstacle(Vec3(0.5, 1.1, 0.25), Vec3(-0.1-0.5, 0, -1), 1);
		addObstacle(o);
	}

};


class ObstacleScene3 : public ObstacleScene {
public:
	ObstacleScene3() {
		setSceneName("ObstacleScene 3");
		Obstacle* o;
		o = new CylinderObstacle(Vec3(0.6, 0, 0.7), 1.8, 0.4, 1);
		addObstacle(o);
		o = new CylinderObstacle(Vec3(-0.6, 0, 0.7), 1.3, 0.4, 1);
		addObstacle(o);
		o = new CylinderObstacle(Vec3(-0.6, 0, -0.7), 1.4, 0.5, 1);
		addObstacle(o);
		o = new CylinderObstacle(Vec3(0.6, 0, -0.7), 1, 0.6, 1);
		addObstacle(o);
	}

};


class ObstacleScene4 : public ObstacleScene {
public:
	ObstacleScene4() {
		setSceneName("ObstacleScene 4");
		Obstacle* o;
		o = new SphereObstacle(1, Vec3(0.5, 0.5, 0.5), 1);
		addObstacle(o);
		o = new SphereObstacle(0.9, Vec3(-0.5, 0, 0.5), 1);
		addObstacle(o);
		o = new SphereObstacle(0.5, Vec3(0.5, 1,-0.5), 1);
		addObstacle(o);
		o = new SphereObstacle(0.5, Vec3(-0.5, 1, -0.5), 1);
		addObstacle(o);
	}

};

class ObstacleScene5 : public ObstacleScene {
public:
	ObstacleScene5() {
		setSceneName("ObstacleScene 5");
		Obstacle* o;
		o = new ConeObstacle(Vec3(0.6, 0, 0.7), 1.8, 0.4, 1);
		addObstacle(o);
		o = new CylinderObstacle(Vec3(-0.6, 0, 0.7), 1.8, 0.4, 1);
		addObstacle(o);
		o = new SphereObstacle(0.5, Vec3(0.5, 1, -0.5), 1);
		addObstacle(o);
		o = new BoxObstacle(Vec3(0.5, 1.1, 0.25), Vec3(-0.1 - 0.5, 0, -1), 1);
		addObstacle(o);
	}

};