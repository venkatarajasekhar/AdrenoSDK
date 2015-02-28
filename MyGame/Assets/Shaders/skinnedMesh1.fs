
// Shader for skinned mesh *.model

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

//==================================================================================================
//
// Varying variables
//
//==================================================================================================

varying vec3 v_norW;
varying vec2 v_texC;

varying vec3 v_surfaceToLight;
varying vec3 v_surfaceToEye;

//==================================================================================================
//
// Uniform variables
//
//==================================================================================================

uniform sampler2D u_diffuseSampler;
uniform MATERIAL u_material;

//==================================================================================================
//
// Fragment shader
//
//==================================================================================================

void main()
{
	vec3 norW = normalize(v_norW);
	vec3 surfaceToLight = normalize( v_surfaceToLight );
    vec3 surfaceToEye = normalize( v_surfaceToEye );
	
	// Get texture color
	vec4 diffTexture = texture2D( u_diffuseSampler, v_texC );
	
	// Apply ambient color
    gl_FragColor.rgb = u_material.Ambient;
    gl_FragColor.a = 0.0;
    
	// Apply diffuse color
    float diffFactor = max( 0.0, dot( norW, surfaceToLight ) );
    gl_FragColor.rgba += diffTexture * u_material.Diffuse * diffFactor;
    
    // Apply specular color
    vec3 halfVector = normalize( surfaceToLight + surfaceToEye );
    float specFactor = pow( max( 0.0, dot( norW, halfVector ) ), u_material.Shininess );
    gl_FragColor.rgba += u_material.Specular * specFactor;
}