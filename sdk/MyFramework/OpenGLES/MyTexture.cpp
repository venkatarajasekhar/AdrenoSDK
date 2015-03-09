
#include "MyTexture.h"

int Texture::s_textureSlot = 0;
int Texture::s_maxTextureSlot = -1;

Texture::Texture()
	: m_frmTexture(nullptr)
{
	// Determine maximum of number of texture units
	if (s_maxTextureSlot < 0)
	{
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &s_maxTextureSlot);
	}

	// Determine texture unit for this texture
	m_textureSlot = s_textureSlot++;
	if (s_textureSlot >= s_maxTextureSlot)
	{
		s_textureSlot = 0;
	}
}

Texture::~Texture()
{
	if (m_frmTexture) m_frmTexture->Release();
}

void Texture::init(CFrmTexture* frmTexture)
{
	m_frmTexture = frmTexture;
}

int Texture::bind()
{
	m_frmTexture->Bind(m_textureSlot);
	return m_textureSlot;
}

int Texture::getWidth()
{
	return m_frmTexture->m_nWidth;
}

int Texture::getHeight()
{
	return m_frmTexture->m_nHeight;
}