
#include "MyQuad3D.h"

BasicMesh Quad3D::m_mesh;
bool Quad3D::m_initedStatic = false;

Quad3D::Quad3D()
{
}

Quad3D::~Quad3D()
{
}

void Quad3D::init(Texture* diffuseMap, Shader& shader, const MyVec3& pos, const MyVec3& rot, const MyVec2& size)
{
	if (!m_initedStatic)
	{
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

		m_mesh.init(vertices, indices, shader, diffuseMap, pos, rot, MyVec3(size.x, 1.0f, size.y));

		m_initedStatic = true;
	}
}

void Quad3D::update(Timer& timer)
{
	m_mesh.update(timer);
}

void Quad3D::render(Camera& camera)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	m_mesh.render(camera);

	glDisable(GL_BLEND);
}

// Setter

void Quad3D::setPos(const MyVec3& pos)
{
	m_mesh.setPos(pos);
}

void Quad3D::setRot(const MyVec3& rot)
{
	m_mesh.setRot(rot);
}

void Quad3D::setSize(const MyVec2& size)
{
	m_mesh.setScale(MyVec3(size.x, 1.0f, size.y));
}

void Quad3D::setDiffuseMap(Texture* diffuseMap)
{
	m_mesh.setDiffuseMap(diffuseMap);
}