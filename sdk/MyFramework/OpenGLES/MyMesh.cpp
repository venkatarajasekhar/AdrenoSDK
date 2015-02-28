
#include "MyMesh.h"

Mesh::Mesh()
	: m_shader(nullptr)
{
}

Mesh::~Mesh()
{
}

void Mesh::init(Shader& shader, const MyVec3& pos, const MyVec3& rot, const MyVec3& scale)
{
	m_shader = &shader;
	setPos(pos);
	setRot(rot);
	setScale(scale);
}

void Mesh::update(Timer& timer)
{
}

void Mesh::render(Camera& camera)
{
	m_shader->apply();

	m_world = createTranslationMatrix(m_pos);
	m_world *= createYawPitchRollMatrix(m_rot.y, m_rot.x, m_rot.z);
	m_world *= createScaleMatrix(m_scale);

	m_shader->setUniform("u_world", m_world);
	m_shader->setUniform("u_view", camera.getView());
	m_shader->setUniform("u_proj", camera.getProj());
}

//==========================================================================================================
//
// Getter
//
//==========================================================================================================

MyVec3 Mesh::getPos()const
{
	return m_pos;
}

MyVec3 Mesh::getRot()const
{
	return m_rot;
}

MyVec3 Mesh::getScale()const
{
	return m_scale;
}

//==========================================================================================================
//
// Setter
//
//==========================================================================================================

void Mesh::setPos(const MyVec3& pos)
{
	m_pos = pos;
}

void Mesh::setRot(const MyVec3& rot)
{
	m_rot = rot;
}

void Mesh::setScale(const MyVec3& scale)
{
	m_scale = scale;
}