
const vec3 LIGHT_DIR = vec3(-0.4, -1, -0.7);
const vec4 AMBIENT_COLOR = vec4(0.2, 0.2, 0.2, 1);
const vec4 DIFFUSE_COLOR = vec4(0.5, 0.5, 0, 1);

varying vec3 v_norW;
varying vec2 v_texC;

uniform sampler2D u_diffuseSampler;

void main()
{
	//vec3 norW = normalize(v_norW);
	//vec3 lightDir = -normalize(LIGHT_DIR);
	
	// Compute diffuse color
	vec4 diffColor = texture2D( u_diffuseSampler, v_texC );
	
	// Compute lighting
	//float diffFactor = max( 0.0, dot( norW, lightDir ) );
	
	// Output color
	//gl_FragColor.rgba = AMBIENT_COLOR;
	//gl_FragColor.rgba += diffColor * diffFactor;
	//gl_FragColor.a = diffColor.a;
	gl_FragColor = diffColor;
}