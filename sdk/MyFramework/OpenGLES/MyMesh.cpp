
#include "MyMesh.h"

Mesh::Mesh()
	: m_shader(nullptr),
	m_lightingEnabled(false)
{
}

Mesh::~Mesh()
{
}

void Mesh::init(Shader& shader, const MyVec3& pos, const MyVec3& rot, const MyVec3& scale, Material* material)
{
	m_shader = &shader;

	setPos(pos);
	setRot(rot);
	setScale(scale);

	if (material != nullptr)
	{
		m_material = *material;
	}
}

void Mesh::update(Timer& timer)
{
}

void Mesh::render(Camera& camera, Light* light)
{
	m_shader->apply();

	m_world = createTranslationMatrix(m_pos);
	m_world *= createYawPitchRollMatrix(m_rot.y, m_rot.x, m_rot.z);
	m_world *= createScaleMatrix(m_scale);

	m_shader->setUniform("u_world", m_world);
	m_shader->setUniform("u_view", camera.getView());
	m_shader->setUniform("u_proj", camera.getProj());

	if (m_lightingEnabled && (light != nullptr))
	{
		m_shader->setUniform("u_material.Ambient", m_material.Ambient);
		m_shader->setUniform("u_material.Diffuse", m_material.Diffuse);
		m_shader->setUniform("u_material.Specular", m_material.Specular);
		m_shader->setUniform("u_material.Shininess", m_material.Shininess);

		m_shader->setUniform("u_light.PosOrDir", light->PosOrDir);

		m_shader->setUniform("u_eyePos", camera.getEye());
	}
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

Shader* Mesh::getShader()const
{
	return m_shader;
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

void Mesh::enableLighting()
{
	m_lightingEnabled = true;
}

void Mesh::disableLighting()
{
	m_lightingEnabled = false;
}