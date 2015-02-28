
#include "MyInputLayout.h"

InputLayout::InputLayout()
	: m_shaderAttribsDesc(nullptr),
	m_numShaderAttribsDesc(0)
{
}

InputLayout::~InputLayout()
{
}

void InputLayout::init(const SHADER_ATTRIBUTE_DESC* shaderAttribsDesc, int numShaderAttribsDesc)
{
	m_shaderAttribsDesc = shaderAttribsDesc;
	m_numShaderAttribsDesc = numShaderAttribsDesc;
}

void InputLayout::bind()const
{
	for (int i = 0; i < m_numShaderAttribsDesc; i++)
	{
		glEnableVertexAttribArray(m_shaderAttribsDesc[i].Location);

		glVertexAttribPointer(
			m_shaderAttribsDesc[i].Location,
			m_shaderAttribsDesc[i].Size,
			m_shaderAttribsDesc[i].Type,
			GL_FALSE,
			m_shaderAttribsDesc[i].Stride,
			(GLvoid*)(m_shaderAttribsDesc[i].Offset)
			);
	}
}

void InputLayout::unbind()const
{
	for (int i = 0; i < m_numShaderAttribsDesc; i++)
	{
		glDisableVertexAttribArray(m_shaderAttribsDesc[i].Location);
	}
}