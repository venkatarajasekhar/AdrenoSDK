
// Shader for skinned mesh *.model

//==================================================================================================
//
// Constants
//
//==================================================================================================

const int MAX_BONES = 50;

//==================================================================================================
//
// Vertex attributes
//
//==================================================================================================

attribute vec4 a_posL;
attribute float a_boneIndices;
#if defined( USE_TWO_BONES ) || defined( USE_THREE_BONES )
attribute float a_boneIndices1;
#endif
#if defined( USE_THREE_BONES )
attribute float a_boneIndices2;
#endif
attribute vec3 a_boneWeights;
attribute vec3 a_norL;
attribute vec2 a_texC;

//==================================================================================================
//
// Varying variables
//
//==================================================================================================

varying vec3 v_norW;
varying vec2 v_texC;

varying vec3 v_surfaceToEye;

//==================================================================================================
//
// Uniform variables
//
//==================================================================================================

uniform vec4 u_worldArray[MAX_BONES * 3];

uniform mat4 u_world;
uniform mat4 u_view;
uniform mat4 u_proj;

uniform vec3 u_eyePos;

//==================================================================================================
//
// Helpers
//
//==================================================================================================

vec3 MulBone3( vec3 inputVector, int boneIndex, float weight )
{
	vec3 result;
	
	int matrixIndex = 3 * boneIndex;
	
    result.x = dot( inputVector, u_worldArray[matrixIndex + 0].xyz );
    result.y = dot( inputVector, u_worldArray[matrixIndex + 1].xyz );
    result.z = dot( inputVector, u_worldArray[matrixIndex + 2].xyz );
    
    return result * weight;
}

vec3 MulBone4( vec4 inputVector, int boneIndex, float weight )
{
    vec3 result;
    
    int matrixIndex = 3 * boneIndex;
    
    result.x = dot( inputVector, u_worldArray[matrixIndex + 0].xyzw );
    result.y = dot( inputVector, u_worldArray[matrixIndex + 1].xyzw );
    result.z = dot( inputVector, u_worldArray[matrixIndex + 2].xyzw );
    
    return result * weight;
}

//==================================================================================================
//
// Vertex shader
//
//==================================================================================================

void main()
{
	// Get bone indices
    ivec3 boneIndices;
    boneIndices.x = int( a_boneIndices );
#if defined( USE_TWO_BONES ) || defined( USE_THREE_BONES )
    boneIndices.y = int( a_boneIndices1 );
#endif
#if defined( USE_THREE_BONES )
    boneIndices.z = int( a_boneIndices2 );
#endif

	// Transform position
    vec3 posL = MulBone4( a_posL, boneIndices.x, a_boneWeights.x );
#if defined( USE_TWO_BONES ) || defined( USE_THREE_BONES )
    posL += MulBone4( a_posL, boneIndices.y, a_boneWeights.y );
#endif
#if defined( USE_THREE_BONES )
    posL += MulBone4( a_posL, boneIndices.z, a_boneWeights.z );
#endif
    
    // Transform normal
    vec3 norL = MulBone3( a_norL, boneIndices.x, a_boneWeights.x );
#if defined( USE_TWO_BONES ) || defined( USE_THREE_BONES )
    norL += MulBone3( a_norL, boneIndices.y, a_boneWeights.y );
#endif
#if defined( USE_THREE_BONES )
    norL += MulBone3( a_norL, boneIndices.z, a_boneWeights.z );
#endif
	
	// Output clip-space position
	mat4 wvp = u_proj * u_view * u_world;
	
    gl_Position = wvp * vec4( posL, 1.0 );
    
    // Pass varying variables
	vec4 posW = u_world * vec4( posL, 1.0 );
	
	v_norW = (u_world * vec4( norL, 0.0 )).xyz;
    v_texC = a_texC;
	v_surfaceToEye = u_eyePos - posW.xyz;
}