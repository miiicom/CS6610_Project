#version 430

layout( vertices=4 ) out;
uniform int TessLevel;

in PipelineData {
    vec4 position;
	vec3 normal;
	vec2 fragmentUV;
} tc_in[];

out PipelineData {
    vec4 position;
	vec3 normal;
	vec2 fragmentUV;
} tc_out[];

void main()
{
	tc_out[gl_InvocationID].position = tc_in[gl_InvocationID].position;
	tc_out[gl_InvocationID].normal = tc_in[gl_InvocationID].normal;
	tc_out[gl_InvocationID].fragmentUV = tc_in[gl_InvocationID].fragmentUV;

	if (gl_InvocationID == 0){
		gl_TessLevelOuter[0] = float(TessLevel);
		gl_TessLevelOuter[1] = float(TessLevel);
		gl_TessLevelOuter[2] = float(TessLevel);
		gl_TessLevelOuter[3] = float(TessLevel);

		gl_TessLevelInner[0] = float(TessLevel);
		gl_TessLevelInner[1] = float(TessLevel);
	}
}