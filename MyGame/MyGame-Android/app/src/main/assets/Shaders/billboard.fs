
uniform sampler2D u_diffuseSampler;

varying vec2 v_texC;

void main()
{
	vec2 texC = vec2(v_texC.x, 1.0 - v_texC.y);
	gl_FragColor = texture2D( u_diffuseSampler, texC );
}