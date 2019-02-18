#include <glad/glad.h> //include glad before everything else, glad will have all the openGL headers included already
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
//#include "Model2.h"


/*
ACRONYM LIBRARY:
VBO = vertex buffer object
VAO = Vertex array object
EBO = Element buffer object


*/






//window resize call back
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//mouse move callback
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

//wheel scroll callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//user inputs
void processInput(GLFWwindow *window);

unsigned int loadTexture(char const * path);


//Camera Details
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = true;
float fov = 45.0f;

//My lamps position
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightColour(1.0f, 1.0f, 1.0f);

//mouse details
float lastX = 400, lastY = 300;

//time management
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float animationtime = 0.0f;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //core means ignore all the backwards compat crap before
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); replace above line with thise to make it work


	//build window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Mother Fucking Triangle", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//initialise GLAD (openGL)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//setup openGL viewport x,y,w,h
	glViewport(0, 0, 800, 600);

	//hide cursor but also capture it inside this window
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//NOTE: openGL viewPort coordinates range -1 to 1 on x and y axis and transform to window size. E.G viewport(0,0) is windows(400,300)
//register callback with window resize func
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//setup mouse move callback
	glfwSetCursorPosCallback(window, mouse_callback);

	//scroll wheel callback
	glfwSetScrollCallback(window, scroll_callback);

	//Configure global opengl state
	glEnable(GL_DEPTH_TEST);//Z buffer depth testing on!


	//Using our shader class
	Shader lightShader("modelShader.vs", "modelShader.fs");
	Shader lampShader("shader6.vs", "lampShader.fs");

	// load models
	// -----------
	Model ourModel("assets/Glock 3d.obj");
	Model suit("assets/nanosuit.obj");
	Model bullet("assets/13517_Beach_Ball_v2_L3.obj");

	//Model ourModel("assets/shareablebullet.obj");



	//set cursor position
	glfwSetCursorPos(window, lastX, lastY);
	//glfw game loop
	while (!glfwWindowShouldClose(window))
	{
		//time management
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		//RENDER SHIT
		//set draw colour
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//clear screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//view matrix
		//glm::mat4 view;
		// note that we're translating the scene in the reverse direction of where we want to move
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); //push all world objects away on z axis
		glm::mat4 view = camera.GetViewMatrix();


		//projection matrix
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);



			//CUBES
		
			lightShader.use();
			lightShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
			lightShader.setVec3("lightColor", lightColour);
			lightShader.setVec3("lightPos", lightPos);
			lightShader.setVec3("viewPos", camera.Position);


			

			

			glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			// render the loaded model
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 1.45f, 4.0f)); //set up hand gun pos
			model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));			// smaller the size
			lightShader.setMat4("model", model);
			ourModel.Draw(lightShader);

			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::translate(model2, glm::vec3(0.0f, -1.45f, 0.0f)); //set up suit  pos
			model2 = glm::scale(model2, glm::vec3(0.2f, 0.2f, 0.2f));	
			lightShader.setMat4("model", model2);
			suit.Draw(lightShader);

			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { // add button press
				animationtime = currentFrame + 8.0f;
				bullet.pos.x = 0.0f;   //set ball pos x, y, z
				bullet.pos.y = 1.45f;
				bullet.pos.z = 3.0f;
			}
		
			
			if (animationtime > currentFrame) {
				bullet.pos.z -= 1.0f*deltaTime; //add animation
				glm::mat4 model3 = glm::mat4(1.0f); //ball fly
				
				model3 = glm::translate(model3, bullet.pos);
				model3 = glm::scale(model3, glm::vec3(0.002f, 0.002f, 0.002f));	
				lightShader.setMat4("model", model3);
				bullet.Draw(lightShader);
				animationtime -= deltaTime;
				if (animationtime < currentFrame)
					bullet.pos.y = -0.7f;
			}
			
			



			
		//LIGHT/LAMP
			lampShader.use();
			lampShader.setVec3("lightColor", lightColour);
	
		//Input
		glfwPollEvents();
		//swap buffers to show completed rendered screen
		glfwSwapBuffers(window);
	}




	//kill window propers
	glfwTerminate();

	//system("pause");

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		lightColour.r += deltaTime;
		lightColour.g += deltaTime;
		lightColour.b += deltaTime;
		if (lightColour.r > 1)
			lightColour.r = 1;
		if (lightColour.g > 1)
			lightColour.g = 1;
		if (lightColour.b > 1)
			lightColour.b = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		lightColour.r -= deltaTime;
		lightColour.g -= deltaTime;
		lightColour.b -= deltaTime;
		if (lightColour.r < 0)
			lightColour.r = 0;
		if (lightColour.g < 0)
			lightColour.g = 0;
		if (lightColour.b < 0)
			lightColour.b = 0;
	}

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}