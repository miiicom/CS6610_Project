#version 430

out vec4 FragmentColor;

in PipelineData {
    vec4 position;
	vec3 normal;
	vec2 fragmentUV;
} fs_in;

void main()
{

	FragmentColor = vec4(1.0,1.0,0.0,1.0);
	//FragmentColor = vec4(fs_in.normal, 1.0);
	//FragmentColor = fs_in.position;
	//FragmentColor = gl_FragCoord/200;
}