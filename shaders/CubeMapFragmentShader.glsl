#version 430

in vec3 fragmentPositionModel;;
in vec3 VertexPositionWorld;
in vec3 NormalWorld;
in vec2 fragmentUV;

out vec4 FragmentColor;

uniform bool DrawSkyBox;  // Are we drawing the sky box?
uniform vec3 pointLightPosition;
uniform float pointLightIntensity;
uniform vec3 ambientLightUniform;
uniform vec3 cameraPositionWorld;



uniform samplerCube CubeMapTexture;

void main()

{
	vec3 ReflectDir = vec3(0.0,0.0,0.0);
	if(DrawSkyBox){
		ReflectDir = vec3(fragmentPositionModel.x,fragmentPositionModel.y,fragmentPositionModel.z);
	}
	else{
		vec3 ViewDirectionWorld = normalize(cameraPositionWorld - VertexPositionWorld);
		ReflectDir = reflect(-ViewDirectionWorld, normalize(NormalWorld));
		//ReflectDir = vec3(ReflectDir.x,ReflectDir.y,ReflectDir.z);
		//ReflectDir = ViewDirectionWorld;
	}
	
	vec4 cubeMapColor = texture(CubeMapTexture,ReflectDir);
	FragmentColor = cubeMapColor;
	//FragmentColor = vec4( normalize(ReflectDir),1.0);
}