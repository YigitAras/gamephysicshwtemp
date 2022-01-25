#pragma once

#include "util/vectorbase.h";

using namespace std;
using namespace GamePhysics;

float computeDistance(Vec3 v1, Vec3 v2) {
	Vec3 diff = v1 - v2;
	return norm(diff);
}