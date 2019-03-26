#version 430

layout( quads ) in;

in PipelineData {
    vec4 position;
} te_in[];

out PipelineData {
    vec4 position;
} te_out;

uniform mat4 modelToProjectionMatrix;

void main()
{
	//Vertex Position Interpolation & Calculation
	vec4 VertexTempLerp1 = mix(te_in[0].position, te_in[1].position, gl_TessCoord.x);
	vec4 VertexTempLerp2 = mix(te_in[2].position, te_in[3].position, gl_TessCoord.x);
	vec4 VertexLerp = mix(VertexTempLerp1,VertexTempLerp2,gl_TessCoord.y);

	te_out.position = VertexLerp;
	vec4 VertexLerpScreenSpace = modelToProjectionMatrix * te_out.position;
	gl_Position = VertexLerpScreenSpace;
}