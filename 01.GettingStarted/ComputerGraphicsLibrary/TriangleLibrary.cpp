#include "stdafx.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

void processTriangleColors(GLFWwindow *window, float& red, float& green, float& blue)
{
	//Q = +red, A = -red, W = +green, S = -green, E = +blue, D = -blue
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (red <= 0.0)
			return;
		else
			red -= 0.0007;
	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		if (red >= 1.0)
			return;
		else
			red += 0.0007;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		if (green <= 0.0)
			return;
		else
			green -= 0.0007;
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		if (green >= 1.0)
			return;
		else
			green += 0.0007;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (blue <= 0.0)
			return;
		else
			blue -= 0.0007;
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		if (blue >= 1.0)
			return;
		else
		{
			blue += 0.0007;
		}
	}
}

void generateTriangle(unsigned int& VAO, unsigned int& shaderProgram)
{
	unsigned int vertexShader = 1;
	glCompileShader(vertexShader);//TODO
}