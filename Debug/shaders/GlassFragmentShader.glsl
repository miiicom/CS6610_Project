#version 430

out vec4 FragmentColor;

uniform sampler2D ReversedSceneTexture;

void main()
{
	vec2 TextureUV = vec2(gl_FragCoord.x / 2 + 1, gl_FragCoord.y / 2 + 1); //Map to (0,1);
	vec4 samplerTexture = texture(ReversedSceneTexture, TextureUV);

	TextureUV = normalize(TextureUV);
	//FragmentColor = vec4(TextureUV.x, TextureUV.y, 0.0,1.0);
	FragmentColor = samplerTexture;
}