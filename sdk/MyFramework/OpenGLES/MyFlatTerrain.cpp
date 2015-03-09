
#include "MyFlatTerrain.h"

FlatTerrain::FlatTerrain()
	: m_diffuseMap1(nullptr),
	m_diffuseMap2(nullptr),
	m_blendMap(nullptr)
{
}

FlatTerrain::~FlatTerrain()
{
}

void FlatTerrain::init(
	Shader& shader,
	Texture& diffuseMap1,
	Texture& diffuseMap2,
	Texture& blendMap,
	const MyVec2& size,
	FlatTerrainProperties& properties)
{
	m_diffuseMap1 = &diffuseMap1;
	m_diffuseMap2 = &diffuseMap2;
	m_blendMap = &blendMap;
	m_properties = properties;

	{
		// Init mesh
		std::vector<PosTexVertex> vertices;
		vertices.resize(4);
		vertices[0] = PosTexVertex(MyVec3(-0.5f, +0.0f, -0.5f), MyVec2(0, 0));
		vertices[1] = PosTexVertex(MyVec3(-0.5f, +0.0f, +0.5f), MyVec2(0, 1));
		vertices[2] = PosTexVertex(MyVec3(+0.5f, +0.0f, +0.5f), MyVec2(1, 1));
		vertices[3] = PosTexVertex(MyVec3(+0.5f, +0.0f, -0.5f), MyVec2(1, 0));

		UIntArray indices;
		indices.resize(6);

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 0;
		indices[4] = 2;
		indices[5] = 3;

		m_mesh.init(vertices, indices, shader, nullptr, MyVec3(0), MyVec3(0), MyVec3(size.x, 1.0f, size.y));
	}
}

void FlatTerrain::render(Camera& camera)
{
	Shader* shader = m_mesh.getShader();

	shader->apply();

	shader->setUniform("u_diffuseSampler1", m_diffuseMap1->bind());
	shader->setUniform("u_diffuseSampler2", m_diffuseMap2->bind());
	shader->setUniform("u_blendSampler", m_blendMap->bind());

	shader->setUniform("u_properties.TileRepeatFactor", m_properties.TileRepeatFactor);
	shader->setUniform("u_properties.Threshold", m_properties.Threshold);

	m_mesh.render(camera);
}