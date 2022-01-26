#include "WindField.h"

class ExampleWindField: public WindField {
public:

	// Override this, set name and add obstacles
	ExampleWindField() {
		setWindFieldName("Example Wind");
	};

	// Draw a 3D grid of pyramids pointing in the direction of wind
	void drawWindField(float time, DrawingUtilitiesClass* DUC) {};

	Vec3 getForceAtPointTime(Vec3 point, float time) { return getNormalized(point) * cos(time); };

private:
	string m_windFieldName;
	float m_intensity;
	void setWindFieldName(string name) { m_windFieldName = name; };
};