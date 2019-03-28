#version 430

in layout(location=0) vec4 vertexPositionModel; // shade will nicely change vec3 to vec4 for me
in layout(location=1) vec3 vertexNormalModel;
in layout(location=2) vec3 vertexUVModel;

uniform mat4 modelToProjectionMatrix;

out PipelineData {
    vec4 position;
	vec3 normal;
	vec2 fragmentUV;
} vs_out;

void main()
{
	//gl_Position = modelToProjectionMatrix * vertexPositionModel;
	vs_out.position =  vertexPositionModel;
	vs_out.normal =  vertexNormalModel;
	vs_out.fragmentUV = vec2(vertexUVModel.x,vertexUVModel.y);
}