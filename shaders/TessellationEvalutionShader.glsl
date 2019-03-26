#version 430

layout( quads ) in;

in PipelineData {
    vec4 position;
} te_in[];

out PipelineData {
    vec4 position;
} te_out;

uniform mat4 modelToProjectionMatrix;

void basisFunctions(out float[4] b, out float[4] db, float t){
	float t1 = (1.0 - t);
	float t12 = t1 * t1;

	b[0] = t12 * t1;    
	b[1] = 3.0 * t12 * t;    
	b[2] = 3.0 * t1 * t * t;    
	b[3] = t * t * t;

	db[0] = -3.0 * t1 * t1;    
	db[1] = -6.0 * t * t1 + 3.0 * t12;   
	db[2] = -3.0 * t * t + 6.0 * t * t1;    
	db[3] = 3.0 * t * t;
}

void main()
{
	//Vertex Position Interpolation & Calculation
	vec4 VertexTempLerp1 = mix(te_in[1].position, te_in[0].position, gl_TessCoord.x);
	vec4 VertexTempLerp2 = mix(te_in[2].position, te_in[3].position, gl_TessCoord.x);
	vec4 VertexLerp = mix(VertexTempLerp1, VertexTempLerp2, gl_TessCoord.y);

	te_out.position =VertexLerp;
	vec4 VertexLerpScreenSpace = modelToProjectionMatrix * te_out.position;
	gl_Position = VertexLerpScreenSpace;
}