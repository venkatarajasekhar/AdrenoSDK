
#pragma once

#include "FrmResourceGLES.h"
#include "MyUtils.h"

class Texture
{
public:
	Texture();
	virtual ~Texture();

	virtual void init(CFrmTexture* frmTexture);

	virtual int bind();

	// Getter

	virtual int getWidth();
	virtual int getHeight();
	MyMat4& getTexMat();

	// Setter

	void setSrcTex(const Rect2D& src);

protected:
	CFrmTexture* m_frmTexture;
	int m_textureSlot;

	MyMat4 m_texMat;

protected:
	static int s_textureSlot;
	static int s_maxTextureSlot;
};