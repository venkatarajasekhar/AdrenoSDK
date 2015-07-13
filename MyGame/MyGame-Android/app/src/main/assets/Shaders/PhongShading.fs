
//==================================================================================================
//
// Declaring structs
//
//==================================================================================================

struct MATERIAL
{
    vec3 Ambient;
    vec4 Diffuse;
    vec4 Specular;
    float Shininess;
};

struct LIGHT
{
	// w == 0.0f: Directional light
	// w == 1.0f: Point light
    vec4 PosOrDir;
};

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

uniform sampler2D u_diffuseSampler;

uniform MATERIAL u_material;
uniform LIGHT u_light;

//==================================================================================================
//
// Fragment shader
//
//==================================================================================================

void main()
{
	vec3 norW = normalize(v_norW);
	vec3 lightDir = -normalize(u_light.PosOrDir.xyz);
	vec3 surfaceToEye = normalize( v_surfaceToEye );

	// Get texture color
	vec4 diffTexture = texture2D( u_diffuseSampler, v_texC );
	
	// Apply ambient color
	gl_FragColor.rgb = u_material.Ambient;
	gl_FragColor.a = 0.0;
    
	// Apply diffuse color
	float diffFactor = max( 0.0, dot( norW, lightDir ) );
	//gl_FragColor.rgba += diffTexture * u_material.Diffuse * diffFactor;
	gl_FragColor.rgba += diffTexture  * (diffFactor * u_material.Diffuse.x + u_material.Diffuse.y);
	
	// Apply specular color
	vec3 halfVector = normalize( lightDir + surfaceToEye );
	float specFactor = pow( max( 0.0, dot( norW, halfVector ) ), u_material.Shininess );
	gl_FragColor.rgba += u_material.Specular * specFactor;
}