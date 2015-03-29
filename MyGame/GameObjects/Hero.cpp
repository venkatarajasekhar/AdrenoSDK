
#include "Hero.h"

Hero::Hero()
{
}

Hero::~Hero()
{
}

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
}

void Hero::update(UserInput& userInput, Timer& timer)
{
	m_skinnedMesh.update(timer);
}

void Hero::render(Camera& camera, Light& light)
{
	m_skinnedMesh.render(camera, &light);
}