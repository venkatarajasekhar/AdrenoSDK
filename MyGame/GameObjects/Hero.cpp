
#include "Hero.h"

Hero::Hero()
	: m_skinnedMeshIns(nullptr)
{
}

Hero::~Hero()
{
}

// Core functions

void Hero::init(
	Adreno::Model* model,
	Adreno::Animation* anim,
	Texture** modelTexture,
	Shader& shader,
	Material& material,
	const MyVec3& pos,
	const MyVec3& rot,
	const MyVec3& scale,
	std::map<MyString, SkinnedMesh1::AnimAction>& animActions,
	FLOAT32 speedFactor)
{
	// Mesh/Appearance elements
	m_skinnedMesh.init(
		model,
		anim,
		modelTexture,
		shader,
		&material,
		&animActions,
		speedFactor);

	m_skinnedMeshIns = SkinnedMesh1::buildSkinnedMeshInstance(pos, rot, scale, "Idle");
	m_skinnedMesh.addInstance(m_skinnedMeshIns);

	// Moving elements
	m_movingEnt.init(m_skinnedMeshIns->Position, m_skinnedMeshIns->Position, m_skinnedMeshIns->Rotation,
		0, 3.0f, 3.0f);
}

void Hero::update(UserInput& userInput, Timer& timer)
{
	// Moving elements
	m_movingEnt.update(timer);

	m_skinnedMeshIns->Position = m_movingEnt.getPos();
	m_skinnedMeshIns->Rotation = m_movingEnt.getRot();

	// Mesh/Appearance elements
	m_skinnedMesh.update(timer);
}

void Hero::render(Camera& camera, Light& light)
{
	// Mesh/Appearance elements
	m_skinnedMesh.render(camera, &light);
}