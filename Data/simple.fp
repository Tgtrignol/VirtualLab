#version 410 compatibility

uniform sampler2D texture;
uniform bool glow;
uniform vec4 glowColor;
in vec2 textureCoord;

out vec4 outputColor;

void main()
{
	if(glow)
	{
		outputColor = glowColor;
	}
	else
	{
		outputColor = texture2D(texture, textureCoord);
	}
}