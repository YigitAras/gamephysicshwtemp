#pragma once

#include "../util/vectorbase.h"
#include "../DrawingUtilitiesClass.h"

using namespace std;
using namespace GamePhysics;


// Base class for the two types of obstacles that will be available in the simulation: Spheres and Boxes
class Obstacle
{
public:
	
	virtual void drawObstacle(DrawingUtilitiesClass* DUC) { cout << name << endl; }
	virtual Vec3 isInsideorSurface(Vec3 position, bool* isinside){ return Vec3(0, 0, 0); }
	float getTemp() { return m_temp; }
	void initUI(DrawingUtilitiesClass* DUC){
		string tmp = name + " Temperature";
		TwAddVarRW(DUC->g_pTweakBar, tmp.c_str(), TW_TYPE_FLOAT, &m_temp, "");
	}
private:
	// Define whatever is necessary, center, temperature, etc.
protected:
	virtual bool checkboundingbox(Vec3 position) { return false; };


	string name;
	float m_temp;
};

class SphereObstacle : public Obstacle {

public:
	SphereObstacle(float d, Vec3 center, float temp) {
		m_spheredia = d;
		m_sphereRadius = d / 2;
		m_sphereCenter = center;
		m_temp = temp;
		name = "Sphere";
	}
	void drawObstacle(DrawingUtilitiesClass* DUC) override {
		
		DUC->drawSphere(m_sphereCenter, Vec3(m_sphereRadius, m_sphereRadius, m_sphereRadius));
	}

	Vec3 isInsideorSurface(Vec3 position, bool* isinside){
		if (checkboundingbox(position)) {
			float d = sqrtf(powf(position[0] - m_sphereCenter[0], 2) + powf(position[1] - m_sphereCenter[1], 2) + powf(position[2] - m_sphereCenter[2], 2));
			if (d <= m_sphereRadius) {
				*isinside = true;
				Vec3 tmp(position[0] - m_sphereCenter[0], position[1] - m_sphereCenter[1], position[2] - m_sphereCenter[2]);
				float dis = sqrtf(powf(tmp[0], 2) + powf(tmp[1], 2) + powf(tmp[2], 2));
				if (dis == m_sphereRadius)
					return position;

				tmp *= m_sphereRadius / dis;
				return (tmp+m_sphereCenter);

			}
		}
		*isinside = false;
		return position;
	}

private:
	float m_sphereRadius;
	Vec3 m_sphereCenter;
	float m_spheredia;
	
protected:
	bool checkboundingbox(Vec3 position) { 

		if (position[0] >= (m_sphereCenter[0]-m_sphereRadius) && position[0] <= (m_sphereCenter[0] + m_sphereRadius))
			if (position[1] >= (m_sphereCenter[1] - m_sphereRadius) && position[1] <= (m_sphereCenter[1] + m_sphereRadius))
				if (position[2] >= (m_sphereCenter[2] - m_sphereRadius) && position[2] <= (m_sphereCenter[2] + m_sphereRadius))
					return true;
		
		
		return false; 
	};

};

class BoxObstacle : public Obstacle {
public:
	void drawObstacle(DrawingUtilitiesClass* DUC) {
		Mat4f a;
		a.initScaling(m_dim[0], m_dim[1], m_dim[2]);
		const XMMATRIX Scale = a.toDirectXMatrix();
		a.initTranslation(m_position_center[0], m_position_center[1], m_position_center[2]);
		const XMMATRIX Rotation = Quat{ 0,0,0,1 }.getRotMat().toDirectXMatrix();
		const XMMATRIX Translation = a.toDirectXMatrix();
		const XMMATRIX ObjToWorld = (Scale * Rotation) * Translation;

		DUC->drawRigidBody(ObjToWorld);
	}
	
	BoxObstacle(Vec3 dim, Vec3 pos, float temp) {
		m_dim = dim;
		m_position = pos;
		m_temp = temp;
		m_position_center = Vec3(m_position[0] + m_dim[0] * 0.5, m_position[1] + m_dim[1] * 0.5, m_position[2] + m_dim[2] * 0.5);
		name = "Box";
	}

	Vec3 isInsideorSurface(Vec3 position, bool* isinside) {
		
		if(position[0]>=m_position[0] && position[0]<=(m_position[0]+m_dim[0]))
			if(position[1]>=m_position[1] && position[1] <= (m_position[1] + m_dim[1]))
				if (position[2] >= m_position[2] && position[2] <= (m_position[2] + m_dim[2])) {
					*isinside = true;
					float disx = position[0] - m_position[0];
					float disy = position[1] - m_position[1];
					float disz = position[2] - m_position[2];
					if (disx <= disy && disx <= disz) {
						return Vec3(m_position[0], position[1], position[2]);
					}
					if (disy <= disx && disy <= disz) {
						return Vec3(position[0], m_position[1], position[2]);
					}
					if (disz <= disx && disz <= disy) {
						return Vec3(position[0], position[1], m_position[2]);
					}
				}

		*isinside = false;
		return position;


	}


private:
	Vec3 m_dim;
	Vec3 m_position;
	Vec3 m_position_center;
	

};

class CylinderObstacle : public Obstacle {
public:
	void drawObstacle(DrawingUtilitiesClass* DUC) {
		if(cylinder == nullptr)
			 cylinder = GeometricPrimitive::CreateCylinder(DUC->g_pd3dImmediateContext, m_height, m_diag, 32, false);

		Mat4f a;
		a.initScaling(1, 1, 1);
		const XMMATRIX Scale = a.toDirectXMatrix();
		a.initTranslation(m_position_center[0], m_position_center[1], m_position_center[2]);
		const XMMATRIX Rotation = Quat{ 0,0,0,1 }.getRotMat().toDirectXMatrix();
		const XMMATRIX Translation = a.toDirectXMatrix();
		const XMMATRIX ObjToWorld = Scale * Rotation * Translation;
		DUC->g_pEffectPositionNormal->SetWorld(ObjToWorld * DUC->g_camera.GetWorldMatrix());
		cylinder->Draw(DUC->g_pEffectPositionNormal,DUC->g_pInputLayoutPositionNormal);
	}

