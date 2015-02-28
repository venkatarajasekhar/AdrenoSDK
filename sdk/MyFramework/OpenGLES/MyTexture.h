
#pragma once

#include "FrmResourceGLES.h"

class Texture
{
public:
	Texture();
	~Texture();

	void init(CFrmTexture* frmTexture);

	int bind();

	int getWidth();
	int getHeight();

private:
	CFrmTexture* m_frmTexture;
	int m_textureSlot;

	static int s_textureSlot;
	static int s_maxTextureSlot;
};