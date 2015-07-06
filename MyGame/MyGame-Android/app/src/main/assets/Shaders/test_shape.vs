
attribute vec4 a_posL;
attribute vec3 a_norL;
attribute vec2 a_texC;

varying vec3 v_norW;
varying vec2 v_texC;

uniform mat4 u_world;
uniform mat4 u_view;
uniform mat4 u_proj;

uniform mat4 u_texMat;

void main()
{
	// Output clip-space position
	mat4 wvp = u_proj * u_view * u_world;
	
	gl_Position = wvp * a_posL;
	
	// Pass normal vector
	v_norW = (u_world * vec4( a_norL, 0.0 )).xyz;
	
	// Pass texture coordinate
	v_texC = (u_texMat * vec4(a_texC, 0.0, 1.0)).xy;
}