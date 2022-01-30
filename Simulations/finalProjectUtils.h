#pragma once

#include "vectorbase.h";

using namespace std;
using namespace GamePhysics;

/*
	A set of miscellaneous functions that will help our code look cleaner!
*/


static inline float computeDistance(Vec3 v1, Vec3 v2) {
	Vec3 diff = v1 - v2;
	return norm(diff);
}

static inline void setUpLightingColor(DrawingUtilitiesClass* DUC,float temp) {
	float specMult = 5;
	float f = fabsf(temp);
	f /= 1000.0;
	Vec3 l(0, 0, 0);
	if (temp < 0) {
		l[2] = f;
	}
	else {
		l[0] = f;
	}
	DUC->setUpLighting(l, specMult * Vec3(1, 1, 1), 70, 0.015 * Vec3(1, 1, 1));

}