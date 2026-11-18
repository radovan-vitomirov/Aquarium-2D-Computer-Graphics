#pragma once

#include <GL/glew.h>

extern unsigned int basicShader;

void formVAO(float* vertices, size_t size, unsigned int& VAO);
void formColorRect(float left, float bottom, float right, float top, float r, float g, float b, float a, unsigned int& VAO);
void formTextureRect(float left, float bottom, float right, float top, unsigned int& VAO, float sRepeat = 1.0f);

void drawColored(unsigned int VAO);
void drawTextured(unsigned int VAO, unsigned texture, float x = 0.0f, float y = 0.0f, float scale = 1.0f, bool flipped = false, float alpha = 1.0f);