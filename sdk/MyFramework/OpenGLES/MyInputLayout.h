
#pragma once

#include "MyShader.h"

class InputLayout
{
public:
	InputLayout();
	~InputLayout();

	void init(const SHADER_ATTRIBUTE_DESC* shaderAttribsDesc, int numShaderAttribsDesc);
	void bind()const;
	void unbind()const;

private:
	const SHADER_ATTRIBUTE_DESC* m_shaderAttribsDesc;
	int m_numShaderAttribsDesc;
};