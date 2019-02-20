#version 430

in layout(location=0) vec4 vertexPositionModel; // shade will nicely change vec3 to vec4 for me
in layout(location=1) vec3 vertexNormalModel;
in layout(location=2) vec3 vertexUVModel;

out vec3 ReflectDir;  // The direction of the reflected ray
out vec3 VertexPositionWorld;
out vec3 NormalWorld;
out vec2 fragmentUV;

uniform bool DrawSkyBox;  // Are we drawing the sky box?
uniform vec3 cameraPositionWorld;
uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldTransMatrix;

void main()
{
	gl_Position = modelToProjectionMatrix * vertexPositionModel;
	fragmentUV = vec2(vertexUVModel.x,vertexUVModel.y);

	if(DrawSkyBox){
		ReflectDir = vec3(vertexPositionModel.x,vertexPositionModel.y,vertexPositionModel.z);
	}
	else{
		VertexPositionWorld = vec3(modelToWorldTransMatrix * vertexPositionModel);
		NormalWorld = vec3(modelToWorldTransMatrix * vec4(vertexNormalModel,0));
		vec3 ViewDirectionWorld = normalize(cameraPositionWorld - VertexPositionWorld);
		ReflectDir = reflect(-ViewDirectionWorld, NormalWorld);
	}
}