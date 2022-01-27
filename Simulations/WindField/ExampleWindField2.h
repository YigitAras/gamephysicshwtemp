#include "WindField.h"

class ExampleWindField2 : public WindField {
public:

	// Override this, set name and add obstacles
	ExampleWindField2() {
		setWindFieldName("Example Wind2");
		setIntensity(1.0);
		setEnabled(true);
	};

	// Draw a 3D grid of pyramids pointing in the direction of wind
	void drawWindField(float time, DrawingUtilitiesClass* DUC) {};

	Vec3 getForceAtPointTime(Vec3 point, float time) { return getNormalized(point) * cos(time); };
};