	CylinderObstacle(Vec3 base_position, float height, float d, float temp) {
		m_temp = temp;
		m_diag = d;
		m_rad = d / 2;
		m_height = height;
		m_basePosition = base_position;
		cylinder = nullptr;
		m_position_center = Vec3(m_basePosition[0], m_basePosition[1] +0.5*height, m_basePosition[2]);
		name = "Cylinder";
	}

	Vec3 isInsideorSurface(Vec3 position, bool* isinside) {
		if(checkboundingbox(position))
			if (position[1] >= m_basePosition[1] && position[1] <= (m_basePosition[1] + m_height)) {
				float d = sqrtf(powf(position[0] - m_basePosition[0], 2) + powf(position[2] - m_basePosition[2], 2));
				if (d <= m_diag / 2) {
					*isinside = true;
					if ((position[1] - m_basePosition[1]) < (d - m_diag / 2)) {
						return Vec3(position[0], m_basePosition[1], position[2]);
					}
					if ((m_basePosition[1] + m_height - position[1]) < (d - m_diag / 2)) {
						return Vec3(position[0], m_basePosition[1] + m_height, position[2]);
					}
					Vec3 tmp(position[0] - m_position_center[0], 0, position[2] - m_position_center[2]);
					tmp *= (m_diag / 2) / d;
					tmp += m_basePosition;
					return Vec3(tmp[0], position[1], tmp[2]);
				}
			}



		*isinside = false;
		return position;
	}


private:
	float m_height;
	float m_diag;
	float m_rad;
	Vec3 m_basePosition;
	Vec3 m_position_center;
	std::unique_ptr<DirectX::GeometricPrimitive> cylinder;

protected:
	bool checkboundingbox(Vec3 position) {

		if (position[0] >= (m_basePosition[0] - m_rad) && position[0] <= (m_basePosition[0] + m_rad))
			if (position[1] >= (m_basePosition[1]) && position[1] <= (m_basePosition[1] + m_height))
				if (position[2] >= (m_basePosition[2] - m_rad) && position[2] <= (m_basePosition[2] + m_rad))
					return true;


		return false;
	};

};

class ConeObstacle : public Obstacle {
public:
	void drawObstacle(DrawingUtilitiesClass* DUC) {
		if (cone == nullptr)
			cone = GeometricPrimitive::CreateCone(DUC->g_pd3dImmediateContext,m_dia, m_height,32,false);

		Mat4f a;
		a.initScaling(1, 1, 1);
		const XMMATRIX Scale = a.toDirectXMatrix();
		a.initTranslation(m_position_center[0], m_position_center[1], m_position_center[2]);
		const XMMATRIX Rotation = Quat{ 0,0,0,1 }.getRotMat().toDirectXMatrix();
		const XMMATRIX Translation = a.toDirectXMatrix();
		const XMMATRIX ObjToWorld = Scale * Rotation * Translation;
		DUC->g_pEffectPositionNormal->SetWorld(ObjToWorld * DUC->g_camera.GetWorldMatrix());
		cone->Draw(DUC->g_pEffectPositionNormal, DUC->g_pInputLayoutPositionNormal);
	}

	ConeObstacle(Vec3 base_position, float height, float d, float temp){
		m_temp = temp;
		m_dia = d;
		m_rad = d / 2;
		m_height = height;
		m_basePosition = base_position;
		m_position_center = Vec3(m_basePosition[0], m_basePosition[1] + 0.5 * height, m_basePosition[2]);
		name = "Cone";
		angle = atanf(m_rad/height);
	}

	Vec3 isInsideorSurface(Vec3 position, bool* isinside) {
		if(checkboundingbox(position))
			if (position[1] >= m_basePosition[1] && position[1] <= (m_basePosition[1] + m_height)) {
				float current_heigth = position[1] - m_basePosition[1];
				float current_rad = current_heigth * tanf(angle);
				float d = sqrtf(powf(position[0] - m_basePosition[0], 2) + powf(position[2] - m_basePosition[2], 2));
				if (d <= current_rad) {
					*isinside = true;
					if ((position[1] - m_basePosition[1]) < (d - current_rad)) {
						return Vec3(position[0], m_basePosition[1], position[2]);
					}
					if ((m_basePosition[1] + m_height - position[1]) < (d - current_rad)) {
						return Vec3(position[0], m_basePosition[1] + m_height, position[2]);
					}
					Vec3 tmp(position[0] - m_position_center[0], 0, position[2] - m_position_center[2]);
					tmp *= (current_rad) / d;
					tmp += m_basePosition;
					return Vec3(tmp[0], position[1], tmp[2]);
				}
			}



		*isinside = false;
		return position;
	}




private:
	float m_height;
	float m_dia;
	float m_rad;
	Vec3 m_basePosition;
	Vec3 m_position_center;
	float angle;
	std::unique_ptr<DirectX::GeometricPrimitive> cone;

protected:
	bool checkboundingbox(Vec3 position) {

		if (position[0] >= (m_basePosition[0] - m_rad) && position[0] <= (m_basePosition[0] + m_rad))
			if (position[1] >= (m_basePosition[1]) && position[1] <= (m_basePosition[1] + m_height))
				if (position[2] >= (m_basePosition[2] - m_rad) && position[2] <= (m_basePosition[2] + m_rad))
					return true;


		return false;
	};

};