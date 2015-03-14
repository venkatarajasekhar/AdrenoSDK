
#include "MySpriteBatch.h"

SpriteBatch::SpriteBatch()
	: m_scrWidth(0),
	m_scrHeight(0)
{
}

SpriteBatch::~SpriteBatch()
{
}

void SpriteBatch::init(Shader& shader)
{
	m_camera2D.init();

	std::vector<Pos2TexVertex> vertices;
	vertices.resize(4);
	vertices[0] = Pos2TexVertex(MyVec2(-0.5f, +0.5f), MyVec2(0, 0));
	vertices[1] = Pos2TexVertex(MyVec2(-0.5f, -0.5f), MyVec2(0, 1));
	vertices[2] = Pos2TexVertex(MyVec2(+0.5f, -0.5f), MyVec2(1, 1));
	vertices[3] = Pos2TexVertex(MyVec2(+0.5f, +0.5f), MyVec2(1, 0));

	UIntArray indices;
	indices.resize(6);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	m_squad.init(vertices, indices, shader, nullptr, MyVec3(0), MyVec3(0), MyVec3(1));
}

void SpriteBatch::resize(int width, int height)
{
	m_scrWidth = width;
	m_scrHeight = height;

	m_camera2D.resize(width, height);
}

void SpriteBatch::update()
{
	m_camera2D.update();
}

MyVec2 SpriteBatch::convertToWorld(const MyVec2& screenPos)
{
	return MyVec2(screenPos.x - (float)m_scrWidth / 2, (float)m_scrHeight / 2 - screenPos.y);
}

void SpriteBatch::renderTexture2D(Texture& texture, const MyVec2& pos, float rot, const MyVec2& scale)
{
	Rect2D dest = 
	{
		pos,
		MyVec2(scale.x * texture.getWidth(), scale.y * texture.getHeight())
	};

	renderTexture2D(texture, dest, rot);
}

void SpriteBatch::renderTexture2D(Texture& texture, const Rect2D& dest, float rot)
{
	MyVec2 pos2 = convertToWorld(dest.Pos);
	MyVec3 pos3 = MyVec3(pos2.x + dest.Size.x / 2.0f, pos2.y - dest.Size.y / 2.0f, 0);
	MyVec3 rot3 = MyVec3(0, 0, -rot);
	MyVec3 scale3 = MyVec3(dest.Size.x, dest.Size.y, 1);

	m_squad.setPos(pos3);
	m_squad.setRot(rot3);
	m_squad.setScale(scale3);
	m_squad.setDiffuseMap(&texture);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	m_squad.render(m_camera2D);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}