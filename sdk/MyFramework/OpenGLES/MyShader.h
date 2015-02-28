
#pragma once

#include "MyUtils.h"
#include <FrmShader.h>

//======================================================================================
//
// SHADER_ATTRIBUTE_DESC struct
//
//======================================================================================

struct SHADER_ATTRIBUTE_DESC
{
	const char* Semantic;
	GLuint      Location;
	GLint       Size;
	GLenum      Type;
	GLsizei     Stride;
	GLsizei     Offset;
};

//======================================================================================
//
// Shader class
//
//======================================================================================

class Shader
{
public:
	Shader();
	~Shader();

	void init(
		const MyString& vsFilename,
		const MyString& fsFilename,
		const SHADER_ATTRIBUTE_DESC* shaderAttribsDesc,
		UINT32 numShaderAttribsDesc);

	void setUniform(const MyString& name, const MyMat4& value);
	void setUniform(const MyString& name, const MyMat3& value);
	void setUniform(const MyString& name, const MyVec4& value);
	void setUniform(const MyString& name, const MyVec3& value);
	void setUniform(const MyString& name, const MyVec2& value);
	void setUniform(const MyString& name, GLfloat value);
	void setUniform(const MyString& name, GLint value);
	void setUniform(const MyString& name, GLboolean value);
	void setUniform(const MyString& name, const GLfloat* v, GLsizei count, GLsizei size);

	void apply()const;

private:
	GLint getUniformLocation(const MyString& name);

private:
	GLuint m_program;

	MyString m_vsFilename;
	MyString m_fsFilename;

	StringIntDictionary m_uniforms;
};