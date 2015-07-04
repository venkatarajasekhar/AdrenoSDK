
#pragma once

#include "MyMesh.h"
#include "MyInputLayout.h"
#include "MyVertex.h"
#include "MyTexture.h"

//===============================================================================================================
//
// Declaring BasicMesh class
//
//===============================================================================================================

class BasicMesh : public Mesh
{
public:
	BasicMesh();
	virtual ~BasicMesh();

	template <typename VertexType>
	void init(
		std::vector<VertexType>& vertices,
		UIntArray& indices,
		Shader& shader,
		Texture* diffuseMap,
		const MyVec3& pos,
		const MyVec3& rot,
		const MyVec3& scale,
		Material* material = nullptr);

	virtual void render(Camera& camera, Light* light = nullptr);

	// Getter

	MyVec3 getPos();
	MyVec3 getRot();
	MyVec3 getScale();

	// Setter

	void setPos(const MyVec3& pos);
	void setRot(const MyVec3& rot);
	void setScale(const MyVec3& scale);

	void setDiffuseMap(Texture* diffuseMap);

	void setPrimitive(GLenum primitive);

protected:
	GLuint m_vbo;
	GLuint m_ibo;
	GLuint m_numIndices;

	InputLayout m_inputLayout;
	Texture* m_diffuseMap;

	GLenum m_primitive;
};

//===============================================================================================================
//
// Implementing BasicMesh class
//
//===============================================================================================================

template <typename VertexType>
void BasicMesh::init(
	std::vector<VertexType>& vertices,
	UIntArray& indices,
	Shader& shader,
	Texture* diffuseMap,
	const MyVec3& pos,
	const MyVec3& rot,
	const MyVec3& scale,
	Material* material)
{
	m_diffuseMap = diffuseMap;
	m_inputLayout.init(VertexType::ShaderAttribsDesc, VertexType::NumShaderAttribsDesc);

	// Init vertex buffer

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexType), &vertices[0], GL_STATIC_DRAW);

	// Init index buffer

	m_numIndices = indices.size();

	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	// Add an instance
	addInstance(Mesh::buildMeshInstance(pos, rot, scale));

	//Mesh::init(shader, pos, rot, scale, material);
	Mesh::init(shader, material);
}