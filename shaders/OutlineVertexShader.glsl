#version 430

in layout(location=0) vec4 vertexPositionModel; // shade will nicely change vec3 to vec4 for me
in layout(location=1) vec3 vertexNormalModel;
in layout(location=2) vec3 vertexUVModel;

uniform mat4 modelToProjectionMatrix;

out PipelineData {
    vec4 position;
} vs_out;

void main()
{
	gl_Position = modelToProjectionMatrix * vertexPositionModel;
	vs_out.position = modelToProjectionMatrix * vertexPositionModel;

}