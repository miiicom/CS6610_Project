#version 430

out vec4 FragmentColor;
in vec3 fragColor;

void main()
{
	FragmentColor = vec4(fragColor,1.0);
}