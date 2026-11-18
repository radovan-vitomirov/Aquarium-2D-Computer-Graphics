#include "../Header/Aquarium.h"

#include <cmath>
#include <cstdlib>

bool chestOpened = false;

float fishWidth = 0.0f;
float fishHeight = 0.0f;
float fishMinX = 0.0f;
float fishMaxX = 0.0f;

float goldX = -0.3f;
float goldY = 0.0f;
float goldHeight = 0.0f;
bool goldFlipped = false;

float clownX = 0.3f;
float clownY = -0.2f;
float clownHeight = 0.0f;
bool clownFlipped = true;

Bubble bubbles[MAX_BUBBLES];
float bubbleWidth = 0.0f;
float bubbleHeight = 0.0f;

float crabWidth = 0.0f;
float crabHeight = 0.0f;
float crabBottom = SAND_TOP - 0.03f;
float crabMinX = 0.0f;
float crabMaxX = 0.0f;

float crabX = 0.5f;
float crabCurHeight = 0.0f;
double crabJumpStart = -1.0f;
float crabJumpY = 0.0f;

Food food[MAX_FOOD];
float foodWidth = 0.0f;
float foodHeight = 0.0f;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS)
		return;

	if (key == GLFW_KEY_C)
		chestOpened = !chestOpened;

	if (key == GLFW_KEY_Z)
		spawnBubbles(goldX, goldY, goldFlipped);

	if (key == GLFW_KEY_K)
		spawnBubbles(clownX, clownY, clownFlipped);

	if (key == GLFW_KEY_SPACE && crabJumpStart < 0)
		crabJumpStart = glfwGetTime();

	if (key == GLFW_KEY_ENTER)
		dropFood();
}

void moveFish(GLFWwindow* window, float& x, float& y, bool& flipped, float height, int keyLeft, int keyRight, int keyUp, int keyDown)
{
	if (glfwGetKey(window, keyLeft) == GLFW_PRESS)
	{
		x -= FISH_SPEED;
		flipped = true;
	}
	if (glfwGetKey(window, keyRight) == GLFW_PRESS)
	{
		x += FISH_SPEED;
		flipped = false;
	}

	if (glfwGetKey(window, keyUp) == GLFW_PRESS)
		y += FISH_SPEED;
	if (glfwGetKey(window, keyDown) == GLFW_PRESS)
		y -= FISH_SPEED;

	if (x < fishMinX) x = fishMinX;
	if (x > fishMaxX) x = fishMaxX;

	float minY = SAND_TOP + height / 2;
	float maxY = AQUARIUM_TOP - height / 2;
	if (y < minY) y = minY;
	if (y > maxY) y = maxY;
}

void fishEat()
{
	for (int i = 0; i < MAX_FOOD; i++)
	{
		if (!food[i].active || food[i].onSand)
			continue;

		if (fabs(food[i].x - goldX) < fishWidth / 2 &&
			fabs(food[i].y - goldY) < goldHeight / 2)
		{
			food[i].active = false;
			goldHeight += GROW_STEP;
		}
		else if (fabs(food[i].x - clownX) < fishWidth / 2 &&
			fabs(food[i].y - clownY) < clownHeight / 2)
		{
			food[i].active = false;
			clownHeight += GROW_STEP;
		}
	}
}

void spawnBubbles(float fishX, float fishY, bool flipped)
{
	float mouthX = flipped ? fishX - MOUTH_OFFSET : fishX + MOUTH_OFFSET;
	int made = 0;

	for (int i = 0; i < MAX_BUBBLES && made < BUBBLES_PER_PRESS; i++)
	{
		if (!bubbles[i].active)
		{
			bubbles[i].active = true;

			if (flipped)
				bubbles[i].x = mouthX - made * BUBBLE_STEP_X;
			else
				bubbles[i].x = mouthX + made * BUBBLE_STEP_X;

			bubbles[i].y = fishY + made * BUBBLE_STEP_Y;

			made++;
		}
	}
}

void updateBubbles()
{
	for (int i = 0; i < MAX_BUBBLES; i++)
	{
		if (bubbles[i].active)
		{
			bubbles[i].y += BUBBLE_SPEED;

			if (bubbles[i].y + bubbleHeight / 2 > AQUARIUM_TOP)
				bubbles[i].active = false;
		}
	}
}

void updateCrab(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		crabX -= CRAB_SPEED;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		crabX += CRAB_SPEED;

	if (crabX < crabMinX) crabX = crabMinX;
	if (crabX > crabMaxX) crabX = crabMaxX;

	if (crabJumpStart >= 0)
	{
		float t = (float)(glfwGetTime() - crabJumpStart);
		float v0 = sqrt(2 * JUMP_G * JUMP_H);
		float s = v0 * t - 0.5f * JUMP_G * t * t;

		if (s < 0.0f)
		{
			s = 0.0f;
			crabJumpStart = -1.0f;
		}

		crabJumpY = s;
	}
}

void crabEat()
{
	for (int i = 0; i < MAX_FOOD; i++)
	{
		if (!food[i].active)
			continue;

		if (!food[i].onSand && crabJumpStart < 0)
			continue;

		float crabLow = crabBottom + crabJumpY;
		float crabHigh = crabLow + crabCurHeight;

		if (fabs(food[i].x - crabX) < crabWidth / 2 &&
			food[i].y > crabLow && food[i].y < crabHigh)
		{
			food[i].active = false;
			crabCurHeight += GROW_STEP;
		}
	}
}

void dropFood()
{
	int made = 0;

	for (int i = 0; i < MAX_FOOD && made < MAX_PER_DROP; i++)
	{
		if (!food[i].active)
		{
			food[i].active = true;
			food[i].onSand = false;

			food[i].x = FOOD_SPAWN_LEFT + (rand() / (float)RAND_MAX) * FOOD_SPAWN_WIDTH;
			food[i].y = FOOD_SPAWN_BOTTOM + (rand() / (float)RAND_MAX) * FOOD_SPAWN_HEIGHT;

			made++;
		}
	}
}

void updateFood()
{
	for (int i = 0; i < MAX_FOOD; i++)
	{
		if (food[i].active && !food[i].onSand)
		{
			food[i].y -= FOOD_SPEED;

			if (food[i].y - foodHeight / 2 <= SAND_TOP)
			{
				food[i].y = SAND_TOP + foodHeight / 2;
				food[i].onSand = true;
			}
		}
	}
}