
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

void BasicMesh::render(Camera& camera)
{
	Mesh::render(camera);

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

void BasicMesh::setDiffuseMap(Texture* diffuseMap)
{
	m_diffuseMap = diffuseMap;
}