
#pragma once

#include "MyTimer.h"
#include "MyShader.h"
#include "MyCamera.h"
#include "MyLight.h"
#include "MyMaterial.h"

class Mesh
{
public:
	struct Instance
	{
		MyVec3 Position;
		MyVec3 Rotation;
		MyVec3 Scale;
		MyMat4 World;
	};

public:
	Mesh();
	virtual ~Mesh();

	//virtual void init(Shader& shader, const MyVec3& pos, const MyVec3& rot, const MyVec3& scale, Material* material = nullptr);
	virtual void init(Shader& shader, Material* material = nullptr);
	virtual void update(Timer& timer);
	virtual void render(Camera& camera, Light* light = nullptr);

	/*
	// Getter

	MyVec3 getPos()const;
	MyVec3 getRot()const;
	MyVec3 getScale()const;

	Shader* getShader()const;

	// Setter

	void setPos(const MyVec3& pos);
	void setRot(const MyVec3& rot);
	void setScale(const MyVec3& scale);
	/**/

	
	// Getter

	Shader* getShader()const;

	int getNumInstances();
	Instance* getInstance(int id);

	// Setter

	// An instance must be initialized on heap (i.e. using 'new' operator)
	void addInstance(Instance* instance);

	void enableLighting();
	void disableLighting();

public:
	static Instance* buildMeshInstance(const MyVec3& pos, const MyVec3& rot, const MyVec3& scale);

protected:
	Shader* m_shader;

	std::vector<Instance*> m_instances;
	/*
	MyVec3 m_pos;
	MyVec3 m_rot;
	MyVec3 m_scale;
	MyMat4 m_world;
	/**/

	Material m_material;
	bool m_lightingEnabled;
};