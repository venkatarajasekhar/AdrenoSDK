
#include "MyShader.h"

//===========================================================================================================
//
// Helpers
//
//===========================================================================================================

static FRM_SHADER_ATTRIBUTE* convert(
	const SHADER_ATTRIBUTE_DESC* shaderAttribsDesc,
	UINT32 numShaderAttribsDesc)
{
	FRM_SHADER_ATTRIBUTE* result = new FRM_SHADER_ATTRIBUTE[numShaderAttribsDesc];
	for (size_t i = 0; i < numShaderAttribsDesc; i++)
	{
		result[i].strName = shaderAttribsDesc[i].Semantic;
		result[i].nLocation = shaderAttribsDesc[i].Location;
	}
	return result;
}

//===========================================================================================================
//
// Init
//
//===========================================================================================================

Shader::Shader()
	: m_program(0)
{
}

Shader::~Shader()
{
	glDeleteProgram(m_program);
	m_program = 0;
}

void Shader::init(
	const MyString& vsFilename,
	const MyString& fsFilename,
	const SHADER_ATTRIBUTE_DESC* shaderAttribsDesc,
	UINT32 numShaderAttribsDesc)
{
	m_vsFilename = vsFilename;
	m_fsFilename = fsFilename;

	FRM_SHADER_ATTRIBUTE* descs = convert(shaderAttribsDesc, numShaderAttribsDesc);

	MyString message = "ERROR: Fail to compiling shader: " + vsFilename;
	throwIfFailed(TRUE == FrmCompileShaderProgramFromFile(vsFilename.c_str(), fsFilename.c_str(), &m_program, descs, numShaderAttribsDesc), message);

	SAFE_DELETE_ARRAY(descs);
}

//===========================================================================================================
//
// Getter
//
//===========================================================================================================

GLint Shader::getUniformLocation(const MyString& name)
{
	// Find uniform variable in cache
	auto ite = m_uniforms.find(name);

	if (ite != m_uniforms.end())
	{
		// Previously cached
		return ite->second;
	}
	else
	{
		// Newly uniform variable

		// Get location
		GLint loc = glGetUniformLocation(m_program, name.c_str());

		if (loc == -1)
		{
			smartLog("Uniform " + name + " in " + m_fsFilename + ": Not found or Unused.");
		}

		// Cache uniform variable
		m_uniforms[name] = loc;

		return loc;
	}
}

//===========================================================================================================
//
// Set uniforms
//
//===========================================================================================================

void Shader::setUniform(const MyString& name, const MyMat4& value)
{
	GLint loc = getUniformLocation(name);
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, GL_FALSE, getRawData(value));
	}
}

void Shader::setUniform(const MyString& name, const MyMat3& value)
{
	GLint loc = getUniformLocation(name);
	if (loc != -1)
	{
		glUniformMatrix3fv(loc, 1, GL_FALSE, getRawData(value));
	}
}

void Shader::setUniform(const MyString& name, const MyVec4& value)
{
	GLint loc = getUniformLocation(name);
	if (loc != -1)
	{
		glUniform4fv(loc, 1, getRawData(value));
	}
}

void Shader::setUniform(const MyString& name, const MyVec3& value)
{
	GLint loc = getUniformLocation(name);
	if (loc != -1)
	{
		glUniform3fv(loc, 1, getRawData(value));
	}
}

void Shader::setUniform(const MyString& name, const MyVec2& value)
{
	GLint loc = getUniformLocation(name);
	if (loc != -1)
	{
		glUniform2fv(loc, 1, getRawData(value));
	}
}

void Shader::setUniform(const MyString& name, GLfloat value)
{
	GLint loc = getUniformLocation(name);
	if (loc != -1)
	{
		glUniform1f(loc, value);
	}
}

void Shader::setUniform(const MyString& name, GLint value)
{
	GLint loc = getUniformLocation(name);
	if (loc != -1)
	{
		glUniform1i(loc, value);
	}
}

void Shader::setUniform(const MyString& name, GLboolean value)
{
	setUniform(name, (GLint)value);
}

/*
*
* Examples:
*     - float a[100]: count = 100; size = 1;
*     - vec2  a[100]: count = 100; size = 2;
*     - vec3  a[100]: count = 100; size = 3;
*     - vec4  a[100]: count = 100; size = 4;
*
*/
void Shader::setUniform(const MyString& name, const GLfloat* v, GLsizei count, GLsizei size)
{
	GLint loc = getUniformLocation(name);
	if (loc != -1)
	{
		switch (size)
		{
		case 1:
			glUniform1fv(loc, count, v);
			break;
		case 2:
			glUniform2fv(loc, count, v);
			break;
		case 3:
			glUniform3fv(loc, count, v);
			break;
		case 4:
			glUniform4fv(loc, count, v);
			break;
		default:
			break;
		}
	}
}

//===========================================================================================================
//
// Misc
//
//===========================================================================================================

void Shader::apply()const
{
	glUseProgram(m_program);
}