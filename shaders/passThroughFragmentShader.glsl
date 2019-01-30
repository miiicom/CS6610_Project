#version 430
in vec3 NormalWorld;
in vec3 VertexPositionWorld;

out vec4 FragmentColor;

uniform vec3 pointLightPosition;
uniform float pointLightIntensity;
uniform vec3 ambientLightUniform;
uniform vec3 cameraPositionWorld;

void main()
{
	// diffuse
	vec3 lightVectorWorld = normalize(pointLightPosition - VertexPositionWorld);
	float diffuseIntensity = dot(lightVectorWorld,normalize(NormalWorld));
	vec4 diffuseLight = vec4(diffuseIntensity,0.2 *diffuseIntensity,0.6* diffuseIntensity,1.0);

	//specular
	vec3 reflectedLightVectorWorld = reflect(lightVectorWorld, normalize(NormalWorld));
	vec3 cameraToWorld = -normalize(cameraPositionWorld - VertexPositionWorld);
	float SpecIntensity =  dot(cameraToWorld,reflectedLightVectorWorld);
	SpecIntensity = pow(SpecIntensity,6);
	vec4 specLight = vec4(SpecIntensity,SpecIntensity,SpecIntensity,1.0);
	FragmentColor = clamp(diffuseLight,0,1) + vec4(ambientLightUniform,0.0) + clamp(specLight,0,1);
	//FragmentColor = vec4(normalize(cameraPositionWorld),1.0);
	//FragmentColor =clamp(specLight,0,1);
	//FragmentColor = vec4(normalize(NormalWorld),1.0);
	//FragmentColor = vec4(0.0,0.0,0.0,1.0);
}