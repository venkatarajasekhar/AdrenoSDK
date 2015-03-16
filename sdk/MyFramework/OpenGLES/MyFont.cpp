
#include "MyFont.h"

Font::Font()
	: m_pGlyphsGLES(nullptr),
	m_nGlyphHeight(0),
	m_scale(1.0f, 1.0f)
{
}

Font::~Font()
{
	m_ResourceGLES.Destroy();
}

void Font::init(const MyString& filename)
{
	// Load the packed resources
	throwIfFailed((TRUE == m_ResourceGLES.LoadFromFile(filename.c_str())), "Can't load resource: " + filename);

	// Access the font data
	FONT_FILE_HEADER* pFontData = (FONT_FILE_HEADER*)m_ResourceGLES.GetData("FontData");
	throwIfFailed((pFontData != nullptr), filename + ": Font resource is invalid");

	// Verify the font data is valid
	throwIfFailed((pFontData->nMagicID == FONT_FILE_MAGIC_ID), filename + ": Font resource is invalid");

	// Extract the glyphs
	m_nGlyphHeight = pFontData->nGlyphHeight;
	m_pGlyphsGLES = pFontData->pGlyphs;

	// Create the font texture
	m_texture.init(m_ResourceGLES.GetTexture("FontTexture"));
}

// Getter
float Font::getTextWidth(const MyString& text)
{
	INT32 sx = 0;

	for (auto i = text.begin(); i != text.end(); ++i)
	{
		BYTE ch = (*i);

		if (ch == '\n')
			break;

		FONT_GLYPH* pGlyph = &m_pGlyphsGLES[ch];
		sx += pGlyph->nOffset;
		sx += pGlyph->nAdvance;
	}

	return m_scale.x * sx;
}

float Font::getTextHeight()
{
	return m_scale.y * m_nGlyphHeight;
}

MyVec2 Font::getScale()
{
	return m_scale;
}

Font::FONT_GLYPH* Font::getGlyph(char ch)
{
	return &m_pGlyphsGLES[ch];
}

Texture& Font::getTexture()
{
	return m_texture;
}

// Setter
void Font::setScale(const MyVec2& scale)
{
	m_scale = scale;
}