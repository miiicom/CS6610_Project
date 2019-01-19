#version 430

in layout(location=0) vec4 vertexPositionModel; // shade will nicely change vec3 to vec4 for me
in layout(location=1) vec3 vertexColorModel;
in layout(location=2) vec3 normalModel;

uniform mat4 modelToProjectionMatrix;

out vec3 fragColor;

void main()
{
	gl_Position = modelToProjectionMatrix * vertexPositionModel;
	fragColor = vertexColorModel;
}