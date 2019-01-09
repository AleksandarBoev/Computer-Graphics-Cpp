#include "stdafx.h"
#include <GLFW/glfw3.h>

void moveXCoordinates2(float vertices[], float value, int arraySize)
{
	//gotta move [0], [3] and [6]
	int numberOfExes = arraySize / 3;

	for (int i = 0; i < arraySize; i++)
	{
		vertices[0 + i * 3] += value;
	}
}

void moveYCoordinates2(float vertices[], float value, int arraySize)
{
	//gotta move [1], [4] and [7]
	int numberOfY = arraySize / 3;

	for (int i = 0; i < arraySize; i++)
	{
		vertices[1 + i * 3] += value;
	}
}

void processInput2(GLFWwindow *window, float vertices[], int size)
{
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		moveXCoordinates2(vertices, 0.007f, 9);
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		moveXCoordinates2(vertices, -0.007f, 9);
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		moveYCoordinates2(vertices, 0.007f, 9);
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		moveYCoordinates2(vertices, -0.007f, 9);
	}
}
