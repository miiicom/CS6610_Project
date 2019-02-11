#version 430

in vec2 fragmentUV;

out vec4 FragmentColor;

uniform sampler2D frameBufferTexture;

void main()
{
	FragmentColor = vec4(texture(frameBufferTexture, fragmentUV0), 1.0);
}