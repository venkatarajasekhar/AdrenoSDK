
#include "MySpriteBatch.h"

SpriteBatch::SpriteBatch()
	: m_camera2D(nullptr),
	m_scrWidth(0),
	m_scrHeight(0)
{
}

SpriteBatch::~SpriteBatch()
{
}

void SpriteBatch::init(Shader& shader, Camera2D& camera)
{
	m_camera2D = &camera;

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
}

void SpriteBatch::update()
{
}

MyVec2 SpriteBatch::convertToWorld(const MyVec2& screenPos)
{
	return MyVec2(screenPos.x - (float)m_scrWidth / 2, (float)m_scrHeight / 2 - screenPos.y);
}

void SpriteBatch::renderTexture2D(Texture* texture, const MyVec2& pos, float rot, const MyVec2& scale, MyColor color)
{
	Rect2D dest = 
	{
		pos,
		MyVec2(scale.x * texture->getWidth(), scale.y * texture->getHeight())
	};

	renderTexture2D(texture, dest, nullptr, rot, color);
}

void SpriteBatch::renderTexture2D(Texture* texture, const Rect2D& dest, const Rect2D* src, float rot, MyColor color)
{
	// Compute transformation of texture mesh
	MyVec2 pos2 = convertToWorld(dest.Pos);
	MyVec3 pos3 = MyVec3(pos2.x + dest.Size.x / 2.0f, pos2.y - dest.Size.y / 2.0f, 0);
	MyVec3 rot3 = MyVec3(0, 0, -rot);
	MyVec3 scale3 = MyVec3(dest.Size.x, dest.Size.y, 1);

	m_squad.setPos(pos3);
	m_squad.setRot(rot3);
	m_squad.setScale(scale3);
	m_squad.setDiffuseMap(texture);

	// Compute transformation of texture coordinate

	if (src != nullptr) texture->setSrcTex(*src);

	Shader* shader = m_squad.getShader();
	shader->apply();
	shader->setUniform("u_texMat", texture->getTexMat());
	shader->setUniform("u_color", MyVec4(color.r, color.g, color.b, 1.0f));

	// Render texture
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	m_squad.render(*m_camera2D);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void SpriteBatch::renderText2D(Font& font, const MyString& text, const MyVec2& pos, float rot, MyColor color)
{
	FLOAT32 sx = pos.x, sy = pos.y;

	for (auto i = text.begin(); i != text.end(); ++i)
	{
		BYTE ch = (*i);

		if (ch == '\n')
		{
			sx = pos.x;
			sy += font.getTextHeight();

			continue;
		}

		Font::FONT_GLYPH* pGlyph = font.getGlyph(ch);

		sx += font.getScale().x * pGlyph->nOffset;

		FLOAT32 w = font.getScale().x * (pGlyph->u1 - pGlyph->u0);
		FLOAT32 h = font.getScale().y * (pGlyph->v1 - pGlyph->v0);

		// Draw
		Rect2D dest(MyVec2(sx, sy), MyVec2(w, h));
		Rect2D src(MyVec2(pGlyph->u0, pGlyph->v0), MyVec2(pGlyph->u1 - pGlyph->u0, pGlyph->v1 - pGlyph->v0));

		renderTexture2D(&font.getTexture(), dest, &src, rot, color);

		sx += font.getScale().x * pGlyph->nAdvance;
	}
}