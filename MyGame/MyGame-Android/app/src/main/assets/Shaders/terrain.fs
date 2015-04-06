
struct TerrainProperties
{
	vec3 TileRepeatFactor;
	vec2 Threshold;
};

varying vec2 v_texC;

uniform sampler2D u_diffuseSampler1;
uniform sampler2D u_diffuseSampler2;
uniform sampler2D u_blendSampler;

uniform TerrainProperties u_properties;

void main()
{
	vec2 texC = vec2(v_texC.x, 1.0 - v_texC.y);

	vec4 color1 = texture2D( u_diffuseSampler1, texC * u_properties.TileRepeatFactor.x );
	vec4 color2 = texture2D( u_diffuseSampler2, texC * u_properties.TileRepeatFactor.y );
	vec4 blend = texture2D( u_blendSampler, texC * u_properties.TileRepeatFactor.z );
	
	if (blend.x < u_properties.Threshold.x)
	{
		gl_FragColor = color1;
	}
	else if (blend.x > u_properties.Threshold.y)
	{
		gl_FragColor = color2;
	}
	else
	{
		gl_FragColor = mix(color1, color2, blend.x);
	}
}