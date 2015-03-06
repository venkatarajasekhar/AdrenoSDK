
// Shader for skinned mesh *.mesh

//===========================================================================================================
//
// Constants
//
//===========================================================================================================

const int MAX_BONES = 26;

//===========================================================================================================
//
// Vertex attributes
//
//===========================================================================================================

attribute vec4  a_posL;
attribute vec4  a_boneIndices;
attribute float a_boneWeights;
attribute vec3  a_norL;
attribute vec2  a_texC;

//===========================================================================================================
//
// Varying variables
//
//===========================================================================================================

varying vec3 v_norW;
varying vec2 v_texC;

varying vec3 v_surfaceToEye;

//===========================================================================================================
//
// Uniform variables
//
//===========================================================================================================

uniform   vec4     u_worldArray[3*MAX_BONES];
uniform   mat4     u_world;
uniform   mat4     u_view;
uniform   mat4     u_proj;

uniform vec3 u_eyePos;

//===========================================================================================================
//
// Helper functions for transforming a vec3/vec4 by a bone
//
//===========================================================================================================

vec3 MulBone3( vec3 pos, int index, float weight )
{
    vec3 result;
	
	int index3 = 3 * index;
	
    result.x = dot( pos, u_worldArray[index3 + 0].xyz );
    result.y = dot( pos, u_worldArray[index3 + 1].xyz );
    result.z = dot( pos, u_worldArray[index3 + 2].xyz );
	
    return result * weight;
}

vec3 MulBone4( vec4 pos, int index, float weight )
{
    vec3 result;
	
	int index3 = 3 * index;
	
    result.x = dot( pos, u_worldArray[index3 + 0].xyzw );
    result.y = dot( pos, u_worldArray[index3 + 1].xyzw );
    result.z = dot( pos, u_worldArray[index3 + 2].xyzw );
	
    return result * weight;
}

//===========================================================================================================
//
// Vertex shader
//
//===========================================================================================================

void main()
{
	// Get bone indices/ bone weights
    vec2 boneWeights;
    boneWeights.x = a_boneWeights;
    boneWeights.y = 1.0 - a_boneWeights;

    ivec4 boneIndices;
    boneIndices.x = int(a_boneIndices.x);
    boneIndices.y = int(a_boneIndices.y);

	// Transform position
    vec3 posL;
#ifdef USE_TWO_BONES
    posL  = MulBone4( a_posL, boneIndices.x, boneWeights.x );
    posL += MulBone4( a_posL, boneIndices.y, boneWeights.y );
#else
    posL  = MulBone4( a_posL, boneIndices.x, 1.0 );
#endif

	// Transform normal
    vec3 norL;
#ifdef USE_TWO_BONES
    norL  = MulBone3( a_norL, boneIndices.x, boneWeights.x );
    norL += MulBone3( a_norL, boneIndices.y, boneWeights.y ); 
#else
    norL  = MulBone3( a_norL, boneIndices.x, 1.0 );
#endif
	
    // Output clip-space position
	mat4 wvp = u_proj * u_view * u_world;
	
    gl_Position = wvp * vec4( posL.xyz, 1.0 );
	
	// Pass varying variables
	vec4 posW = u_world * vec4( posL.xyz, 1.0 );
	
	v_norW = (u_world * vec4(norL, 0.0)).xyz;
    v_texC = vec2( a_texC.x, a_texC.y );
	v_surfaceToEye = u_eyePos - posW.xyz;
}