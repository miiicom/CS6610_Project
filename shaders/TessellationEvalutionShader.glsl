#version 430

layout( quads ) in;

in PipelineData {
    vec4 position;
	vec3 normal;
	vec2 fragmentUV;
} te_in[];

out PipelineData {
    vec4 position;
	vec3 normal;
	vec2 fragmentUV;
} te_out;

uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldTransMatrix;

void main()
{
	//Vertex Position 
	vec4 VertexTempLerp1 = mix(te_in[0].position, te_in[1].position, gl_TessCoord.x);
	vec4 VertexTempLerp2 = mix(te_in[2].position, te_in[3].position, gl_TessCoord.x);
	vec4 VertexPositionLerp = mix(VertexTempLerp1,VertexTempLerp2,gl_TessCoord.y);

	//Vertex Normal 
	vec3 VertexTempLerp3 = mix(te_in[0].normal, te_in[1].normal, gl_TessCoord.x);
	vec3 VertexTempLerp4 = mix(te_in[2].normal, te_in[3].normal, gl_TessCoord.x);
	vec3 VertexNormalLerp = mix(VertexTempLerp3,VertexTempLerp4,gl_TessCoord.y);

	//Vertex fragmentUV 
	vec2 VertexTempLerp5 = mix(te_in[0].fragmentUV, te_in[1].fragmentUV, gl_TessCoord.x);
	vec2 VertexTempLerp6 = mix(te_in[2].fragmentUV, te_in[3].fragmentUV, gl_TessCoord.x);
	vec2 VertexUVLerp = mix(VertexTempLerp5,VertexTempLerp6,gl_TessCoord.y);

	te_out.position = modelToProjectionMatrix * VertexPositionLerp;
	te_out.normal = vec3(modelToWorldTransMatrix * vec4(VertexNormalLerp,0)); 
	te_out.fragmentUV = VertexUVLerp;
	vec4 VertexLerpScreenSpace = te_out.position;
	gl_Position = VertexLerpScreenSpace;
}