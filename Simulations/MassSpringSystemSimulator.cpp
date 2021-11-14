#include "MassSpringSystemSimulator.h"



//Construtors

MassSpringSystemSimulator::MassSpringSystemSimulator(){

	m_iTestCase = 0;
	time = NULL;
	float* t = (float*)&Vec3(0., 0., 0.)[0];
	box=false;
	m_iIntegrator = 0;
	m_fMass = 10;
	m_fStiffness = 40;

	gravity = 0;

}



//UI


const char* MassSpringSystemSimulator::getTestCasesStr(){
	return "No Demo,Demo 1,Demo 2,Demo 3,Demo 4 and 5";

}

void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass* DUC){
    
    this->DUC = DUC;

}

void MassSpringSystemSimulator::reset(){
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;

}

void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext)
{

	
	DUC->beginLine();
	for(int i = 0; i < springs.size(); i++){
		
		DUC->drawLine(masspoints[springs[i].masspoints[0]].position,
				 Vec3(100,100,50),
				 masspoints[springs[i].masspoints[1]].position,
				 Vec3(100,100,50)
				);
		

	}
	DUC->endLine();


	float m_fSphereSize = 0.05;
	for(int i = 0; i< masspoints.size();i++){

		DUC->setUpLighting(Vec3(),0.4*Vec3(1,1,1),100,0.6* masspoints[i].velocity);
		DUC->drawSphere(masspoints[i].position,Vec3(m_fSphereSize, m_fSphereSize, m_fSphereSize));

	}

	

}

void MassSpringSystemSimulator::notifyCaseChanged(int testCase){

	m_iTestCase = testCase;

	
	switch(testCase){
		case 0:

			this->springs.clear();
			this->masspoints.clear();
			break;

		case 1:

			
			this->springs.clear();
			this->masspoints.clear();
			this->addMassPoint(Vec3(0,0,0),Vec3(-1,0,0),false);
			this->addMassPoint(Vec3(0,2,0),Vec3(1,0,0),false);
			this->addSpring(0, 1, 1);

			m_fDamping = 0;
			m_fStiffness = 40;
			m_fMass = 10;
			print = true;
			
			if(time!=NULL)
			*time = 0.1;
			break;

		case 2:

			this->springs.clear();
			this->masspoints.clear();
			this->addMassPoint(Vec3(0,0,0),Vec3(-1,0,0),false);
			this->addMassPoint(Vec3(0,2,0),Vec3(1,0,0),false);
			this->addSpring(0, 1, 1);

			m_fDamping = 0;
			m_fStiffness = 40;
			m_fMass = 10;
			if(time !=NULL)
				*time = 0.005;
			print = false;
			break;
		
		case 3:

			this->springs.clear();
			this->masspoints.clear();
			this->addMassPoint(Vec3(0,0,0),Vec3(-1,0,0),false);
			this->addMassPoint(Vec3(0,2,0),Vec3(1,0,0),false);
			this->addSpring(0, 1, 1);

			m_fDamping = 0;
			m_fStiffness = 40;
			m_fMass = 10;
			if(time!=NULL)
				*time = 0.005;
			print = false;
			break;

		case 4:
			//TwAddVarRW(DUC->g_pTweakBar,"Method",TW_TYPE_INT32,&m_iIntegrator,"min=0 max=3");
			TwAddVarRW(DUC->g_pTweakBar,"Gravity",TW_TYPE_FLOAT,&gravity,"step=0.01");
			TwAddVarRW(DUC->g_pTweakBar, "Damping", TW_TYPE_FLOAT, &m_fDamping, "step=0.01");
			TwAddVarRW(DUC->g_pTweakBar, "Stiffness", TW_TYPE_FLOAT, &m_fStiffness, "step=0.01 min=1");
			TwType TW_TYPE_METHOD = TwDefineEnumFromString("Method", "Euler,Leap-Frog,Midpoint");
			TwAddVarRW(DUC->g_pTweakBar, "Method", TW_TYPE_METHOD, &m_iIntegrator, "");
			
			std::mt19937 eng;
			std::uniform_real_distribution<float> randPos(-0.5f, 0.5f);

			this->springs.clear();
			this->masspoints.clear();
			this->addMassPoint(Vec3(-0.5,0,1),Vec3(0,0,0),false);
			this->addMassPoint(Vec3(randPos(eng),randPos(eng),randPos(eng)),Vec3(0,0,0),false);
			this->addMassPoint(Vec3(randPos(eng),randPos(eng),randPos(eng)),Vec3(0,0,0),false);
			this->addMassPoint(Vec3(randPos(eng),randPos(eng),randPos(eng)),Vec3(0,0,0),false);
			this->addMassPoint(Vec3(randPos(eng),randPos(eng),randPos(eng)),Vec3(0,0,0),false);
			this->addMassPoint(Vec3(randPos(eng),randPos(eng),randPos(eng)),Vec3(0,0,0),false);
			this->addMassPoint(Vec3(randPos(eng),randPos(eng),randPos(eng)),Vec3(0,0,0),false);
			this->addMassPoint(Vec3(randPos(eng),randPos(eng),randPos(eng)),Vec3(0,0,0),false);
			this->addMassPoint(Vec3(randPos(eng),randPos(eng),randPos(eng)),Vec3(0,0,0),false);
			this->addMassPoint(Vec3(randPos(eng),randPos(eng),randPos(eng)),Vec3(0,0,0),false);
			

			this->addSpring(0,1,1);
			this->addSpring(1,2,1);
			this->addSpring(2,3,1);
			this->addSpring(3,4,1);
			this->addSpring(4,1,1);
			this->addSpring(5,1,1);
			this->addSpring(6,5,1);
			this->addSpring(7,5,1);
			this->addSpring(7,8,1);
			this->addSpring(8,9,1);
			
			gravity = 0;
			m_fDamping = 0;
			m_iIntegrator = 0;
			m_fStiffness = 40;
			*time = 0.001;


	}

}

