
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

	virtual void init(Shader& shader, Material* material = nullptr);
	virtual void update(Timer& timer);
	virtual void render(Camera& camera, Light* light = nullptr);

	// Getter

	Shader* getShader()const;

	int getNumInstances();
	Instance* getInstance(int id);

	// Setter

	// An instance must be initialized on heap (i.e. using 'new' operator)
	void addInstance(Instance* instance);
	void removeInstance(Instance* instance);

	void enableLighting();
	void disableLighting();

public:
	static Instance* buildMeshInstance(const MyVec3& pos, const MyVec3& rot, const MyVec3& scale);

protected:
	Shader* m_shader;

	std::vector<Instance*> m_instances;

	Material m_material;
	bool m_lightingEnabled;
};