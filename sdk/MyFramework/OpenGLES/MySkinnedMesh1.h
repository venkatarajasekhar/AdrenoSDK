
#pragma once

#include "MyFileMesh1.h"

//===============================================================================================================
//
// Wrapper for file *.model
//
// TERMINOLOGY:
//  - Joint: Bone
//  - Animation: A complete action such as run, fight, dead, ...
//  - Frame (Pose): An animation consists of many frames. Time t0 corresponds to frame 0,
//    time t1 corresponds to frame 1, ...
//  - Key-frame: A frame consists of many transforms which control joints. Such a transform is called key-frame.
//    So, obviously, a frame consists of many key-frames.
//  - Track: An animation is considered as a matrix of key-frames. Each row is called a frame and each column
//    is called a track.
//
//===============================================================================================================

class SkinnedMesh1 : public FileMesh1
{

#pragma region Constants

public:
	static const UINT32 MAX_BONES       = 60;
	static const UINT32 TOTAL_BONES     = 256;
	static const UINT32 TICKS_PER_FRAME = 150;

#pragma endregion

#pragma region Structs

public:

	// An action (e.g. run, walk, dead, ...) will begin at Frame of FrameStart and last FrameLength frames.
	struct AnimAction
	{
		UINT32 FrameStart;
		UINT32 FrameLength;
	};

	struct AnimFile
	{
		AnimFile(const MyString& _fileName, const MyString& _name, const AnimAction& _range = {0, 0})
			: FileName(_fileName), Name(_name), Range(_range), Anim(nullptr)
		{}

		MyString FileName;
		Adreno::Animation* Anim;
		
		MyString Name;
		AnimAction Range;
	};

	struct AnimData
	{
		AnimData();
		~AnimData();

		void init(const MyString& filename, std::map<MyString, AnimAction>* animActions = nullptr);
		void init(AnimFile* animFiles, int numAnimFiles);

		Adreno::Animation* Anim;
		std::map<MyString, AnimAction> AnimActions;
	};

	// Some problems with animation action:
	//	1. Set current action
	//	2. Action is unloop
	//	3. Some event occurring at halfway of an action (0->1)

	struct Instance : public Mesh::Instance
	{
		// At time of t0, we're being between m_leftFrame Frame and m_rightFrame Frame.
		// We're interpolating transform at t0 basing on m_leftFrame Frame and m_rightFrame Frame.
		INT32   LeftFrame;
		INT32   RightFrame;
		FLOAT32 FrameWeight;

		bool LoopedAction;
		MyString CurrentAction;
		MyString NextAction;

		UINT32   TotalTicks;

		Instance();

		// action: Current action will be played.
		// looped: Current action is looped or not.
		// nextAction: If current action is unlooped, play 'nextAction' action when finish current action
		void setAction(const MyString& action, const MyString& nextAction = "", bool looped = true);

		/*------------------------- Temp ------------------------------*/
		void setActionAndReset(const MyString& action)
		{
			CurrentAction = action;
			TotalTicks = 0;
		}
		/**/
	};

#pragma endregion

public:
	SkinnedMesh1();
	~SkinnedMesh1();

	void init(
		FileMesh1::MeshData& model,
		SkinnedMesh1::AnimData& anim,
		FileMesh1::MeshTextures& modelTexture,
		Shader& shader,
		Material* material = nullptr,
		FLOAT32 speedFactor = 1.0f);

	void update(Timer& timer);
	void render(Camera& camera, Light* light = nullptr);

private:
	void setWorldArray(SkinnedMesh1::Instance* instance);

	AnimAction getAction(const MyString& name)const;

public:
	static SkinnedMesh1::Instance* buildSkinnedMeshInstance(
		const MyVec3& pos, 
		const MyVec3& rot, 
		const MyVec3& scale, 
		const MyString& action);

private:
	Adreno::Animation* m_anim;

	// Bone remapping
	UINT32 m_boneRemap[MAX_BONES];
	UINT32 m_boneRemapCount;

	// Animation speed and range
	FLOAT32 m_speedFactor;

	std::map<MyString, AnimAction> m_animActions;
};