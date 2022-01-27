#pragma once

#include "vectorbase.h";
using namespace std;

/*
This class represents the cloth state and also holds all necesessary aux members
needed to compute the computations.
*/

class SimulationState
{
public:
	SimulationState();

	// All fields necessary to represent and to work with
	// the simulator, eg: springs, masspoints, T, etc.
	// Essentially a commbination of original member attributes of 
	// MassSpringSystemSimulator and DiffussionSimulator, both
	// found in Rafael1 and Rafael3 branches
};