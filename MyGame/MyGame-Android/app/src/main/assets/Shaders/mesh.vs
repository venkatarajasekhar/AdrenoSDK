
//==================================================================================================
//
// Vertex attributes
//
//==================================================================================================

attribute vec4 a_posL;
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

uniform mat4 u_world;
uniform mat4 u_view;
uniform mat4 u_proj;

uniform vec3 u_eyePos;

//==================================================================================================
//
// Vertex shader
//
//==================================================================================================

void main()
{
	// Output clip-space position
	mat4 wvp = u_proj * u_view * u_world;
	
	gl_Position = wvp * a_posL;
	
	// Pass varying variables
    vec4 posW = u_world * a_posL;
	
	v_norW = (u_world * vec4( a_norL, 0.0 )).xyz;
    v_texC = a_texC;
	v_surfaceToEye = u_eyePos - posW.xyz;
}