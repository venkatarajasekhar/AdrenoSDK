
#pragma once

#include "IRenderableEntity.h"
#include <MySkinnedMesh1.h>

class Hero : public IRenderableEntity
{
public:
	Hero();
	virtual ~Hero();

	virtual void init(
		Adreno::Model* model,
		Adreno::Animation* anim,
		Texture** modelTexture,
		Shader& shader,
		Material& material,
		const MyVec3& pos,
		const MyVec3& rot,
		const MyVec3& scale,
		std::map<MyString, SkinnedMesh1::AnimAction>& animActions,
		FLOAT32 speedFactor = 1.0f);
	virtual void update(UserInput& userInput, Timer& timer);
	virtual void render(Camera& camera, Light& light);

protected:
	SkinnedMesh1 m_skinnedMesh;
	SkinnedMesh1::Instance* m_skinnedMeshIns;
};