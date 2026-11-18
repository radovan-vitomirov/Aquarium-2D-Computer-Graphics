#version 330 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec4 inCol;
layout (location = 2) in vec2 inTexture;

out vec4 chCol;
out vec2 chTexture;

uniform vec2 uPosition;
uniform float uScale;
uniform bool flipped;

void main()
{
	gl_Position = vec4(inPos.x + uPosition.x, inPos.y * uScale + uPosition.y, 0.0f, 1.0f);

	chCol = inCol;

	if (flipped)
		chTexture = vec2(1.0f - inTexture.x, inTexture.y);
	else
		chTexture = inTexture;
}
