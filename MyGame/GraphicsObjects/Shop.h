
#pragma once

#include <MyFileMesh1.h>
#include <MyEventListener.h>
#include <MyQuad3D.h>
#include <MyAudio.h>

class Shop : public OnPressListenee
{
public:

	enum
	{
		AUDIO_MYPAWN_ATTACK,
		AUDIO_MYPAWN_DEATH,
		AUDIO_ENEMYPAWN_ATTACK,
		AUDIO_ENEMYPAWN_DEATH,
		AUDIO_MYIFV_ATTACK,
		AUDIO_ENEMYIFV_ATTACK,
		AUDIO_IFV_DEATH,
		AUDIO_MYTOWER_ATTACK,
		AUDIO_ENEMYTOWER_ATTACK,
		AUDIO_TOWER_DEATH,
		AUDIO_MYHERO_ATTACK,
		AUDIO_MYHERO_DEATH,
		AUDIO_ENEMYHERO_ATTACK,
		AUDIO_ENEMYHERO_ATTACK2,
		AUDIO_ENEMYHERO_DEATH,
		AUDIO_HEALTH_SKILL,
		AUDIO_UPGRADE_SKILL,
		AUDIO_SHOP_OPEN,
		AUDIO_BACKGROUND,
		NUM_AUDIOS,
	};

	Shop();
	~Shop();

	void init(
		FileMesh1::MeshData& model, 
		FileMesh1::MeshTextures& modelTexture, 
		Shader& shader,
		const MyVec3& pos,
		const MyVec3& rot,
		const MyVec3& scale,
		Quad3D& selectedDecal,
		Audio lAudios[]);
	void update(Timer& timer, bool& isPressed, MyVec3& pressedPoint);
	void render(Camera& camera, Light& light);

private:
	FileMesh1 m_mesh;
	Quad3D* m_selectedDecal;
	bool m_isPressed;

	Audio* m_audios[NUM_AUDIOS];
};