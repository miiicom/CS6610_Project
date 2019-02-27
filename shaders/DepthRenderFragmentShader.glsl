#version 430

out float FragmentDepth;

void main(){
    FragmentDepth = gl_FragCoord.z;
	//FragmentDepth = vec4(gl_FragCoord.z,0.0,0.0,1.0);
}