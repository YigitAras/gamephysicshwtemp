#pragma once

#include <vector>
#include "vectorbase.h";
#include "util/vectorbase.h";

using namespace GamePhysics;
using namespace std;

/*
This class represents the cloth state and also holds all necesessary aux members
needed to compute the computations.
*/

typedef struct MassPointStruct
{
	Vec3 pos;
	Vec3 vel;
	Vec3 accumForces;
	bool fixed;

	// Midpoint aux members
	Vec3 posTmp;
	Vec3 velTmp;
	Vec3 posBak;
	Vec3 velBak;
	Vec3 externalForceQueue;

} MassPoint;

typedef struct SpringStruct
{
	unsigned int p1;
	unsigned int p2;
	float initialLength;
	float currentLength;
} Spring;


class SimulationState
{
public:
	SimulationState();

	Spring m_springs[500];
	unsigned int m_totalSprings;

	MassPoint m_massPoints[500];
	unsigned int m_totalMassPoints;

	MassPoint* getMassPoint(unsigned int idx);

	int addMassPoint(Vec3 position, Vec3 Velocity, bool isFixed);
	void addSpring(int masspoint1, int masspoint2, float initialLength);
};

