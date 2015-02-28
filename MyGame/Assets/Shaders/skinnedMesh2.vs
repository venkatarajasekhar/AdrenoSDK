
// Shader for skinned mesh *.mesh

//===========================================================================================================
//
// Define some useful struct
//
//===========================================================================================================

struct MATERIAL
{
    vec3  Ambient;
    vec4  Diffuse;
    vec4  Specular;
};

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

varying   vec4  v_lightingIntensity;
varying   vec2  v_texC;

//===========================================================================================================
//
// Uniform variables
//
//===========================================================================================================

uniform   vec4     u_worldArray[3*MAX_BONES];
uniform   mat4     u_world;
uniform   mat4     u_view;
uniform   mat4     u_proj;

uniform   MATERIAL u_material;

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
    vec2 boneWeights;
    boneWeights.x = a_boneWeights;
    boneWeights.y = 1.0 - a_boneWeights;

    ivec4 boneIndices;
    boneIndices.x = int(a_boneIndices.x);
    boneIndices.y = int(a_boneIndices.y);

    vec3 currPose_PosL;
#ifdef USE_TWO_BONES
    currPose_PosL  = MulBone4( a_posL, boneIndices.x, boneWeights.x );
    currPose_PosL += MulBone4( a_posL, boneIndices.y, boneWeights.y );
#else
    currPose_PosL  = MulBone4( a_posL, boneIndices.x, 1.0 );
#endif

    vec3 currPose_NorL;
#ifdef USE_TWO_BONES
    currPose_NorL  = MulBone3( a_norL, boneIndices.x, boneWeights.x );
    currPose_NorL += MulBone3( a_norL, boneIndices.y, boneWeights.y ); 
#else
    currPose_NorL  = MulBone3( a_norL, boneIndices.x, 1.0 );
#endif
	
	vec3 currPose_NorW = (u_world * vec4(currPose_NorL, 0.0)).xyz;
    currPose_NorW = normalize( currPose_NorW );

    // Output clip-space position
	mat4 wvp = u_proj * u_view * u_world;
    gl_Position = wvp * vec4( currPose_PosL.xyz, 1.0 );

    // Compute the lighting
    vec3  lightDir   = normalize( vec3( 0.0, 0.5, 1.0 ) );
    float intensity = max( 0.0, dot( currPose_NorW, lightDir ) );

    // Combine lighting with the material properties
    vec3 ambient = u_material.Ambient;
    vec4 diffuse = u_material.Diffuse.a != 0.0 ? u_material.Diffuse.rgba : vec4(1.0);
    v_lightingIntensity.rgba  = vec4( ambient.rgb, 0.0 );
    v_lightingIntensity.rgba += diffuse.rgba * intensity;

    v_texC = vec2( a_texC.x, a_texC.y );
}