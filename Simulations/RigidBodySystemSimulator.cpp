#include "RigidBodySystemSimulator.h"
#include "collisionDetect.h"
bool b_demo1_step;
bool click = false;
	// Construtors
	RigidBodySystemSimulator::RigidBodySystemSimulator(){
		m_iTestCase = 10;
		gravity = Vec3();
		f.size = Vec3(1000, 1, 1000);
		
		f.pos = Vec3(0, -1.5, 0);
		f.mass = 1000000;

		f.R.initRotationXYZ(0,0,0);
		
		f.r = Quat(f.R);
		f.I = Mat4(0);
		f.I_current = f.I;
		
		f.scale.initScaling(f.size[0], f.size[1], f.size[2]);
		f.trans.initTranslation(f.pos[0], f.pos[1], f.pos[2]);
		f.Obj2WorldMatrix = f.scale * f.R * f.trans;
		scale = 0;

	}

	// Functions
	const char* RigidBodySystemSimulator::getTestCasesStr(){
		return "Demo 1,Demo 2,Demo 3,Demo 4";
	}
	void RigidBodySystemSimulator::initUI(DrawingUtilitiesClass* DUC) {
		this->DUC = DUC;
	}
	void RigidBodySystemSimulator::reset() {
		m_mouse.x = m_mouse.y = 0;
		m_trackmouse.x = m_trackmouse.y = 0;
		m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
	}
	void RigidBodySystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext) {
		DUC->setUpLighting(Vec3(0, 0, 0), 0.4 * Vec3(1, 1, 1), 2000.0, Vec3(0.5, 0.5, 0.5));
		for (int i = 0; i < getNumberOfRigidBodies(); i++) {
			rigid_body* b = &bodies[i];
			Mat4 Obj2WorldMatrix = b->scale * b->R * b->trans;
			DUC->drawRigidBody(Obj2WorldMatrix);
			
		}

		//DUC->drawRigidBody(f.Obj2WorldMatrix);
	}

	void TW_CALL RigidBodySystemSimulator::GetVec(void* value, void* clientData)
	{

		static_cast<float*> (value)[0] = static_cast<const Vec3*>(clientData)->x;
		static_cast<float*> (value)[1] = static_cast<const Vec3*>(clientData)->y;
		static_cast<float*> (value)[2] = static_cast<const Vec3*>(clientData)->z;

	}

	void TW_CALL RigidBodySystemSimulator::SetVec(const void* value, void* clientData)
	{
		static_cast<Vec3*> (clientData)->x = static_cast<const float*> (value)[0];
		static_cast<Vec3*> (clientData)->y = static_cast<const float*> (value)[1];
		static_cast<Vec3*> (clientData)->z = static_cast<const float*> (value)[2];
	}




	void RigidBodySystemSimulator::notifyCaseChanged(int testCase) {

		m_iTestCase = testCase;
		
		Quat r(Vec3(0.0f, 0.0f, 1.0f), (float)(M_PI) * 0.5f);
		Mat4 R;
		gravity = Vec3();
		switch (m_iTestCase) {
			
			case 0:
				bodies.clear();
				this->bodies.clear();
				addRigidBody(Vec3(0,0,0), Vec3(1,0.6,0.5), 2);
				//Mat4 r;
				//r.initRotationZ(90.0);
				
				setOrientationOf(0, r);
				applyForceOnBody(0, Vec3(0.3, 0.5, 0.25), Vec3(1, 1, 0));
				b_demo1_step = true;
				*time = 2;
				break;

			case 1:
				bodies.clear();
				this->bodies.clear();
				addRigidBody(Vec3(0, 0, 0), Vec3(1, 0.6, 0.5), 2);
				setOrientationOf(0, r);

				applyForceOnBody(0, Vec3(0.3, 0.5, 0.25), Vec3(1, 1, 0));
				
				*time = 0.01;
				break;

			case 2:
				bodies.clear();
				/*
				addRigidBody(Vec3(-0.3f, -0.2f, 0.1f), Vec3(0.4f, 0.2f, 0.2f), 1000);

				addRigidBody(Vec3(0.0f, 0.2f, 0.0f), Vec3(0.4f, 0.2f, 0.2f), 10.0);
				
				R.initRotationXYZ(0, 0, 0);
				setOrientationOf(1, Quat(R));
				
				setVelocityOf(1, Vec3(0.0f, -0.1, 0.0));
				setVelocityOf(0,Vec3(0.0f, 0.1, 0.0));
				
				*time = 0.01;
				*/
				
				*time = 0.001f;
				m_iTestCase = 2;
				addRigidBody(Vec3(-0.1f, -0.2f, 0.1f), Vec3(0.4f, 0.2f, 0.2f), 100.0f);
				addRigidBody(Vec3(0.0f, 0.2f, 0.0f), Vec3(0.4f, 0.2f, 0.2f), 100.0);
				setOrientationOf(1, Quat(Vec3(0.0f, 0.0f, 1.0f), (float)(M_PI) * 0.25f));
				setVelocityOf(1, Vec3(0.0f, -0.1f, 0.05f));


				break;

			case 3:

				bodies.clear();
				
				R.initRotationXYZ(0, 20, 20);
				addRigidBody(Vec3(-0.1f, -0.2f, 0.1f), Vec3(0.4f, 0.2f, 0.2f), 100.0f);
				setOrientationOf(0, Quat(R));

				R.initRotationXYZ(45, 0, 45);
				addRigidBody(Vec3(1.0f, 0.2f, 0.0f), Vec3(0.4f, 0.2f, 0.2f), 100.0);
				setOrientationOf(1, Quat(R));
				
				R.initRotationXYZ(45, 45, 45);
				addRigidBody(Vec3(0.5f, 1.2f, 0.0f), Vec3(0.4f, 0.2f, 0.2f), 100.0);
				setOrientationOf(2, Quat(R));

				R.initRotationXYZ(80, 60, 45);
				addRigidBody(Vec3(1.5f, 0.2f, 0.0f), Vec3(0.4f, 0.2f, 0.2f), 100.0);
				setOrientationOf(3, Quat(R));

				R.initRotationXYZ(45, 30, 45);
				addRigidBody(Vec3(2.0f, 0.2f, 1.0f), Vec3(0.4f, 0.2f, 0.2f), 100.0);
				setOrientationOf(4, Quat(R));

				addRigidBody(Vec3(-1.0f, 0.2f, 1.0f), Vec3(0.4f, 0.2f, 0.2f), 100.0);
				
				TwAddVarCB(DUC->g_pTweakBar, "Gravity", TW_TYPE_DIR3F, SetVec, GetVec, &gravity, "group='Sim Params' label='Gravity'");
				TwAddVarRW(DUC->g_pTweakBar, "Mouse Input", TW_TYPE_FLOAT, &scale, "step=0.01");
				for (int i = 0; i < bodies.size(); i++) {
					string s = "Velocity Body "+to_string(i);
					TwAddVarCB(DUC->g_pTweakBar, s.c_str(), TW_TYPE_DIR3F, SetVec, GetVec, &bodies[i].v, "group='Sim Params'");
					s = "Position Body " + to_string(i);
					TwAddVarCB(DUC->g_pTweakBar, s.c_str(), TW_TYPE_DIR3F, SetVec, GetVec, &bodies[i].pos, "group='Sim Params'");
				}
				TwAddVarRW(DUC->g_pTweakBar, "Ground Collision", TW_TYPE_BOOLCPP, &col,"");
				
				*time = 0.001;
				break;


			default:
				bodies.clear();
				addRigidBody(Vec3(-0.1f, -0.2f, 0.1f), Vec3(0.4f, 0.2f, 0.2f), 100.0f);

				addRigidBody(Vec3(0.0f, 0.2f, 0.0f), Vec3(0.4f, 0.2f, 0.2f), 100.0);
				setOrientationOf(1, Quat(Vec3(0.0f, 0.0f, 1.0f), (float)(M_PI) * 0.25f));
				applyForceOnBody(0, Vec3(0.0, 0.0f, 0.0), Vec3(0, 0, 200));
				setVelocityOf(1, Vec3(0.0f, -0.1f, 0.05f));
				*time = 0.1;

		}

	}
	void RigidBodySystemSimulator::externalForcesCalculations(float timeElapsed) {
	
		Point2D mouseDiff;
		mouseDiff.x = m_trackmouse.x - m_oldtrackmouse.x;
		mouseDiff.y = m_trackmouse.y - m_oldtrackmouse.y;
		
		if (mouseDiff.x != 0 || mouseDiff.y != 0)
		{
			
			m_oldtrackmouse.x = m_trackmouse.x;
			m_oldtrackmouse.y = m_trackmouse.y;
			Mat4 worldViewInv = Mat4(DUC->g_camera.GetWorldMatrix() * DUC->g_camera.GetViewMatrix());
			worldViewInv = worldViewInv.inverse();
			Vec3 inputView = Vec3((float)mouseDiff.x, (float)-mouseDiff.y, 0);
			Vec3 inputWorld = worldViewInv.transformVectorNormal(inputView);
			
			
			inputWorld = inputWorld * scale;
			m_forceMouse = inputWorld;

			for (int i = 0; i < bodies.size(); i++) {
				rigid_body* b = &bodies[i];
				b->force += m_forceMouse;
				
			}


		}
		else {
			
			m_forceMouse = Vec3{};
		}
	



	}
	
	void RigidBodySystemSimulator::simulateTimestep(float timeStep) {
		
		switch (this->m_iTestCase) {
		
			case 0:
				if (b_demo1_step) {
					rigid_body* b = &bodies[0];

					//External force (1,1,0) at (0.3,0.5,0.25)
					
					step(timeStep);
					b_demo1_step = false;
					cout << "Linear Velocity: " << b->v << endl;
					cout << "Angular Velocity: " << b->w << endl;
					cout << "Velocity at Point: " << cross(b->w, Vec3(-0.3, -0.5, -0.25)-b->pos) + b->v;
				}
				else {
					//step(timeStep);
				}
				break;

			case 1:
				step(timeStep);
				
				break;
			default:
				//cout << m_trackmouse.x << endl;
				step(timeStep);
				externalForcesCalculations(timeStep);

				//cout << "step";
		
		
		
		
		}
		
	
	}
	void RigidBodySystemSimulator::onClick(int x, int y) {
		m_oldtrackmouse.x = m_trackmouse.x;
		m_oldtrackmouse.y = m_trackmouse.y;
		m_trackmouse.x = x;
		m_trackmouse.y = y;
		click = true;
	}
	void RigidBodySystemSimulator::onMouse(int x, int y) {
		m_mouse.x = x;
		m_mouse.y = y;
		
	}



	void RigidBodySystemSimulator::setTime(float* timestep) {
		this->time = timestep;
	}


	// ExtraFunctions
	int RigidBodySystemSimulator::getNumberOfRigidBodies() {
		return this->bodies.size();
	}
	Vec3 RigidBodySystemSimulator::getPositionOfRigidBody(int i) {
		return bodies[i].pos;
	}
	Vec3 RigidBodySystemSimulator::getLinearVelocityOfRigidBody(int i) {
		return bodies[i].v;
	}
	Vec3 RigidBodySystemSimulator::getAngularVelocityOfRigidBody(int i) {
		return bodies[i].w;
	}
	void RigidBodySystemSimulator::applyForceOnBody(int i, Vec3 loc, Vec3 force) {
		rigid_body* b = &bodies[i];

		b->q = cross(loc-b->pos,force);
		b->force = force;
		//cout << "APPLYING FORCE " << b->q <<endl;

	
	}
	void RigidBodySystemSimulator::addRigidBody(Vec3 position, Vec3 size, int mass) {
		
		rigid_body b;
		b.pos = position;
		b.size = size;
		b.v = Vec3();
		//b.w = Vec3();
		b.mass = mass;
		b.L = Vec3();
		
		Mat4 I((mass / 12.0)*(size[1]*size[1]+size[2]*size[2]),0,0,0,0,
			   (mass / 12.0) * (size[0] * size[0] + size[2] * size[2]),0,0, 0, 0, 
			   (mass / 12.0) * (size[0] * size[0] + size[1] * size[1]),  0, 0, 0, 0, 1);

		b.w = I.inverse() * b.L;


		Mat4 R;
		R.initRotationZ(0);
		Quat q(R);

		b.I = I;
		b.I_current = I;
		b.R = R;
		b.r = q;
		
	
		Mat4 scale;
		scale.initScaling(size[0], size[1], size[2]);
		b.scale = scale;
		
		Mat4 trans;
		trans.initTranslation(position[0], position[1], position[2]);
		b.trans = trans;
		
		b.force = Vec3();
		b.w = Vec3();

		this->bodies.push_back(b);
		

	}
	void RigidBodySystemSimulator::setOrientationOf(int i, Quat orientation) {
		rigid_body* b = &bodies[i];
		b->r = orientation;
		
		b->R = b->r.getRotMat();
		Mat4 R_t = b->R;
		R_t.transpose();
		b->I_current = (b->R * b->I) * R_t;


	}
	void RigidBodySystemSimulator::setVelocityOf(int i, Vec3 velocity) {
		rigid_body* b = &bodies[i];
		b->v = velocity;
	}


	void RigidBodySystemSimulator::step(float timeStep) {

		rigid_body* b;
		rigid_body* b2;



		//Gravity
		for (int i = 0; i < bodies.size(); i++) {
			b = &bodies[i];
			b->force += gravity;
		}



		//Ground Collision
		if(col)
			for (int i = 0; i < bodies.size(); i++) {
				b = &bodies[i];
				Mat4 Obj2WorldMatrix_1 = b->scale * b->R * b->trans;
				CollisionInfo c = checkCollisionSAT(Obj2WorldMatrix_1, f.Obj2WorldMatrix);
				c = checkCollisionSAT(f.Obj2WorldMatrix, Obj2WorldMatrix_1);
				if (!c.isValid)
					continue;
				Vec3 v_rel =(cross(b->w, (c.collisionPointWorld - b->pos)) + b->v);//b->v;
			
			
				if (dot(v_rel, c.normalWorld) < 0){

					continue;
				}
				float J =(-(1+1)*dot(v_rel,c.normalWorld))/
					(1/b->mass + dot(cross(b->I_current.inverse().transformVector(cross(b->pos, c.normalWorld)), b->pos), c.normalWorld));
			
			

				cout <<"normal: "<< c.normalWorld << " n: " << c.collisionPointWorld << " J: " << J << " b->v: " << b->v << " dv: " << J * c.normalWorld / b->mass << endl;
				
				b->v[1] = 0;
				b->v = b->v  + J * c.normalWorld/b->mass ;
				b->L = b->L + cross(b->pos, J * c.collisionPointWorld);
			
			}


		for (int i = 0; i < bodies.size(); i++) {
			b = &bodies[i];
			b->pos = b->pos + timeStep * b->v;
			b->v = b->v + timeStep * b->force / b->mass;

			Quat w( b->w[0], b->w[1], b->w[2],0);

			b->r = b->r + timeStep / 2 * w * b->r;
			b->r = b->r.unit();
			
			b->R = b->r.getRotMat();
			Mat4 R_t = b->R;
			R_t.transpose();

			b->L = b->L + timeStep * b->q;
			b->I_current = (b->R * b->I) * R_t;

			b->w = b->I_current.inverse().transformVector(b->L);

			b->trans.initTranslation(b->pos[0], b->pos[1], b->pos[2]);
			
			
		}
		for (int i = 0; i < bodies.size(); i++) {
			b = &bodies[i];
			b->force = Vec3();
			b->q = Vec3();
		}

		for (int i = 0; i < bodies.size(); i++) {
			b = &bodies[i];
			Mat4 Obj2WorldMatrix_1 = b->scale * b->R * b->trans;
			for (int j = i + 1; j < bodies.size(); j++) {
				b2 = &bodies[j];
				Mat4 Obj2WorldMatrix_2 = b2->scale * b2->R * b2->trans;
				CollisionInfo c = checkCollisionSAT(Obj2WorldMatrix_1, Obj2WorldMatrix_2);
				if (!c.isValid)
					continue;
				Vec3 v_rel;
				v_rel = (cross(b->w, (c.collisionPointWorld - b->pos)) + b->v) - (cross(b2->w, (c.collisionPointWorld - b2->pos)) + b2->v);
				if (dot(v_rel, c.normalWorld) > 0) {
					continue;
				}
				float J = (-(1+1 ) * dot(v_rel, c.normalWorld)) /
					(1 / b->mass + 1 / b2->mass + dot(cross(b->I_current.inverse().transformVector(cross(b->pos, c.normalWorld)), b->pos) + cross(b2->I_current.inverse().transformVector(cross(b2->pos, c.normalWorld)), b2->pos), c.normalWorld));
				
				b->v =b->v + J * c.normalWorld / b->mass;
				b2->v = b2->v - J * c.normalWorld / b2->mass;
				
				b->L = b->L + cross(b->pos, J * (c.collisionPointWorld-b->pos));
				b2->L = b2->L - cross(b2->pos, J * (c.collisionPointWorld - b2->pos));
				
			}
		}

		
	}