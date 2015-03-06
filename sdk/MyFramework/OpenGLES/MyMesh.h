
#pragma once

#include "MyTimer.h"
#include "MyShader.h"
#include "MyCamera.h"
#include "MyLight.h"
#include "MyMaterial.h"

class Mesh
{
public:
	Mesh();
	virtual ~Mesh();

	virtual void init(Shader& shader, const MyVec3& pos, const MyVec3& rot, const MyVec3& scale, Material* material = nullptr);
	virtual void update(Timer& timer);
	virtual void render(Camera& camera, Light* light = nullptr);

	// Getter

	MyVec3 getPos()const;
	MyVec3 getRot()const;
	MyVec3 getScale()const;

	Shader* getShader()const;

	// Setter

	void setPos(const MyVec3& pos);
	void setRot(const MyVec3& rot);
	void setScale(const MyVec3& scale);

	void enableLighting();
	void disableLighting();

protected:
	Shader* m_shader;

	MyVec3 m_pos;
	MyVec3 m_rot;
	MyVec3 m_scale;
	MyMat4 m_world;

	Material m_material;
	bool m_lightingEnabled;
};