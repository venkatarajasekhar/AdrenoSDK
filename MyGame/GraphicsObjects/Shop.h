
#pragma once

#include <MyFileMesh1.h>
#include <MyEventListener.h>
#include <MyQuad3D.h>

class Shop : public OnPressListenee
{
public:
	Shop();
	~Shop();

	void init(
		FileMesh1::MeshData& model, 
		FileMesh1::MeshTextures& modelTexture, 
		Shader& shader,
		const MyVec3& pos,
		const MyVec3& rot,
		const MyVec3& scale,
		Quad3D& selectedDecal);
	void update(Timer& timer, bool isPressed, MyVec3& pressedPoint);
	void render(Camera& camera, Light& light);

private:
	FileMesh1 m_mesh;
	Quad3D* m_selectedDecal;
};