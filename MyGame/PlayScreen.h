
#pragma once

#include <MyScreen.h>

#include <MyPerspectiveCamera.h>
#include <MyBasicMesh.h>
#include <MySkinnedMesh1.h>
#include <MySkinnedMesh2.h>

class PlayScreen : public Screen
{
public:
	PlayScreen(ScreenManager* screenManager);
	~PlayScreen();

	void init();
	void resize(int width, int height);
	void update(void* utilObjs);
	void render(void* utilObjs);

private:

	// Core objects
	PerspectiveCamera m_camera_main;

	// Shader resources
	Shader m_shader_terrain;
	Shader m_shader_mesh;
	Shader m_shader_skinnedMesh1;
	Shader m_shader_skinnedMesh2;

	// Texture resources
	Texture m_texture_grass;
	Texture** m_textures_scorpion;

	// Mesh resources
	Adreno::Model* m_meshData_scorpion;
	Adreno::Animation* m_animData_scorpion;

	CFrmMesh m_meshData_boy;
	FRM_ANIMATION_SET* m_animData_boy;

	// Mesh objects
	BasicMesh m_mesh_terrain;
	//FileMesh1 m_mesh_scorpion;
	//SkinnedMesh1 m_skinnedMesh_scorpion;
	SkinnedMesh2 m_skinnedMesh_boy;
};