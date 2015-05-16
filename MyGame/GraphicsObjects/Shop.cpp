
#include "Shop.h"

Shop::Shop()
{
}

Shop::~Shop()
{
}

void Shop::init(
	FileMesh1::MeshData& model,
	FileMesh1::MeshTextures& modelTexture,
	Shader& shader,
	const MyVec3& pos,
	const MyVec3& rot,
	const MyVec3& scale)
{
	{
		Material material;
		material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
		material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
		material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
		material.Shininess = 16.0f;

		m_mesh.init(model, modelTexture, shader, &material);
	}

	m_mesh.addInstance(Mesh::buildMeshInstance(pos, rot, scale));
}

void Shop::update(Timer& timer, bool isPressed, MyVec3& pressedPoint)
{
	MyVec3 pos = m_mesh.getInstance(0)->Position;
	if (isPressed && (distance_optimized(pos, pressedPoint) <= 5.0))
	{
		IOnPressListener::Data data("shop", 0, 0);
		throwPressEvent(data);
	}

	m_mesh.update(timer);
}


void Shop::render(Camera& camera, Light& light)
{
	m_mesh.render(camera, &light);
}