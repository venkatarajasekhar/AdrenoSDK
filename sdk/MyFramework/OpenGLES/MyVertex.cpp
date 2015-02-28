
#include "MyVertex.h"
#include <FrmResourceGLES.h>

static const char* POSITION_SEMANTIC = "a_posL";
static const char* COLOR_SEMANTIC = "a_color";
static const char* BONE_INDICES_SEMANTIC = "a_boneIndices";
static const char* BONE_WEIGHTS_SEMANTIC = "a_boneWeights";
static const char* NORMAL_SEMANTIC = "a_norL";
static const char* TEXTURE_SEMANTIC = "a_texC";

const SHADER_ATTRIBUTE_DESC PosColorVertex::ShaderAttribsDesc[] =
{
	{ POSITION_SEMANTIC, FRM_VERTEX_POSITION, 3, GL_FLOAT, sizeof(PosColorVertex), 0 },
	{ COLOR_SEMANTIC,    FRM_VERTEX_COLOR0,   3, GL_FLOAT, sizeof(PosColorVertex), 3 * sizeof(GLfloat) },
};

const SHADER_ATTRIBUTE_DESC PosTexVertex::ShaderAttribsDesc[] =
{
	{ POSITION_SEMANTIC, FRM_VERTEX_POSITION,  3, GL_FLOAT, sizeof(PosTexVertex), 0 },
	{ TEXTURE_SEMANTIC,  FRM_VERTEX_TEXCOORD0, 2, GL_FLOAT, sizeof(PosTexVertex), 3 * sizeof(GLfloat) },
};

const SHADER_ATTRIBUTE_DESC PosNorTexVertex::ShaderAttribsDesc[] =
{
	{ POSITION_SEMANTIC, FRM_VERTEX_POSITION,  3, GL_FLOAT, sizeof(PosNorTexVertex), 0 },
	{ NORMAL_SEMANTIC,   FRM_VERTEX_NORMAL,    3, GL_FLOAT, sizeof(PosNorTexVertex), 3 * sizeof(GLfloat) },
	{ TEXTURE_SEMANTIC,  FRM_VERTEX_TEXCOORD0, 2, GL_FLOAT, sizeof(PosNorTexVertex), 6 * sizeof(GLfloat) },
};

const SHADER_ATTRIBUTE_DESC Pos2TexVertex::ShaderAttribsDesc[] =
{
	{ POSITION_SEMANTIC, FRM_VERTEX_POSITION,  2, GL_FLOAT, sizeof(Pos2TexVertex), 0 },
	{ TEXTURE_SEMANTIC,  FRM_VERTEX_TEXCOORD0, 2, GL_FLOAT, sizeof(Pos2TexVertex), 2 * sizeof(GLfloat) },
};

const SHADER_ATTRIBUTE_DESC SkinnedVertex::ShaderAttribsDesc[] =
{
	{ POSITION_SEMANTIC,     FRM_VERTEX_POSITION,    0, GL_FLOAT, sizeof(SkinnedVertex), 0 },
	{ BONE_INDICES_SEMANTIC, FRM_VERTEX_BONEINDICES, 0, GL_FLOAT, sizeof(SkinnedVertex), 0 },
	{ BONE_WEIGHTS_SEMANTIC, FRM_VERTEX_BONEWEIGHTS, 0, GL_FLOAT, sizeof(SkinnedVertex), 0 },
	{ NORMAL_SEMANTIC,       FRM_VERTEX_NORMAL,      0, GL_FLOAT, sizeof(SkinnedVertex), 0 },
	{ TEXTURE_SEMANTIC,      FRM_VERTEX_TEXCOORD0,   0, GL_FLOAT, sizeof(SkinnedVertex), 0 },
};