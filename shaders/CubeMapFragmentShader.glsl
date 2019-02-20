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
	}

	//Here calculate Blinn light model

	//Diffuse

	vec3 lightVectorWorld = normalize(pointLightPosition - VertexPositionWorld);
	float diffuseIntensity = dot(lightVectorWorld,normalize(NormalWorld));
	vec4 diffuseLight = vec4(diffuseIntensity,diffuseIntensity,diffuseIntensity,1.0);

	//Specular

	vec3 reflectedLightVectorWorld = reflect(lightVectorWorld, normalize(NormalWorld));
	vec3 cameraToWorld = -normalize(cameraPositionWorld - VertexPositionWorld);
	float SpecIntensity =  dot(cameraToWorld,reflectedLightVectorWorld);
	SpecIntensity = pow(SpecIntensity,6);
	vec4 specLight = vec4(SpecIntensity,SpecIntensity,SpecIntensity,1.0);

	//Combine Diffuse and Specular

	vec4 BlinnColor = clamp(diffuseLight,0,1) + vec4(ambientLightUniform,0.0) + clamp(specLight,0,1);
	
	vec4 cubeMapColor = texture(CubeMapTexture,ReflectDir);

	//Lerp between them
	if(DrawSkyBox){
		FragmentColor = cubeMapColor;
	}
	else{
		FragmentColor = mix(BlinnColor, cubeMapColor,0.6);
	}
	
	//FragmentColor = cubeMapColor;
	//FragmentColor = vec4( normalize(ReflectDir),1.0);


}