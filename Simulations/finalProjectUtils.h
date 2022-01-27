#pragma once

#include "vectorbase.h";

using namespace std;
using namespace GamePhysics;

/*
	A set of miscellaneous functions that will help our code look cleaner!
*/


float computeDistance(Vec3 v1, Vec3 v2) {
	Vec3 diff = v1 - v2;
	return norm(diff);
}