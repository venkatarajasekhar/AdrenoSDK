
#pragma once

#include "MyUtils.h"
#include "MyTexture.h"
#include "FrmPackedResourceGLES.h"

class Font
{
public:
	struct FONT_GLYPH
	{
		UINT16 u0, v0;
		UINT16 u1, v1;
		INT16  nOffset;
		INT16  nWidth;
		INT16  nAdvance;
	};

	struct FONT_FILE_HEADER
	{
		UINT32 nMagicID;
		UINT32 nGlyphHeight;
		FONT_GLYPH pGlyphs[1];
	};

public:
	Font();
	~Font();

	void init(const MyString& filename);

	// Getter
	float getTextWidth(const MyString& text);
	float getTextHeight();

	MyVec2 getScale();
	FONT_GLYPH* getGlyph(char ch);
	Texture& getTexture();

	// Setter
	void setScale(const MyVec2& scale);

private:
	static const UINT32 FONT_FILE_MAGIC_ID = ('F' << 0) | ('O' << 8) | ('N' << 16) | ('T' << 24);

private:
	FONT_GLYPH* m_pGlyphsGLES;
	UINT32 m_nGlyphHeight;

	CFrmPackedResourceGLES m_ResourceGLES;
	Texture m_texture;

	MyVec2 m_scale;
};