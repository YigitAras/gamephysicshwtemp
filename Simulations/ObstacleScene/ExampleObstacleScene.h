
#include "ObstacleScene.h"

class ExampleObstacleScene: public ObstacleScene {
public:

	ExampleObstacleScene() {
		setSceneName("Example Scene");
		// addObstacle(...)
	};

	void drawObstacles(DrawingUtilitiesClass* DUC) {};

	void initUI(DrawingUtilitiesClass* DUC) {};

	string getSceneName() { return m_sceneName; };

	Vec3 clipToSurface(Vec3 inputPoint) { return Vec3(0, 0, 0); };

	float getTemperatureAtPoint(Vec3 point, float currTemp) { return 0; };


private:

	string m_sceneName;
	// Call this on contructor of subclass. Name will appear in the coupledSimulator list
	void setSceneName(string name) { m_sceneName = name; };
	void addObstacle(Obstacle obs);
	vector<Obstacle> m_obstacles;
};