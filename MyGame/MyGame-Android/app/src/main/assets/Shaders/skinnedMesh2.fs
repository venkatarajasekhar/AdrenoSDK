
// Shader for skinned mesh *.mesh

//===========================================================================================================
//
// Varying variables
//
//===========================================================================================================

varying vec4 v_lightingIntensity;
varying vec2 v_texC;

//===========================================================================================================
//
// Uniform variables
//
//===========================================================================================================

uniform sampler2D u_diffuseSampler;

//===========================================================================================================
//
// Fragment shader
//
//===========================================================================================================

void main()
{
    gl_FragColor.rgb = v_lightingIntensity.rgb * texture2D( u_diffuseSampler, v_texC ).rgb, 
    gl_FragColor.a   = v_lightingIntensity.a;
}