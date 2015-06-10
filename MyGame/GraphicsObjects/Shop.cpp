
#include "Shop.h"

static const float SELECTED_RADIUS = 5.0f;
static const MyVec3 MODEL_OFFSET = MyVec3(1.5f, 0, 5);

Shop::Shop()
	: m_selectedDecal(nullptr)
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
	const MyVec3& scale,
	Quad3D& selectedDecal)
{
	m_selectedDecal = &selectedDecal;

	{
		Material material;
		material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
		material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
		material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
		material.Shininess = 16.0f;

		m_mesh.init(model, modelTexture, shader, &material);
	}

	m_mesh.addInstance(Mesh::buildMeshInstance(pos, rot, scale));
	m_isPressed = false;
}

void Shop::update(Timer& timer, bool& isPressed, MyVec3& pressedPoint)
{
	//m_isPressed = isPressed;
	MyVec3 pos = m_mesh.getInstance(0)->Position;
	if (isPressed && (distance_optimized(pos + MODEL_OFFSET, pressedPoint) <= SELECTED_RADIUS))
	{
		IOnPressListener::Data data("shop", 0, 0);
		throwPressEvent(data);

		isPressed = false;
	}

	m_mesh.update(timer);
}

void Shop::render(Camera& camera, Light& light)
{
	/*if (m_isPressed)
	{
		MyVec3 pos = m_mesh.getInstance(0)->Position;
		m_selectedDecal->setPos(pos + MODEL_OFFSET + MyVec3(0, 0.1f, 0));
		m_selectedDecal->setSize(MyVec2(2.0f * SELECTED_RADIUS));
		m_selectedDecal->render(camera);
	}*/

	m_mesh.render(camera, &light);
}