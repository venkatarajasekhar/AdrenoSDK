
attribute vec2 a_posL;
attribute vec2 a_texC;

varying vec2 v_texC;

uniform mat4 u_world;
uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
	mat4 wvp = u_proj * u_view * u_world;
    gl_Position = wvp * vec4( a_posL, 0.0, 1.0 );
    v_texC = a_texC;
}