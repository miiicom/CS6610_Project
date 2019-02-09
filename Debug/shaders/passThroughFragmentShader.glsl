#version 430
in vec3 NormalWorld;
in vec3 VertexPositionWorld;
in vec2 fragmentUV;

out vec4 FragmentColor;

uniform vec3 pointLightPosition;
uniform float pointLightIntensity;
uniform vec3 ambientLightUniform;
uniform vec3 cameraPositionWorld;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

void main()
{
	vec4 diffuseColor = texture(diffuseTexture, fragmentUV);
	vec4 specularColor = texture(specularTexture, fragmentUV);
	
	// diffuse
	vec3 lightVectorWorld = normalize(pointLightPosition - VertexPositionWorld);
	float diffuseIntensity = dot(lightVectorWorld,normalize(NormalWorld));
	vec4 diffuseLight = vec4(diffuseColor.x *diffuseIntensity,diffuseColor.y *diffuseIntensity,diffuseColor.z * diffuseIntensity,1.0);

	//specular
	vec3 reflectedLightVectorWorld = reflect(lightVectorWorld, normalize(NormalWorld));
	vec3 cameraToWorld = -normalize(cameraPositionWorld - VertexPositionWorld);
	float SpecIntensity =  dot(cameraToWorld,reflectedLightVectorWorld);
	SpecIntensity = pow(SpecIntensity,6);
	vec4 specLight = specularColor.x * vec4(SpecIntensity,SpecIntensity,SpecIntensity,1.0);
	FragmentColor = clamp(diffuseLight,0,1) + vec4(ambientLightUniform,0.0) + clamp(specLight,0,1);
	//FragmentColor = vec4(normalize(cameraPositionWorld),1.0);
	//FragmentColor = clamp(specLight,0,1);
	//FragmentColor = vec4(normalize(NormalWorld),1.0);
	//FragmentColor = vec4(0.0,0.0,0.0,1.0);
	//FragmentColor = vec4(fragmentUV.x,-fragmentUV.y,0.0,1.0);
	//FragmentColor = specularColor;
}