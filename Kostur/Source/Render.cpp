#include "../Header/Render.h"

unsigned int basicShader = 0;

void formVAO(float* vertices, size_t size, unsigned int& VAO)
{
	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void formColorRect(float left, float bottom, float right, float top, float r, float g, float b, float a, unsigned int& VAO)
{
	float vertices[] = {
		left,  top,      r, g, b, a,   0.0f, 0.0f,
		left,  bottom,   r, g, b, a,   0.0f, 0.0f,
		right, bottom,   r, g, b, a,   0.0f, 0.0f,
		right, top,      r, g, b, a,   0.0f, 0.0f,
	};

	formVAO(vertices, sizeof(vertices), VAO);
}

void formTextureRect(float left, float bottom, float right, float top, unsigned int& VAO, float sRepeat)
{
	float vertices[] = {
		left,  top,      0.0f, 0.0f, 0.0f, 0.0f,   0.0f,    1.0f,
		left,  bottom,   0.0f, 0.0f, 0.0f, 0.0f,   0.0f,    0.0f,
		right, bottom,   0.0f, 0.0f, 0.0f, 0.0f,   sRepeat, 0.0f,
		right, top,      0.0f, 0.0f, 0.0f, 0.0f,   sRepeat, 1.0f,
	};

	formVAO(vertices, sizeof(vertices), VAO);
}

void drawColored(unsigned int VAO)
{
	glUniform1i(glGetUniformLocation(basicShader, "useTexture"), false);
	glUniform2f(glGetUniformLocation(basicShader, "uPosition"), 0.0f, 0.0f);
	glUniform1f(glGetUniformLocation(basicShader, "uScale"), 1.0f);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void drawTextured(unsigned int VAO, unsigned texture, float x, float y, float scale, bool flipped, float alpha)
{
	glUniform1i(glGetUniformLocation(basicShader, "useTexture"), true);
	glUniform2f(glGetUniformLocation(basicShader, "uPosition"), x, y);
	glUniform1f(glGetUniformLocation(basicShader, "uScale"), scale);
	glUniform1i(glGetUniformLocation(basicShader, "flipped"), flipped);
	glUniform1f(glGetUniformLocation(basicShader, "uAlpha"), alpha);

	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}