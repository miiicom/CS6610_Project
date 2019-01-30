#version 430

in layout(location=0) vec4 vertexPositionModel; // shade will nicely change vec3 to vec4 for me
in layout(location=1) vec3 vertexNormalModel;

uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldTransMatrix;

out vec3 NormalWorld;
out vec3 VertexPositionWorld;

void main()
{
	gl_Position = modelToProjectionMatrix * vertexPositionModel;
	NormalWorld = vec3(modelToWorldTransMatrix * vec4(vertexNormalModel,0));
	VertexPositionWorld = vec3(modelToWorldTransMatrix * vertexPositionModel);
}