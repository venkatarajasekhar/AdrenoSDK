#pragma once

#include <MySkinnedMesh2.h>
#include "EnemyAI.h"
#include <MyTexture.h>

inline MyVec2 convert3DTo2D(const MyVec3& posW, const MyMat4& view, const MyMat4& proj)
{
	struct { GLint x, y, width, height; } viewport;
	glGetIntegerv(GL_VIEWPORT, (GLint*)&viewport);

	FLOAT32 screenWidthHalf = (FLOAT32)viewport.width / 2.0f;
	FLOAT32 screenHeightHalf = (FLOAT32)viewport.height / 2.0f;

	/*
	MyString message = "screen : " + toString(viewport.width) + " " + toString(viewport.height);
	FrmLogMessage(message.c_str());
	/**/

	MyVec4 posH = MyVec4(posW, 1.0f);
	posH = proj * view * posH;
	float w = posH.w;
	posH = posH / w;

	return MyVec2(screenWidthHalf * (posH.x + 1), screenHeightHalf * (1 - posH.y));
}

class BloodBar
{
public:
	void init(Shader& texture2DShader, CFrmTexture* bloodTexture)
	{
		//m_bloodBar.init(MyVec2(0, 0), 1, bloodTexture, texture2DShader);
	}

//	void update();
	void render(Camera& camera)
	{
		//m_bloodBar.setPos(convert3DTo2D(m_pos, camera.getView(), camera.getProj()));
		//m_bloodBar.render();
	}

	MyVec3 m_pos;

private:

	Texture m_bloodBar;

	Shader* m_texture2DShader;
	CFrmTexture* m_bloodTexture;
};

class Dman
{
private:
	SkinnedMesh2::Instance* m_instance;
	EnemyAI m_ai;
	//BloodBar m_bloodBar;
	int m_id;
	int m_type;
	int m_helth;
	int m_dam;
	static int m_numIDs;
	bool m_isDeleted;
public:
	Dman();
	void init(
		int type,
		const MyVec3& pos,
		const MyVec3& rot,
		const MyVec3& scale);

	void update(Timer& timer);
	void copyAllProperties();

	SkinnedMesh2::Instance* getDman();
	bool getIsDeleted();
	int getId();
	void setHelth(int helth);
	int getHelth();
	void setDam(int dam);
	int getDam();
};