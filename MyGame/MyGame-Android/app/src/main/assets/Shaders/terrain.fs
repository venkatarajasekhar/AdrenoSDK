
varying vec2 v_texC;

uniform sampler2D u_diffuseSampler;

void main()
{
	gl_FragColor = texture2D( u_diffuseSampler, v_texC );
}