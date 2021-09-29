//-----------------------------------------------------------------------------
// This code demonstrates how to implement the Blinn-Phong lighting model for
// multiple light sources using OpenGL 3.3 or a forward compatible
// OpenGL 3.0 rendering context. 
//-----------------------------------------------------------------------------
#include <iostream>
#include <sstream>
#include <string>
#define GLEW_STATIC
#include "GL/glew.h"	// Important - this header must come before glfw3 header
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Mesh.h"


// Global Variables
const char* APP_TITLE = "Introduction to Modern OpenGL - Multiple Lights";
int gWindowWidth = 1600;
int gWindowHeight = 900;
GLFWwindow* gWindow = NULL;
bool gWireframe = false;
bool gFlashlightOn = true;
glm::vec4 gClearColor(0.06f, 0.06f, 0.07f, 1.0f);

FPSCamera fpsCamera(glm::vec3(-80.0f, 50.0f, 80.0f));
const double ZOOM_SENSITIVITY = -3.0;
const float MOVE_SPEED = 15.0; // units per second
const float MOUSE_SENSITIVITY = 0.1f;


// Function prototypes
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY);
void update(double elapsedTime);
void showFPS(GLFWwindow* window);
bool initOpenGL();

//-----------------------------------------------------------------------------
// Main Application Entry Point
//-----------------------------------------------------------------------------
int main()
{
	if (!initOpenGL())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return -1;
	}

	ShaderProgram lightingShader;
	lightingShader.loadShaders("shaders/lighting_dir_point_spot.vert", "shaders/lighting_dir_point_spot.frag");


	fpsCamera.rotate(-100.0f, -20.0f);

	// Load meshes and textures
	const int numModels = 5;
	Mesh mesh[numModels];
	Texture2D texture[numModels];


	mesh[0].loadOBJ("models/floor.obj");
	mesh[1].loadOBJ("models/wooden_tower.obj");
	mesh[2].loadOBJ("models/fox.obj");
	mesh[3].loadOBJ("models/campfire.obj");
	mesh[4].loadOBJ("models/cart.obj");

	texture[0].loadTexture("textures/Green1.jpg", true);
	texture[1].loadTexture("textures/wooden_tower.jpg", true);
	texture[2].loadTexture("textures/fox.png", true);
	texture[3].loadTexture("textures/campfire.png", true);
	texture[4].loadTexture("textures/cart_wood.png", true);
	// Model positions
	glm::vec3 modelPos[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),	// floor
		glm::vec3(-20.0f, 0.0f, 20.0f),	// tower
		glm::vec3(10.0f, 0.0f, 35.0f),	// fox
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-10.0f, 0.0f, -20.0f),
	};

	// Model scale
	glm::vec3 modelScale[] = {
		glm::vec3(200.0f, 1.0f, 200.0f),	// floor
		glm::vec3(3.8f, 3.8f, 3.8f),		// tower
		glm::vec3(0.05f, 0.05f, 0.05f)	,	// fox
		glm::vec3(3.0f, 3.0f, 3.0f),
		glm::vec3(2.0f, 2.0f, 2.0f)
	};

	float rotations[] = {
		0.0f,	// floor
		0.0f,		// tower
		160.0f	,	// fox
		0.0f,
		0.0f
	};





	const int number_of_grass_object = 7;
	Mesh grass[number_of_grass_object];
	Texture2D grass_texture[number_of_grass_object];

	grass[0].loadOBJ("models/grass_b1.obj");
	grass[1].loadOBJ("models/grass_b2.obj");
	grass[2].loadOBJ("models/grass_b3.obj");
	grass[3].loadOBJ("models/grass_b4.obj");
	grass[4].loadOBJ("models/grass_b5.obj");
	grass[5].loadOBJ("models/grass_b6.obj");
	grass[6].loadOBJ("models/grass_b7.obj");


	grass_texture[0].loadTexture("textures/Green1.jpg");
	grass_texture[1].loadTexture("textures/Green1.jpg");
	grass_texture[2].loadTexture("textures/Green1.jpg");
	grass_texture[3].loadTexture("textures/Green1.jpg");
	grass_texture[4].loadTexture("textures/Green1.jpg");
	grass_texture[5].loadTexture("textures/Green1.jpg");
	grass_texture[6].loadTexture("textures/Green1.jpg");

	glm::vec3 grassScale[] = {
		glm::vec3(4.0f, 4.0f, 4.0f),		// 1
		glm::vec3(4.0f, 4.0f, 4.0f),
		glm::vec3(4.0f, 4.0f, 4.0f),
		glm::vec3(4.0f, 4.0f, 4.0f),
		glm::vec3(4.0f, 4.0f, 4.0f),
		glm::vec3(4.0f, 4.0f, 4.0f),
		glm::vec3(4.0f, 4.0f, 4.0f)
	};



	//-----------------------------------------------------------------------------
	// Trees Models and Textures
	//-----------------------------------------------------------------------------
	const int number_of_trees_object = 12;
	Mesh trees[number_of_trees_object];
	Texture2D treeTextures[number_of_trees_object];

	trees[0].loadOBJ("models/tree1.obj");
	trees[1].loadOBJ("models/tree2.obj");
	trees[2].loadOBJ("models/tree3.obj");
	trees[3].loadOBJ("models/tree4.obj");
	trees[4].loadOBJ("models/tree5.obj");
	trees[5].loadOBJ("models/tree6.obj");
	trees[6].loadOBJ("models/tree7.obj");
	trees[7].loadOBJ("models/tree8.obj");
	trees[8].loadOBJ("models/tree9.obj");
	trees[9].loadOBJ("models/tree10.obj");
	trees[10].loadOBJ("models/tree11.obj");
	trees[11].loadOBJ("models/tree12.obj");

	treeTextures[0].loadTexture("textures/tree1.png", true);
	treeTextures[1].loadTexture("textures/tree2.png", true);
	treeTextures[2].loadTexture("textures/tree3.png", true);
	treeTextures[3].loadTexture("textures/tree4.png", true);
	treeTextures[4].loadTexture("textures/tree5.png", true);
	treeTextures[5].loadTexture("textures/tree6.png", true);
	treeTextures[6].loadTexture("textures/tree7.png", true);
	treeTextures[7].loadTexture("textures/tree8.png", true);
	treeTextures[8].loadTexture("textures/tree9.png", true);
	treeTextures[9].loadTexture("textures/tree10.png", true);
	treeTextures[10].loadTexture("textures/tree11.png", true);
	treeTextures[11].loadTexture("textures/tree12.png", true);

	glm::vec3 treeScale[] = {
		glm::vec3(15.0f, 15.0f, 15.0f),		// 1
		glm::vec3(15.0f, 15.0f, 15.0f),		// 2
		glm::vec3(15.0f, 15.0f, 15.0f),		// 3
		glm::vec3(15.0f, 15.0f, 15.0f),		// 4
		glm::vec3(15.0f, 15.0f, 15.0f),		// 5
		glm::vec3(15.0f, 15.0f, 15.0f),		// 6
		glm::vec3(15.0f, 15.0f, 15.0f),		// 7
		glm::vec3(15.0f, 15.0f, 15.0f),		// 8
		glm::vec3(30.0f, 30.0f, 30.0f),		// 9
		glm::vec3(15.0f, 15.0f, 15.0f),		// 10
		glm::vec3(15.0f, 15.0f, 15.0f),		// 11
		glm::vec3(15.0f, 15.0f, 15.0f)		// 12
	};




	//-----------------------------------------------------------------------------
	// Mushrooms Models and Textures
	//-----------------------------------------------------------------------------
	const int number_of_mushrooms_object = 6;
	Mesh mushrooms[number_of_mushrooms_object];
	Texture2D mushroomTextures[number_of_mushrooms_object];

	mushrooms[0].loadOBJ("models/mushroom1.obj");
	mushrooms[1].loadOBJ("models/mushroom2.obj");
	mushrooms[2].loadOBJ("models/mushroom3.obj");
	mushrooms[3].loadOBJ("models/mushroom5.obj");
	mushrooms[4].loadOBJ("models/mushroom5.obj");
	mushrooms[5].loadOBJ("models/mushroom8.obj");

	mushroomTextures[0].loadTexture("textures/mushroom1.jpg", true);
	mushroomTextures[1].loadTexture("textures/mushroom2.jpg", true);
	mushroomTextures[2].loadTexture("textures/mushroom3.jpg", true);
	mushroomTextures[3].loadTexture("textures/mushroom5.jpg", true);
	mushroomTextures[4].loadTexture("textures/mushroom5.jpg", true);
	mushroomTextures[5].loadTexture("textures/mushroom8.jpg", true);

	glm::vec3 mushroomScale[] = {
		glm::vec3(1.0f, 1.0f, 1.0f),		// 1
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(10.0f, 10.0f, 10.0f),
		glm::vec3(10.0f, 10.0f, 10.0f),
		glm::vec3(10.0f, 10.0f, 10.0f),
		glm::vec3(10.0f, 10.0f, 10.0f)
	};




	//-----------------------------------------------------------------------------
	// Wood Models and Textures
	//-----------------------------------------------------------------------------
	const int number_of_wood_object = 1;
	Mesh woods[number_of_wood_object];
	Texture2D woodTextures[number_of_wood_object];

	woods[0].loadOBJ("models/wood.obj");

	woodTextures[0].loadTexture("textures/wood.png", true);

	glm::vec3 woodScale[] = {
		glm::vec3(3.0f, 3.0f, 3.0f)
	};


	//-----------------------------------------------------------------------------
	// Axe Models and Textures
	//-----------------------------------------------------------------------------
	Mesh axe;
	Texture2D axeTexture;

	axe.loadOBJ("models/axe.obj");

	axeTexture.loadTexture("textures/axe.png", true);



	//-----------------------------------------------------------------------------
	// House Models and Textures
	//-----------------------------------------------------------------------------
	Mesh house;
	Texture2D houseTexture;

	house.loadOBJ("models/house.obj");

	houseTexture.loadTexture("textures/cart_wood.png", true);





	fpsCamera.rotate(60.0f, 0.0f);


	float angle = 0.0f;

	const int number_of_trees = 800;

	int treesNum[number_of_trees];
	glm::vec3 treePos[number_of_trees];
	float tree_rotation[number_of_trees];
	for (int i = 0; i < number_of_trees; i++)
	{
		int k = rand() % number_of_trees_object;
		treesNum[i] = k;
		float x, z;
		do {
			x = (rand() % 800) - 400;
			z = (rand() % 800) - 400;

		} while (sqrt(x * x + z * z) < 75);

		tree_rotation[i] = rand() % 360;

		treePos[i] = glm::vec3(x, 0.0f, z);
	}






	const int number_of_grasses = 3000;

	int grassNum[number_of_grasses];
	glm::vec3 grassPos[number_of_grasses];
	float grass_rotation[number_of_grasses];
	for (int i = 0; i < number_of_grasses; i++)
	{
		int k = rand() % number_of_grass_object;
		grassNum[i] = k;
		float x, z;
		do {
			x = (rand() % 800) - 400;
			z = (rand() % 800) - 400;
		} while (x > 30 && x < 70 && z >5 && z < 35);

		grass_rotation[i] = rand() % 360;

		grassPos[i] = glm::vec3(x, 0.0f, z);
	}





	const int number_of_mushrooms = 600;

	int mushroomNum[number_of_mushrooms];
	glm::vec3 mushroomPos[number_of_mushrooms];
	float mushroom_rotation[number_of_mushrooms];
	for (int i = 0; i < number_of_mushrooms; i++)
	{
		int k = rand() % number_of_mushrooms_object;
		mushroomNum[i] = k;
		float x, z;
		x = (rand() % 800) - 400;
		z = (rand() % 800) - 400;

		mushroom_rotation[i] = rand() % 360;

		mushroomPos[i] = glm::vec3(x, 0.0f, z);
	}



	const int number_of_woods = 6;

	int woodNum[number_of_woods];
	glm::vec3 woodPos[number_of_woods];
	float wood_rotation[number_of_woods];
	for (int i = 0; i < number_of_woods; i += 2)
	{
		int k = rand() % number_of_wood_object;
		woodNum[i] = k;
		woodNum[i + 1] = k;
		float x, z;

		float r = 45.0 + (rand() % 10) - 5;
		float angle = rand() % 360;

		x = r * cosf(glm::radians(angle));
		z = r * sinf(glm::radians(angle));

		wood_rotation[i] = rand() % 360;
		wood_rotation[i + 1] = wood_rotation[i] + 180.0f;

		woodPos[i] = glm::vec3(x, 0.0f, z);
		woodPos[i + 1] = glm::vec3(x, 0.0f, z);
	}




	// Point Light positions
	glm::vec3 pointLightPos[6] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-20.0f,  25.0f, 20.0f),
		glm::vec3(50.0f,  5,  20.0f),
		glm::vec3(50.0f,  5,  15.0f),
		glm::vec3(50.0f,  5,  25.0f)
	};


	double lastTime = glfwGetTime();

	// Rendering loop
	while (!glfwWindowShouldClose(gWindow))
	{
		showFPS(gWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		// Poll for and process events
		glfwPollEvents();
		update(deltaTime);

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model(1.0), view(1.0), projection(1.0);

		// Create the View matrix
		view = fpsCamera.getViewMatrix();

		// Create the projection matrix
		projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 600.0f);

		// update the view (camera) position
		glm::vec3 viewPos;
		viewPos.x = fpsCamera.getPosition().x;
		viewPos.y = fpsCamera.getPosition().y;
		viewPos.z = fpsCamera.getPosition().z;


		// Must be called BEFORE setting uniforms because setting uniforms is done
		// on the currently active shader program.
		lightingShader.use();
		lightingShader.setUniform("model", glm::mat4(1.0));  // do not need to translate the models so just send the identity matrix
		lightingShader.setUniform("view", view);
		lightingShader.setUniform("projection", projection);
		lightingShader.setUniform("viewPos", viewPos);

		// Directional light
		lightingShader.setUniform("sunLight.direction", glm::vec3(0.0f, -0.9f, -0.17f));
		lightingShader.setUniform("sunLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		lightingShader.setUniform("sunLight.diffuse", glm::vec3(0.2f, 0.2f, 0.2f));		// dark
		lightingShader.setUniform("sunLight.specular", glm::vec3(0.1f, 0.1f, 0.1f));

		// Point Light 1
		lightingShader.setUniform("pointLights[0].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		lightingShader.setUniform("pointLights[0].diffuse", glm::vec3(1.0f, 0.0f, 0.0f));	// light  campfire
		lightingShader.setUniform("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setUniform("pointLights[0].position", pointLightPos[0]);
		lightingShader.setUniform("pointLights[0].constant", 1.0f);
		lightingShader.setUniform("pointLights[0].linear", 0.05f);
		lightingShader.setUniform("pointLights[0].exponent", 0.05f);

		// Point Light 2
		lightingShader.setUniform("pointLights[1].ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("pointLights[1].diffuse", glm::vec3(1.0f, 0.1f, 0.0f));	// red-ish light  tower
		lightingShader.setUniform("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setUniform("pointLights[1].position", pointLightPos[1]);
		lightingShader.setUniform("pointLights[1].constant", 1.0f);
		lightingShader.setUniform("pointLights[1].linear", 0.001f);
		lightingShader.setUniform("pointLights[1].exponent", 0.002f);

		// Point Light 3
		lightingShader.setUniform("pointLights[2].ambient", glm::vec3(0.9f, 0.9f, 0.9f));
		lightingShader.setUniform("pointLights[2].diffuse", glm::vec3(0.8f, 0.5f, 0.5f));	//  light  house
		lightingShader.setUniform("pointLights[2].specular", glm::vec3(0.2f, 0.2f, 0.2f));
		lightingShader.setUniform("pointLights[2].position", pointLightPos[2]);
		lightingShader.setUniform("pointLights[2].constant", 1.0f);
		lightingShader.setUniform("pointLights[2].linear", 0.001f);
		lightingShader.setUniform("pointLights[2].exponent", 0.001f);

		// Point Light 3
		lightingShader.setUniform("pointLights[3].ambient", glm::vec3(0.9f, 0.9f, 0.9f));
		lightingShader.setUniform("pointLights[3].diffuse", glm::vec3(0.8f, 0.5f, 0.5f));	//  light  house
		lightingShader.setUniform("pointLights[3].specular", glm::vec3(0.2f, 0.2f, 0.2f));
		lightingShader.setUniform("pointLights[3].position", pointLightPos[3]);
		lightingShader.setUniform("pointLights[3].constant", 1.0f);
		lightingShader.setUniform("pointLights[3].linear", 0.001f);
		lightingShader.setUniform("pointLights[3].exponent", 0.001f);

		// Point Light 3
		lightingShader.setUniform("pointLights[4].ambient", glm::vec3(0.9f, 0.9f, 0.9f));
		lightingShader.setUniform("pointLights[4].diffuse", glm::vec3(0.8f, 0.5f, 0.5f));	//  light  house
		lightingShader.setUniform("pointLights[4].specular", glm::vec3(0.2f, 0.2f, 0.2f));
		lightingShader.setUniform("pointLights[4].position", pointLightPos[4]);
		lightingShader.setUniform("pointLights[4].constant", 1.0f);
		lightingShader.setUniform("pointLights[4].linear", 0.001f);
		lightingShader.setUniform("pointLights[4].exponent", 0.001f);

		// Spot light
		glm::vec3 spotlightPos = fpsCamera.getPosition();

		// offset the flash light down a little
		spotlightPos.y -= 0.5f;

		lightingShader.setUniform("spotLight.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("spotLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setUniform("spotLight.position", spotlightPos);
		lightingShader.setUniform("spotLight.direction", fpsCamera.getLook());
		lightingShader.setUniform("spotLight.cosInnerCone", glm::cos(glm::radians(15.0f)));
		lightingShader.setUniform("spotLight.cosOuterCone", glm::cos(glm::radians(20.0f)));
		lightingShader.setUniform("spotLight.constant", 1.0f);
		lightingShader.setUniform("spotLight.linear", 0.01f);
		lightingShader.setUniform("spotLight.exponent", 0.001f);
		lightingShader.setUniform("spotLight.on", gFlashlightOn);

		// Render the scene
		for (int i = 0; i < numModels; i++)
		{
			model = glm::translate(glm::mat4(1.0), modelPos[i]) * glm::scale(glm::mat4(1.0), modelScale[i]) * glm::rotate(glm::mat4(), glm::radians(rotations[i]), glm::vec3(0.0f, 1.0f, 0.0f));
			lightingShader.setUniform("model", model);

			// Set material properties
			lightingShader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
			lightingShader.setUniformSampler("material.diffuseMap", 0);
			lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
			lightingShader.setUniform("material.shininess", 32.0f);

			texture[i].bind(0);		// set the texture before drawing.  Our simple OBJ mesh loader does not do materials yet.
			mesh[i].draw();			// Render the OBJ mesh
			texture[i].unbind(0);
		}


		// render the log
		model = glm::translate(glm::mat4(1.0), glm::vec3(20.0f, 0.0f, 10.0f)) * glm::scale(glm::mat4(1.0), glm::vec3(10.0f, 10.0f, 10.0f));
		lightingShader.setUniform("model", model);

		// Set material properties
		lightingShader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.4f, 0.4f, 0.4f));
		lightingShader.setUniform("material.shininess", 32.0f);

		treeTextures[11].bind(0);		// set the texture before drawing.  Our simple OBJ mesh loader does not do materials yet.
		trees[11].draw();			// Render the OBJ mesh
		treeTextures[11].unbind(0);



		// render the axe
		model = glm::translate(glm::mat4(1.0), glm::vec3(18.0f, 4.1f, 10.0f)) * glm::scale(glm::mat4(1.0), glm::vec3(6.0f, 6.0f, 6.0f)) * glm::rotate(glm::mat4(), glm::radians(100.0f), glm::vec3(0.0f, 0.0f, -1.0f)) * glm::rotate(glm::mat4(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		lightingShader.setUniform("model", model);

		// Set material properties
		lightingShader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		axeTexture.bind(0);		// set the texture before drawing.  Our simple OBJ mesh loader does not do materials yet.
		axe.draw();			// Render the OBJ mesh
		axeTexture.unbind(0);



		// render the house
		model = glm::translate(glm::mat4(1.0), glm::vec3(50.0f, 0.0f, 20.0f)) * glm::scale(glm::mat4(1.0), glm::vec3(1.0f, 1.0f, 1.0f)) * glm::rotate(glm::mat4(), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setUniform("model", model);

		// Set material properties
		lightingShader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		houseTexture.bind(0);		// set the texture before drawing.  Our simple OBJ mesh loader does not do materials yet.
		house.draw();			// Render the OBJ mesh
		houseTexture.unbind(0);


		// render the trees
		for (int i = 0; i < number_of_trees; i++)
		{
			model = glm::translate(glm::mat4(1.0), treePos[i]) * glm::scale(glm::mat4(1.0), treeScale[treesNum[i]]) * glm::rotate(glm::mat4(), glm::radians(tree_rotation[i]), glm::vec3(0.0f, 1.0f, 0.0f));
			lightingShader.setUniform("model", model);

			// Set material properties
			lightingShader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
			lightingShader.setUniformSampler("material.diffuseMap", 0);
			lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
			lightingShader.setUniform("material.shininess", 32.0f);

			treeTextures[treesNum[i]].bind(0);		// set the texture before drawing.  Our simple OBJ mesh loader does not do materials yet.
			trees[treesNum[i]].draw();			// Render the OBJ mesh
			treeTextures[treesNum[i]].unbind(0);
		}




		// render the grass
		for (int i = 0; i < number_of_grasses; i++)
		{
			model = glm::translate(glm::mat4(1.0), grassPos[i]) * glm::scale(glm::mat4(1.0), grassScale[grassNum[i]]) * glm::rotate(glm::mat4(), glm::radians(grass_rotation[i]), glm::vec3(0.0f, 1.0f, 0.0f));
			lightingShader.setUniform("model", model);

			// Set material properties
			lightingShader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
			lightingShader.setUniformSampler("material.diffuseMap", 0);
			lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
			lightingShader.setUniform("material.shininess", 32.0f);

			grass_texture[grassNum[i]].bind(0);		// set the texture before drawing.  Our simple OBJ mesh loader does not do materials yet.
			grass[grassNum[i]].draw();			// Render the OBJ mesh
			grass_texture[grassNum[i]].unbind(0);
		}




		// render the mushrooms
		for (int i = 0; i < number_of_mushrooms; i++)
		{
			model = glm::translate(glm::mat4(1.0), mushroomPos[i]) * glm::scale(glm::mat4(1.0), mushroomScale[mushroomNum[i]]) * glm::rotate(glm::mat4(), glm::radians(mushroom_rotation[i]), glm::vec3(0.0f, 1.0f, 0.0f));
			lightingShader.setUniform("model", model);

			// Set material properties
			lightingShader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
			lightingShader.setUniformSampler("material.diffuseMap", 0);
			lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
			lightingShader.setUniform("material.shininess", 32.0f);

			mushroomTextures[mushroomNum[i]].bind(0);		// set the texture before drawing.  Our simple OBJ mesh loader does not do materials yet.
			mushrooms[mushroomNum[i]].draw();			// Render the OBJ mesh
			mushroomTextures[mushroomNum[i]].unbind(0);
		}


		// render the woods
		for (int i = 0; i < number_of_woods; i++)
		{
			model = glm::translate(glm::mat4(1.0), woodPos[i]) * glm::scale(glm::mat4(1.0), woodScale[woodNum[i]]) * glm::rotate(glm::mat4(), glm::radians(wood_rotation[i]), glm::vec3(0.0f, 1.0f, 0.0f));
			lightingShader.setUniform("model", model);

			// Set material properties
			lightingShader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
			lightingShader.setUniformSampler("material.diffuseMap", 0);
			lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
			lightingShader.setUniform("material.shininess", 32.0f);

			woodTextures[woodNum[i]].bind(0);		// set the texture before drawing.  Our simple OBJ mesh loader does not do materials yet.
			woods[woodNum[i]].draw();			// Render the OBJ mesh
			woodTextures[woodNum[i]].unbind(0);
		}


		// Swap front and back buffers
		glfwSwapBuffers(gWindow);

		lastTime = currentTime;
	}

	glfwTerminate();

	return 0;
}

//-----------------------------------------------------------------------------
// Initialize GLFW and OpenGL
//-----------------------------------------------------------------------------
bool initOpenGL()
{
	// Intialize GLFW 
	// GLFW is configured.  Must be called before calling any GLFW functions
	if (!glfwInit())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// forward compatible with newer versions of OpenGL as they become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3


	// Create an OpenGL 3.3 core, forward compatible context window
	gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	if (gWindow == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Make the window's context the current one
	glfwMakeContextCurrent(gWindow);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return false;
	}

	// Set the required callback functions
	glfwSetKeyCallback(gWindow, glfw_onKey);
	glfwSetFramebufferSizeCallback(gWindow, glfw_onFramebufferSize);
	glfwSetScrollCallback(gWindow, glfw_onMouseScroll);

	// Hides and grabs cursor, unlimited movement
	glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

	glClearColor(gClearColor.r, gClearColor.g, gClearColor.b, gClearColor.a);

	// Define the viewport dimensions
	int w, h;
	glfwGetFramebufferSize(gWindow, &w, &h); // For retina display
	glViewport(0, 0, w, h);

	//    glViewport(0, 0, gWindowWidth, gWindowHeight);

	glEnable(GL_DEPTH_TEST);

	return true;
}

//-----------------------------------------------------------------------------
// Is called whenever a key is pressed/released via GLFW
//-----------------------------------------------------------------------------
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
	{
		gWireframe = !gWireframe;
		if (gWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		// toggle the flashlight
		gFlashlightOn = !gFlashlightOn;
	}
}

//-----------------------------------------------------------------------------
// Is called when the window is resized
//-----------------------------------------------------------------------------
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height)
{
	gWindowWidth = width;
	gWindowHeight = height;

	// Define the viewport dimensions
	int w, h;
	glfwGetFramebufferSize(gWindow, &w, &h); // For retina display
	glViewport(0, 0, w, h);

	//    glViewport(0, 0, gWindowWidth, gWindowHeight);

}

//-----------------------------------------------------------------------------
// Called by GLFW when the mouse wheel is rotated
//-----------------------------------------------------------------------------
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY)
{
	double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSITIVITY;

	fov = glm::clamp(fov, 1.0, 120.0);

	fpsCamera.setFOV((float)fov);
}

//-----------------------------------------------------------------------------
// Update stuff every frame
//-----------------------------------------------------------------------------
void update(double elapsedTime)
{
	// Camera orientation
	double mouseX, mouseY;

	// Get the current mouse cursor position delta
	glfwGetCursorPos(gWindow, &mouseX, &mouseY);

	// Rotate the camera the difference in mouse distance from the center screen.  Multiply this delta by a speed scaler
	fpsCamera.rotate((float)(gWindowWidth / 2.0 - mouseX) * MOUSE_SENSITIVITY , (float)(gWindowHeight / 2.0 - mouseY) * MOUSE_SENSITIVITY);

	// Clamp mouse cursor to center of screen
	glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

	// Camera FPS movement

	// Forward/backward
	if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getLook());
	else if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getLook());

	// Strafe left/right
	if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getRight());
	else if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getRight());

	// Up/down
	if (glfwGetKey(gWindow, GLFW_KEY_Z) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * glm::vec3(0.0f, 1.0f, 0.0f));
	else if (glfwGetKey(gWindow, GLFW_KEY_X) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -glm::vec3(0.0f, 1.0f, 0.0f));
}

//-----------------------------------------------------------------------------
// Code computes the average frames per second, and also the average time it takes
// to render one frame.  These stats are appended to the window caption bar.
//-----------------------------------------------------------------------------
void showFPS(GLFWwindow* window)
{
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime(); // returns number of seconds since GLFW started, as double float

	elapsedSeconds = currentSeconds - previousSeconds;

	// Limit text updates to 4 times per second
	if (elapsedSeconds > 0.25)
	{
		previousSeconds = currentSeconds;
		double fps = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		// The C++ way of setting the window title
		std::ostringstream outs;
		outs.precision(3);	// decimal places
		outs << std::fixed
			<< APP_TITLE << "    "
			<< "FPS: " << fps << "    "
			<< "Frame Time: " << msPerFrame << " (ms)";
		glfwSetWindowTitle(window, outs.str().c_str());

		// Reset for next average.
		frameCount = 0;
	}

	frameCount++;
}
