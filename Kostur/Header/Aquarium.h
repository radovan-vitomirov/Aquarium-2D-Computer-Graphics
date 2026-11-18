#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const float AQUARIUM_TOP = 0.4f;
const float SAND_TOP = -0.7f;
const float SEAWEED_X = 0.6f;

const float FISH_SPEED = 0.01f;

const int MAX_BUBBLES = 30;
const int BUBBLES_PER_PRESS = 3;
const float BUBBLE_SPEED = 0.006f;
const float MOUTH_OFFSET = 0.05f;
const float BUBBLE_STEP_X = 0.02f;
const float BUBBLE_STEP_Y = 0.06f;

const float CRAB_SPEED = 0.008f;
const float JUMP_G = 3.0f;
const float JUMP_H = 0.15f;

const int MAX_FOOD = 50;
const int MAX_PER_DROP = 5;
const float FOOD_SPEED = 0.006f;
const float FOOD_SPAWN_LEFT = -0.9f;
const float FOOD_SPAWN_WIDTH = 1.8f;
const float FOOD_SPAWN_BOTTOM = 1.1f;
const float FOOD_SPAWN_HEIGHT = 0.3f;

const float GROW_STEP = 0.01f;

struct Bubble
{
	float x;
	float y;
	bool active;
};

struct Food
{
	float x;
	float y;
	bool active;
	bool onSand;
};

extern bool chestOpened;

extern float fishWidth, fishHeight;
extern float fishMinX, fishMaxX;

extern float goldX, goldY, goldHeight;
extern bool goldFlipped;

extern float clownX, clownY, clownHeight;
extern bool clownFlipped;

extern Bubble bubbles[MAX_BUBBLES];
extern float bubbleWidth, bubbleHeight;

extern float crabWidth, crabHeight, crabBottom;
extern float crabMinX, crabMaxX;

extern float crabX, crabCurHeight;
extern double crabJumpStart;
extern float crabJumpY;

extern Food food[MAX_FOOD];
extern float foodWidth, foodHeight;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

void moveFish(GLFWwindow* window, float& x, float& y, bool& flipped, float height, int keyLeft, int keyRight, int keyUp, int keyDown);
void fishEat();

void spawnBubbles(float fishX, float fishY, bool flipped);
void updateBubbles();

void updateCrab(GLFWwindow* window);
void crabEat();

void dropFood();
void updateFood();