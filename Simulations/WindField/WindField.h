#pragma once


using namespace std;
using namespace GamePhysics;


#pragma once

#include "../DrawingUtilitiesClass.h"
#include "../util/vectorbase.h"

using namespace std;
using namespace GamePhysics;

/*
Abstract class for representing a wind field. Set up a name and vectorial function to represent the field
*/


class WindField {
public:


	// Maybe set some name like turbulent, calm, storm, idk haha
	// Dont touch this
	void setIntensity(float intensity) { m_intensity = intensity; };
	float getIntensity() { return m_intensity; };

	void setEnabled(bool state) { m_enabled = state; };
	bool isEnabled() { return m_enabled;  }

	// Dont touch
	string getWindFieldName() { return m_windFieldName; };

	// Draw a 3D grid of pyramids pointing in the direction of wind
	void drawWindField(float time, DrawingUtilitiesClass* DUC) { return;  };


	// Implement here the wind vector field function. Ideally it should vary with the point and time.
	// Time is an absolute number from the start of the simulation, not a delta such as a timestep.
	// Important: should also depend on intensity
	Vec3 getForceAtPointTime(Vec3 point, float time) { return Vec3(0, 0, 0); };


protected:
	string m_windFieldName;
	// Call this on contructor of subclass. Name will appear in the coupledSimulator list
	float m_intensity;
	float m_enabled;
	void setWindFieldName(string name) { m_windFieldName = name; };
};