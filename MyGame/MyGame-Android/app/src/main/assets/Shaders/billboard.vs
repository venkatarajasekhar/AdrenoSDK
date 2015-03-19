
attribute vec2 a_texC;

varying vec2 v_texC;

uniform mat4 u_world;
uniform mat4 u_view;
uniform mat4 u_proj;

uniform mat4 u_texMat;

uniform vec2 u_billboardSize;
uniform vec3 u_eye;

void main()
{
	vec2 cornerIndex = -a_texC * 2.0 + 1.0;
	vec3 center = u_world[3].xyz;
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec2 halfSize = u_billboardSize / 2.0;
	
	vec3 CEv = u_eye - center;
	vec3 CKv = normalize( cross(CEv, up) );
	vec3 KMv = normalize( cross(CKv, CEv) );
	
	vec3 posW = halfSize.x * cornerIndex.x * CKv + halfSize.y * cornerIndex.y * KMv;

	mat4 wvp = u_proj * u_view * u_world;
    gl_Position = wvp * vec4( posW, 1.0 );
	
    v_texC = (u_texMat * vec4(a_texC, 0.0, 1.0)).xy;
}