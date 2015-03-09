
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
public:
	static const UINT32 MAX_BONES = 50;
	static const UINT32 TOTAL_BONES = 256;
	static const UINT32 TICKS_PER_FRAME = 150;

public:

	// An action (e.g. run, walk, dead, ...) will begin at Frame of FrameStart and last FrameLength frames.
	struct AnimAction
	{
		UINT32 FrameStart;
		UINT32 FrameLength;
	};

	struct Instance : public Mesh::Instance
	{
		// At time of t0, we're being between m_leftFrame Frame and m_rightFrame Frame.
		// We're interpolating transform at t0 basing on m_leftFrame Frame and m_rightFrame Frame.
		//INT32   LeftFrame;
		//INT32   RightFrame;
		//FLOAT32 FrameWeight;

		MyString CurrentAction;
		UINT32   TotalTicks;

		FRMMATRIX4X3 WorldArray[MAX_BONES];
	};

public:
	SkinnedMesh1();
	~SkinnedMesh1();

	/*
	void init(
		Adreno::Model* model,
		Adreno::Animation* anim,
		Texture** modelTexture,
		Shader& shader,
		const MyVec3& pos,
		const MyVec3& rot,
		const MyVec3& scale,
		Material* material = nullptr,
		std::map<MyString, AnimAction>* animActions = nullptr,
		FLOAT32 speedFactor = 1.0f);
	/**/
	void init(
		Adreno::Model* model,
		Adreno::Animation* anim,
		Texture** modelTexture,
		Shader& shader,
		Material* material = nullptr,
		std::map<MyString, AnimAction>* animActions = nullptr,
		FLOAT32 speedFactor = 1.0f);

	void update(Timer& timer);

	//MyString getCurrentAction()const;
	//void setCurrentAction(const MyString& name);

private:
	//void foreachSubmesh(int index);
	void foreachInstance(int id);

	AnimAction getAction(const MyString& name)const;

public:
	static SkinnedMesh1::Instance* buildSkinnedMeshInstance(const MyVec3& pos, const MyVec3& rot, const MyVec3& scale, const MyString& action);

private:
	Adreno::Animation* m_anim;

	// Bone remapping
	UINT32 m_boneRemap[MAX_BONES];
	UINT32 m_boneRemapCount;

	/*
	// At time of t0, we're being between m_leftFrame Frame and m_rightFrame Frame.
	// We're interpolating transform at t0 basing on m_leftFrame Frame and m_rightFrame Frame.
	INT32   m_leftFrame;
	INT32   m_rightFrame;
	FLOAT32 m_frameWeight;
	/**/

	// Animation speed and range
	FLOAT32 m_speedFactor;

	std::map<MyString, AnimAction> m_animActions;
	//MyString m_currentAction;

	//UINT32 m_totalTicks;
};