
varying vec2 v_texC;

uniform sampler2D u_diffuseSampler;
uniform vec4 u_color;

void main()
{
	vec2 texC = vec2(v_texC.x, 1.0 - v_texC.y);

	gl_FragColor = u_color * texture2D( u_diffuseSampler, texC );
}