
const vec3 LIGHT_DIR = vec3(-0.4, -1, -0.7);
const vec4 AMBIENT_COLOR = vec4(0.2, 0.2, 0.2, 1);
const vec4 DIFFUSE_COLOR = vec4(0.5, 0.5, 0, 1);

varying vec3 v_norW;
varying vec2 v_texC;

void main()
{
	vec3 norW = normalize(v_norW);
	vec3 lightDir = -normalize(LIGHT_DIR);
	
	float diffFactor = max( 0.0, dot( norW, lightDir ) );
	
	gl_FragColor.rgba = AMBIENT_COLOR;
	gl_FragColor.rgba += DIFFUSE_COLOR * diffFactor;
}