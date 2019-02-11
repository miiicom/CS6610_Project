#version 430

in vec2 fragmentUV;

out vec4 FragmentColor;

uniform sampler2D frameBufferTexture;

void main()
{
	FragmentColor = vec4(vec3(texture(frameBufferTexture, fragmentUV)), 1.0);
}