void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed) {}

void MassSpringSystemSimulator::simulateTimestep(float timeStep){

	switch (this->m_iTestCase) {
		
		case 1: 
			single_euler(timeStep,false,print);
			single_midpoint(timeStep,false,print);
			print=false;
			break;
		
		case 2:
			euler(timeStep);
			break;

		case 3:
			midpoint(timeStep);
			break;

		case 4:
			if (m_iIntegrator == 0)
				euler(timeStep);
			else if (m_iIntegrator == 1)
				leapfrog(timeStep);
			else if(m_iIntegrator == 2)
				midpoint(timeStep);
			break;

		default:
			if (m_iIntegrator == 0)
				euler(timeStep);
			else if (m_iIntegrator == 2)
				midpoint(timeStep);
			else
				leapfrog(timeStep);

	}



}

void MassSpringSystemSimulator::onClick(int x, int y){
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void MassSpringSystemSimulator::onMouse(int x, int y){
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

//



void MassSpringSystemSimulator::setMass(float mass){

	this->m_fMass = mass;

}

void MassSpringSystemSimulator::setStiffness(float stiffness){

	this->m_fStiffness = stiffness;

}

void MassSpringSystemSimulator::setDampingFactor(float damping){

	this->m_fDamping = damping;

}

int MassSpringSystemSimulator::addMassPoint(Vec3 position, Vec3 Velocity, bool isFixed){
	struct MASSPOINT new_masspoint;
	new_masspoint.position = position;
	new_masspoint.velocity = Velocity;
	new_masspoint.isFixed = isFixed;
	this->masspoints.push_back(new_masspoint);
	return this->masspoints.size() - 1;
}

void MassSpringSystemSimulator::addSpring(int masspoint1, int masspoint2, float initialLength){
	struct SPRING new_spring;
	new_spring.masspoints[0] = masspoint1;
	new_spring.masspoints[1] = masspoint2;
	new_spring.length = initialLength;
	this->springs.push_back(new_spring);
}

int MassSpringSystemSimulator::getNumberOfMassPoints(){
	return this->masspoints.size();
}

int MassSpringSystemSimulator::getNumberOfSprings(){
	return this->springs.size();
}

Vec3 MassSpringSystemSimulator::getPositionOfMassPoint(int index){
	return this->masspoints[index].position;
}

Vec3 MassSpringSystemSimulator::getVelocityOfMassPoint(int index){
	return this->masspoints[index].velocity;
	
}

void MassSpringSystemSimulator::applyExternalForce(Vec3 force){
	this->m_externalForce = force;
}



void MassSpringSystemSimulator::single_euler(float timeStep, bool change, bool p){

	for(int i = 0; i<springs.size(); i++){

		masspoint* m1 = &masspoints[springs[i].masspoints[0]];
		masspoint* m2 = &masspoints[springs[i].masspoints[1]];

		Vec3 d = m1->position - m2->position;

		float d_norm = norm(d);

		d/=d_norm;
	

		Vec3 acc1 = (-m_fStiffness * (d_norm - springs[i].length) * d ) / m_fMass;
		Vec3 acc2 = (-m_fStiffness * (d_norm - springs[i].length) * -d ) / m_fMass;

		if(change){
			m1->position = m1->position + timeStep * m1->velocity;
			m2->position = m2->position + timeStep * m2->velocity;
			m1->velocity = m1->velocity + timeStep * acc1;
			m2->velocity = m2->velocity + timeStep * acc2;
		}

		if(p){
			cout << "Euler:" << endl \
			     << "p1: " << (m1->position + timeStep * m1->velocity) << endl \
				 << "p2: " << (m2->position + timeStep * m2->velocity) << endl \
			     << "v1: " << (m1->velocity + timeStep * acc1) << endl \
				 << "v2: " << (m2->velocity + timeStep * acc2) << endl;
		}
	}


}


void MassSpringSystemSimulator::single_midpoint(float timeStep, bool change, bool p){


	for(int i = 0; i<springs.size(); i++){

		masspoint* m1 = &masspoints[springs[i].masspoints[0]];
		masspoint* m2 = &masspoints[springs[i].masspoints[1]];


		//Midpoint step
		Vec3 d = m1->position - m2->position;

		float d_norm = norm(d);

		d/=d_norm;
	

		Vec3 acc1_old = (-m_fStiffness * (d_norm - springs[i].length) * d ) / m_fMass;
		Vec3 acc2_old = (-m_fStiffness * (d_norm - springs[i].length) * -d ) / m_fMass;

		Vec3 pos1_mid = m1->position + 0.5 * timeStep * m1->velocity; 
		Vec3 pos2_mid = m2->position + 0.5 * timeStep * m2->velocity;

		Vec3 vel1_mid = m1->velocity + 0.5 * timeStep * acc1_old; 
		Vec3 vel2_mid = m2->velocity + 0.5 * timeStep * acc2_old; 

		//Endstep

		d = pos1_mid - pos2_mid;

		d_norm = norm(d);
		d/=d_norm;

		Vec3 acc1_mid = (-m_fStiffness * (d_norm - springs[i].length) * d ) / m_fMass;
		Vec3 acc2_mid = (-m_fStiffness * (d_norm - springs[i].length) * -d ) / m_fMass;

		if(change){
			m1->position = m1->position + timeStep * vel1_mid;
			m2->position = m2->position + timeStep * vel2_mid;
			m1->velocity = m1->velocity + timeStep * acc1_mid;
			m2->velocity = m2->velocity + timeStep * acc2_mid;
		}

		if(p){
			cout << "Midpoint:" << endl \
			     << "p1: " << (m1->position + timeStep * vel1_mid) << endl \
			     << "p2: " << (m2->position + timeStep * vel2_mid) << endl \
			     << "v1: " << (m1->velocity + timeStep * acc1_mid) << endl \
				 << "v2: " << (m2->velocity + timeStep * acc2_mid) << endl;
		}
		

				
	}
}



void MassSpringSystemSimulator::setTime(float* timestep){
	this->time = timestep;
}

void MassSpringSystemSimulator::euler(float timestep) {

	for (int i = 0; i < masspoints.size(); i++) {
		masspoints[i].force = Vec3(0, -gravity, 0) - m_fDamping * masspoints[i].velocity;

	}

	for(int i = 0; i<springs.size(); i++){

		masspoint* m1 = &masspoints[springs[i].masspoints[0]];
		masspoint* m2 = &masspoints[springs[i].masspoints[1]];

		Vec3 d = m1->position - m2->position;

		float d_norm = norm(d);

		d/=d_norm;
	
		m1->force += (-m_fStiffness * (d_norm - springs[i].length) * d ); 
		m2->force += (-m_fStiffness * (d_norm - springs[i].length) * -d ); 

	}
		
	for(int i = 0; i < masspoints.size(); i++){
		
		

		masspoint* m = &masspoints[i];
		
		m->position += timestep * m->velocity;
		if(m->position[1] < -1)
			m->position[1] = -1;
		m->velocity += timestep * (m->force/m_fMass);

	}

	




}


void MassSpringSystemSimulator::midpoint(float timeStep){

	masspoint* m1;
	masspoint* m2;


	//Forces old position

	for (int i = 0; i < masspoints.size(); i++) {
		masspoints[i].force = Vec3(0, -gravity, 0) - m_fDamping * masspoints[i].velocity;
		masspoints[i].a_p = false;
	}

	for(int i = 0; i<springs.size(); i++){

		m1 = &masspoints[springs[i].masspoints[0]];
		m2 = &masspoints[springs[i].masspoints[1]];

		Vec3 d = m1->position - m2->position;

		float d_norm = norm(d);

		d/=d_norm;
	
		m1->force += (-m_fStiffness * (d_norm - springs[i].length) * d ); 
		m2->force += (-m_fStiffness * (d_norm - springs[i].length) * -d ); 

	}


	for(int i = 0; i<springs.size(); i++){

		
		m1 = &masspoints[springs[i].masspoints[0]];
		m2 = &masspoints[springs[i].masspoints[1]];


		// position/velocity m1 and reset forces
		if(!m1->a_p){
			m1->mid_position = m1->position + 0.5 * timeStep * m1->velocity;
			m1->mid_velocity = m1->velocity + 0.5 * timeStep * (m1->force/m_fMass);
			m1->mid_force = Vec3(0, gravity, 0) - m_fDamping * m1->mid_velocity;
			m1->a_p = true;
		}
		//Midstep position/veloctiy m2 and reset forces
		if(!m2->a_p){
			m2->mid_position = m2->position + 0.5 * timeStep * m2->velocity;
			m2->mid_velocity = m2->velocity + 0.5 * timeStep * (m2->force/m_fMass);
			m2->mid_force = Vec3(0, gravity, 0) - m_fDamping * m2->mid_velocity;
			m2->a_p = true;
		}

		//Force Midstep
		Vec3 d = m1->mid_position - m2->mid_position;

		float d_norm = norm(d);

		d/=d_norm;
	
		m1->mid_force += (-m_fStiffness * (d_norm - springs[i].length) * d ); 
		m2->mid_force += (-m_fStiffness * (d_norm - springs[i].length) * -d ); 


	}

	//New Velocity


	for(int i = 0; i<masspoints.size(); i++){

		m1 = &masspoints[i];
		m1->velocity += timeStep * (m1->mid_force/m_fMass);
		
		m1->position = m1->position + timeStep * m1->mid_velocity;



		if(m1->position[1]<-1)
			m1->position[1] = -1;



	}


}



void MassSpringSystemSimulator::leapfrog(float timeStep) {

	for (int i = 0; i < masspoints.size(); i++) {
		masspoints[i].force = Vec3(0, -gravity, 0) - m_fDamping * masspoints[i].velocity;

	}

	for (int i = 0; i < springs.size(); i++) {

		masspoint* m1 = &masspoints[springs[i].masspoints[0]];
		masspoint* m2 = &masspoints[springs[i].masspoints[1]];

		Vec3 d = m1->position - m2->position;

		float d_norm = norm(d);

		d /= d_norm;

		m1->force += (-m_fStiffness * (d_norm - springs[i].length) * d);
		m2->force += (-m_fStiffness * (d_norm - springs[i].length) * -d);

	}

	for (int i = 0; i < masspoints.size(); i++) {

		masspoint* m = &masspoints[i];
		m->velocity += timeStep * (m->force / m_fMass);
		
		m->position += timeStep * m->velocity;
		if (m->position[1] < -1)
			m->position[1] = -1;
		
	}


}





