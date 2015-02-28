#pragma once

#include <OpenGLES/FrmMesh.h>
#include "MyMesh.h"

//================================================================================================
//
// Wrapper for file *.mesh
//
//================================================================================================

class SkinnedMesh2 : public Mesh
{
public:

	// An action (e.g. run, walk, dead, ...) will begin at time of TimeStart (secs) and 
	// end at TimeEnd (secs).
	struct AnimAction
	{
		float TimeStart;
		float TimeEnd;
	};

public:
	SkinnedMesh2();
	~SkinnedMesh2();

	void init(
		CFrmMesh& mesh,
		FRM_ANIMATION_SET* animationSet,
		CFrmPackedResourceGLES& resource,
		Shader& shader,
		const MyVec3& pos,
		const MyVec3& rot,
		const MyVec3& scale,
		std::map<MyString, AnimAction>* animActions = nullptr);

	void update(Timer& timer);
	void render(Camera& camera);

	MyString getCurrentAction()const;
	void setCurrentAction(const MyString& name);

private:
	AnimAction getAction(const MyString& name)const;

public:
	static const UINT32 MAX_BONES = 26;

private:
	CFrmMesh* m_mesh;
	FRM_ANIMATION_SET* m_animationSet;

	MyString m_currentAction;
	std::map<MyString, AnimAction> m_animActions;

	UINT32 m_totalTicks;
};