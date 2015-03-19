
#include "MyBillboard.h"
#include "MySpriteSheet.h"

Billboard::Billboard()
	: m_texture(nullptr)
{
}

Billboard::~Billboard()
{
}

void Billboard::init(Texture* texture, Shader& shader, const MyVec3& pos, const MyVec2& size, float rot)
{
	m_texture = texture;
	m_billboardSize = size;
	{
		// Init mesh
		std::vector<BillboardVertex> vertices;
		vertices.resize(4);
		
		vertices[0] = BillboardVertex(MyVec2(0, 0));
		vertices[1] = BillboardVertex(MyVec2(0, 1));
		vertices[2] = BillboardVertex(MyVec2(1, 1));
		vertices[3] = BillboardVertex(MyVec2(1, 0));

		UIntArray indices;
		indices.resize(6);

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 0;
		indices[4] = 2;
		indices[5] = 3;

		m_mesh.init(vertices, indices, shader, texture, pos, MyVec3(0), MyVec3(1));
	}
}

void Billboard::update(Timer& timer)
{
	m_mesh.update(timer);
}

void Billboard::render(Camera& camera)
{
	Shader* shader = m_mesh.getShader();

	shader->apply();

	shader->setUniform("u_texMat", m_texture->getTexMat());
	shader->setUniform("u_billboardSize", m_billboardSize);
	shader->setUniform("u_eye", camera.getEye());

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	m_mesh.render(camera);

	glDisable(GL_BLEND);
}

void Billboard::setPos(const MyVec3& pos)
{
	m_mesh.setPos(pos);
}