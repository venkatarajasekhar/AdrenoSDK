
#include "MyBasicMesh.h"

BasicMesh::BasicMesh()
	: m_vbo(0),
	m_ibo(0),
	m_numIndices(0),
	m_diffuseMap(nullptr)
{
}

BasicMesh::~BasicMesh()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
	m_vbo = 0;
	m_ibo = 0;
}

void BasicMesh::render(Camera& camera, Light* light)
{
	Mesh::render(camera, light);

	// Set world matrix
	Instance* instance = getInstance(0);
	if (instance != nullptr)
	{
		m_shader->setUniform("u_world", instance->World);
	}
	
	// Bind texture
	if (m_diffuseMap != nullptr)
	{
		m_shader->setUniform("u_diffuseSampler", m_diffuseMap->bind());
	}

	// Bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	m_inputLayout.bind();

	// Render
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, NULL);

	// Unbind vertex buffer
	m_inputLayout.unbind();
}

// Getter

MyVec3 BasicMesh::getPos()
{
	Instance* instance = getInstance(0);
	return instance->Position;
}

MyVec3 BasicMesh::getRot()
{
	Instance* instance = getInstance(0);
	return instance->Rotation;
}

MyVec3 BasicMesh::getScale()
{
	Instance* instance = getInstance(0);
	return instance->Scale;
}

// Setter

void BasicMesh::setPos(const MyVec3& pos)
{
	Instance* instance = getInstance(0);
	instance->Position = pos;
}

void BasicMesh::setRot(const MyVec3& rot)
{
	Instance* instance = getInstance(0);
	instance->Rotation = rot;
}

void BasicMesh::setScale(const MyVec3& scale)
{
	Instance* instance = getInstance(0);
	instance->Scale = scale;
}

void BasicMesh::setDiffuseMap(Texture* diffuseMap)
{
	m_diffuseMap = diffuseMap;
}