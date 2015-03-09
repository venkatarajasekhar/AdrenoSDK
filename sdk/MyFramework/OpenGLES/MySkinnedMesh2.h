#pragma once

#include "FrmMesh.h"
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

	struct Instance : public Mesh::Instance
	{
		MyString CurrentAction;
		UINT32   TotalTicks;
	};

public:
	SkinnedMesh2();
	~SkinnedMesh2();

	void init(
		CFrmMesh& mesh,
		FRM_ANIMATION_SET* animationSet,
		CFrmPackedResourceGLES& resource,
		Shader& shader,
		Material* material = nullptr,
		std::map<MyString, AnimAction>* animActions = nullptr);

	void update(Timer& timer);
	void render(Camera& camera, Light* light = nullptr);

private:
	AnimAction getAction(const MyString& name)const;

public:
	static SkinnedMesh2::Instance* buildSkinnedMeshInstance(const MyVec3& pos, const MyVec3& rot, const MyVec3& scale, const MyString& action);

public:
	static const UINT32 MAX_BONES = 26;

private:
	CFrmMesh* m_mesh;
	FRM_ANIMATION_SET* m_animationSet;

	std::map<MyString, AnimAction> m_animActions;
};