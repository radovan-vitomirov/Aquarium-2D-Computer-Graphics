#version 330 core

in vec4 chCol;
in vec2 chTexture;

out vec4 outCol;

uniform bool useTexture;
uniform sampler2D uTexture;
uniform float uAlpha;

void main()
{
	if (useTexture)
	{
		outCol = texture(uTexture, chTexture);
		outCol.a *= uAlpha;
	}
	else
		outCol = chCol;
}
