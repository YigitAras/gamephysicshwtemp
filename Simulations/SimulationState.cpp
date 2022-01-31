#include "SimulationState.h"
#include "finalProjectUtils.h"

SimulationState::SimulationState()
{
	cout << "Simulation state created" << endl;

	m_totalMassPoints = 0;
	m_totalSprings = 0;


	int rowMassCount = 10;
	int columnMassCount = 7;

	float posScale = 0.2;
	// Draw the cloth in the XY plane

	Vec3 basePosition = posScale * Vec3(-columnMassCount / 2.0, rowMassCount + 3, 0);
	// Vec3 baseVelocity = 0.1 * posScale * getNormalized(Vec3(1, 0, 0));


	int massCounter = 0;
	for (int row = 0; row < rowMassCount; row++)
	{
		for (int col = 0; col < columnMassCount; col++)
		{
			int idx = addMassPoint(basePosition + posScale * Vec3(col, 0, row), Vec3(0, 0, 0), row == 0);
			// Add spring to the left
			// if (col > 0) { addSpring(massCounter - 1, massCounter, posScale); }
			// Add spring upwards
			// if (row > 0) { addSpring(massCounter - columnMassCount, massCounter, posScale);
		//}
			// Add spring left and up
			// if (row > 0 && col > 0) addSpring(massCounter - columnMassCount - 1, massCounter, posScale * 1.4142);

			// if (col < columnMassCount - 1 && row > 0) addSpring(massCounter - columnMassCount + 1, massCounter, posScale * 1.4142);

			massCounter++;
		}
	}

	cout << "Finished setup" << endl;
}

int SimulationState::addMassPoint(Vec3 position, Vec3 Velocity, bool isFixed) {
	MassPoint mp;
	mp.vel = Velocity;
	mp.pos = position;
	mp.fixed = isFixed;
	m_massPoints[m_totalMassPoints++] = mp;
	return m_totalMassPoints; // We just return the index
}

void SimulationState::addSpring(int masspoint1, int masspoint2, float initialLength)
{
	Spring sp;
	sp.p1 = masspoint1;
	sp.p2 = masspoint2;
	sp.initialLength = initialLength;
	sp.currentLength = computeDistance(getMassPoint(sp.p1)->pos, getMassPoint(sp.p2)->pos);
	m_springs[m_totalSprings++] = sp;
}


MassPoint* SimulationState::getMassPoint(unsigned int idx) { return &m_massPoints[idx]; }