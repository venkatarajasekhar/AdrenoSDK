
attribute vec4 a_posL;
attribute vec3 a_norL;
attribute vec2 a_texC;

varying vec3 v_norW;
varying vec2 v_texC;

uniform mat4 u_world;
uniform mat4 u_view;
uniform mat4 u_proj;

uniform float u_timer;
//uniform vec3 u_distortModifier;

void main()
{
	// Output clip-space position
	mat4 wvp = u_proj * u_view * u_world;
	
	vec3 u_distortModifier = vec3(1,0,2);
	//vec4 posL = a_posL + a_norL *5*sin (u_timer *a_texC.x); // 3
	//vec4 posL = a_posL + vec4(a_norL, 0) *2*sin (u_timer *a_texC.x); // 1
	vec4 posL = a_posL + vec4(a_norL, 0) * sin (u_timer*a_texC.x*u_distortModifier.x + u_timer*a_texC.y*u_distortModifier.y)*u_distortModifier.z;
	
	gl_Position = wvp * posL;
	
	// Pass normal vector
	//v_norW = (u_world * vec4( a_norL, 0.0 )).xyz;
	
	// Pass texture coordinate
	v_texC = a_texC;
}