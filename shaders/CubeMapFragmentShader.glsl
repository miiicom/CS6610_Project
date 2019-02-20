#version 430

in vec3 ReflectDir;  // The direction of the reflected ray
in vec3 VertexPositionWorld;
in vec3 NormalWorld;
in vec2 fragmentUV;

out vec4 FragmentColor;

uniform vec3 pointLightPosition;
uniform float pointLightIntensity;
uniform vec3 ambientLightUniform;
uniform vec3 cameraPositionWorld;

uniform samplerCube CubeMapTexture;

void main()
{
	vec4 cubeMapColor = texture(CubeMapTexture,ReflectDir);
	FragmentColor = cubeMapColor;
	//FragmentColor = vec4(VertexPositionWorld,1.0);
}