#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include <iostream>
#include "ComputerGraphicsLibrary.cpp"

const char *vertexShaderSource = "#version 450 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 450 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

void processInput(GLFWwindow *window);

void printArray(float myArray[], int size);

int main()
{
	float windowRed = 0.0, windowGreen = 0.0, windowBlue = 0.0;
	GLFWwindow* window = initializeWindow(800, 600, "Window Name 32", windowRed, windowGreen, windowBlue);

	unsigned int VAO;
	int shaderProgram;

	float triangleCoordinates[] = {
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
	};

	generateTriangle(vertexShaderSource, fragmentShaderSource, triangleCoordinates, sizeof(triangleCoordinates),
		GL_DYNAMIC_DRAW, VAO, shaderProgram);

	while (!glfwWindowShouldClose(window)) //iterates for every frame
	{
		processInput(window);
		
		processInputWindowColor(window, GLFW_KEY_KP_4, GLFW_KEY_KP_1, GLFW_KEY_KP_5, 
			GLFW_KEY_KP_2, GLFW_KEY_KP_6, GLFW_KEY_KP_3, windowRed, windowGreen, windowBlue, 0.0003f);
		
		changeWindowColor(windowRed, windowGreen, windowBlue);

		processInputMove(window, triangleCoordinates, 9, 
			GLFW_KEY_RIGHT, GLFW_KEY_LEFT, GLFW_KEY_UP, GLFW_KEY_DOWN, 0.0007f); //TODO problem
		//printArray(triangleCoordinates, 9);
		
		//glUseProgram(shaderProgram);
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3); //GL_TRIANGLE / GL_LINES / GL_POINTS
		drawTriangle(shaderProgram, VAO, GL_LINE_LOOP, triangleCoordinates, 9);

		//______________________
		glfwSwapBuffers(window); //drawing takes time, so a double buffer is used
		glfwPollEvents(); //triggers the callback events
		//std::cout << glfwGetTime() << std::endl; //slows down the process
	}

	glfwTerminate(); //stop everything that glfw is doing and free up the memory. glfwInitiate() to start again.

	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void printArray(float myArray[], int size)
{
	for (int i = 0; i < size; i++)
		std::cout << myArray[i] << " | ";

	std::cout << std::endl;
}