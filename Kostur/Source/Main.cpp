#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <ctime>

#include "../Header/Util.h"
#include "../Header/Render.h"
#include "../Header/Aquarium.h"

int screenWidth = 0;
int screenHeight = 0;

float widthFromPixels(float pixels)
{
	return pixels * 2.0f / screenWidth;
}

float heightFromPixels(float pixels)
{
	return pixels * 2.0f / screenHeight;
}

GLFWcursor* anchorCursor;

unsigned skyTexture;
unsigned waterTexture;
unsigned sandTexture;
unsigned seaweedTexture;
unsigned chestClosedTexture;
unsigned chestOpenedTexture;
unsigned goldfishTexture;
unsigned clownfishTexture;
unsigned bubbleTexture;
unsigned crabTexture;
unsigned foodTexture;
unsigned nameTexture;

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	screenWidth = mode->width;
	screenHeight = mode->height;

	srand((unsigned)time(NULL));

	float borderX = widthFromPixels(30.0f); // U specifikaciji kaže 10px, ali to nije dovoljno da se vidi crni okvir, pa sam stavio 30px.
	float borderY = heightFromPixels(30.0f); // U specifikaciji kaže 10px, ali to nije dovoljno da se vidi crni okvir, pa sam stavio 30px.

	float seaweedWidth = widthFromPixels(135.0f);
	float seaweedHeight = heightFromPixels(500.0f);
	float seaweedBottom = SAND_TOP - 0.05f;

	float chestWidth = widthFromPixels(160.0f);
	float chestHeight = heightFromPixels(160.0f);
	float chestBottom = SAND_TOP - 0.03f;

	fishWidth = widthFromPixels(120.0f);
	fishHeight = heightFromPixels(120.0f);
	goldHeight = fishHeight;
	clownHeight = fishHeight;

	bubbleWidth = widthFromPixels(18.0f);
	bubbleHeight = heightFromPixels(18.0f);

	crabWidth = widthFromPixels(140.0f);
	crabHeight = heightFromPixels(124.0f);
	crabCurHeight = crabHeight;

	foodWidth = widthFromPixels(18.0f);
	foodHeight = heightFromPixels(18.0f);

	float nameWidth = widthFromPixels(384.0f);
	float nameHeight = heightFromPixels(96.0f);
	float nameMarginX = widthFromPixels(20.0f);
	float nameMarginY = heightFromPixels(20.0f);

	float nameLeft = -1.0f + nameMarginX;
	float nameRight = nameLeft + nameWidth;
	float nameTop = 1.0f - nameMarginY;
	float nameBottom = nameTop - nameHeight;

	fishMinX = -1.0f + borderX + fishWidth / 2;
	fishMaxX = 1.0f - borderX - fishWidth / 2;

	crabMinX = -1.0f + borderX + crabWidth / 2;
	crabMaxX = 1.0f - borderX - crabWidth / 2;

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Projekat 2D", monitor, NULL);
	if (window == NULL) return endProgram("Prozor nije uspeo da se kreira.");
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, keyCallback);

	anchorCursor = loadImageToCursor("Resources/anchor.png");
	glfwSetCursor(window, anchorCursor);

	if (glewInit() != GLEW_OK) return endProgram("GLEW nije uspeo da se inicijalizuje.");

	basicShader = createShader("Shaders/basic.vert", "Shaders/basic.frag");
	glUseProgram(basicShader);
	glUniform1i(glGetUniformLocation(basicShader, "uTexture"), 0);
	glUniform1f(glGetUniformLocation(basicShader, "uAlpha"), 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	preprocessTexture(skyTexture, "Resources/sky.png");
	preprocessTexture(waterTexture, "Resources/water.png");
	preprocessTexture(sandTexture, "Resources/sand.png");
	preprocessTexture(seaweedTexture, "Resources/seaweed.png");
	preprocessTexture(chestClosedTexture, "Resources/chest_closed.png");
	preprocessTexture(chestOpenedTexture, "Resources/chest_open.png");
	preprocessTexture(goldfishTexture, "Resources/goldfish.png");
	preprocessTexture(clownfishTexture, "Resources/clownfish.png");
	preprocessTexture(bubbleTexture, "Resources/bubble.png");
	preprocessTexture(crabTexture, "Resources/crab.png");
	preprocessTexture(foodTexture, "Resources/food.png");
	preprocessTexture(nameTexture, "Resources/name.png");

	unsigned int skyVAO, waterVAO, leftBorderVAO, rightBorderVAO, bottomBorderVAO;
	unsigned int sandVAO, seaweedVAO, chestVAO;
	unsigned int fishVAO, bubbleVAO, crabVAO, foodVAO;
	unsigned int nameVAO;

	//formColorRect(-1.0f, -1.0f, 1.0f, AQUARIUM_TOP,            0.0f, 0.6f, 0.85f, 0.2f, aquariumVAO);
	formTextureRect(-1.0f, AQUARIUM_TOP, 1.0f, 1.0f, skyVAO); // Nije u specifikaciji, nebo iznad akvarijuma sam dodao da gornji deo ekrana ne bi bio prazan.
	formTextureRect(-1.0f, -1.0f, 1.0f, AQUARIUM_TOP, waterVAO);
	formColorRect(-1.0f, -1.0f, -1.0f + borderX, AQUARIUM_TOP, 0.0f, 0.0f, 0.0f,  1.0f, leftBorderVAO);
	formColorRect(1.0f - borderX, -1.0f, 1.0f, AQUARIUM_TOP,   0.0f, 0.0f, 0.0f,  1.0f, rightBorderVAO);
	formColorRect(-1.0f, -1.0f, 1.0f, -1.0f + borderY,         0.0f, 0.0f, 0.0f,  1.0f, bottomBorderVAO);

	formTextureRect(-1.0f, -1.0f, 1.0f, SAND_TOP, sandVAO, 8.0f);
	formTextureRect(-seaweedWidth / 2, seaweedBottom, seaweedWidth / 2, seaweedBottom + seaweedHeight, seaweedVAO);
	formTextureRect(-chestWidth / 2, chestBottom, chestWidth / 2, chestBottom + chestHeight, chestVAO);

	formTextureRect(-fishWidth / 2, -fishHeight / 2, fishWidth / 2, fishHeight / 2, fishVAO);
	formTextureRect(-bubbleWidth / 2, -bubbleHeight / 2, bubbleWidth / 2, bubbleHeight / 2, bubbleVAO);
	formTextureRect(-crabWidth / 2, -crabHeight / 2, crabWidth / 2, crabHeight / 2, crabVAO);
	formTextureRect(-foodWidth / 2, -foodHeight / 2, foodWidth / 2, foodHeight / 2, foodVAO);

	formTextureRect(nameLeft, nameBottom, nameRight, nameTop, nameVAO);

	while (!glfwWindowShouldClose(window))
	{
		double initFrameTime = glfwGetTime();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;

		moveFish(window, goldX, goldY, goldFlipped, goldHeight, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S);
		moveFish(window, clownX, clownY, clownFlipped, clownHeight, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN);
		updateBubbles();
		updateCrab(window);
		updateFood();

		fishEat();
		crabEat();

		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(basicShader);
		glActiveTexture(GL_TEXTURE0);

		drawTextured(skyVAO, skyTexture);
		//drawColored(aquariumVAO);
		drawTextured(waterVAO, waterTexture, 0.0f, 0.0f, 1.0f, false, 0.8f); // U specifikaciji kaže beli četvorougao providnosti 0,2, ali sam stavio teksturu vode sa alfom 0.8 da bi akvarijum izgledao kao more.

		drawTextured(seaweedVAO, seaweedTexture, -SEAWEED_X, 0.0f);
		drawTextured(seaweedVAO, seaweedTexture, SEAWEED_X, 0.0f);
		drawTextured(sandVAO, sandTexture);
		drawTextured(chestVAO, chestOpened ? chestOpenedTexture : chestClosedTexture);

		for (int i = 0; i < MAX_FOOD; i++)
			if (food[i].active)
				drawTextured(foodVAO, foodTexture, food[i].x, food[i].y);

		float crabCenterY = crabBottom + crabCurHeight / 2 + crabJumpY;
		drawTextured(crabVAO, crabTexture, crabX, crabCenterY, crabCurHeight / crabHeight);

		drawTextured(fishVAO, goldfishTexture, goldX, goldY, goldHeight / fishHeight, goldFlipped);
		drawTextured(fishVAO, clownfishTexture, clownX, clownY, clownHeight / fishHeight, clownFlipped);

		for (int i = 0; i < MAX_BUBBLES; i++)
			if (bubbles[i].active)
				drawTextured(bubbleVAO, bubbleTexture, bubbles[i].x, bubbles[i].y);

		drawColored(leftBorderVAO);
		drawColored(rightBorderVAO);
		drawColored(bottomBorderVAO);

		drawTextured(nameVAO, nameTexture, 0.0f, 0.0f, 1.0f, false, 0.5f);

		glfwSwapBuffers(window);
		glfwPollEvents();

		while (glfwGetTime() - initFrameTime < 1 / 75.0) {}
	}

	unsigned int vaos[] = {
		skyVAO, waterVAO, leftBorderVAO, rightBorderVAO, bottomBorderVAO,
		sandVAO, seaweedVAO, chestVAO,
		fishVAO, bubbleVAO, crabVAO, foodVAO,
		nameVAO
	};
	glDeleteVertexArrays(sizeof(vaos) / sizeof(vaos[0]), vaos);

	unsigned textures[] = {
		skyTexture, waterTexture, sandTexture, seaweedTexture, chestClosedTexture, chestOpenedTexture,
		goldfishTexture, clownfishTexture,
		bubbleTexture, crabTexture, foodTexture,
		nameTexture
	};
	glDeleteTextures(sizeof(textures) / sizeof(textures[0]), textures);

	glDeleteProgram(basicShader);

	glfwDestroyCursor(anchorCursor);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}