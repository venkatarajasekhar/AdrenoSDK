
attribute vec4 a_posL;
attribute vec3 a_norL;
attribute vec2 a_texC;

varying vec3 v_norW;
varying vec2 v_texC;

uniform mat4 u_world;
uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
	// Output clip-space position
	mat4 wvp = u_proj * u_view * u_world;
	
	gl_Position = wvp * a_posL;
	
	// Pass normal vector
	v_norW = (u_world * vec4( a_norL, 0.0 )).xyz;
	
	// Pass texture coordinate
	v_texC = a_texC;
}