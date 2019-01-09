#include "stdafx.h"
#include <GLFW/glfw3.h>

void processWindowColor(GLFWwindow *window, float& red, float& green, float& blue)
{
	if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS)
	{
		if (red <= 0.0)
			return;
		else
			red -= 0.0007;
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
	{
		if (red >= 1.0)
			return;
		else
			red += 0.0007;
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
	{
		if (green <= 0.0)
			return;
		else
			green -= 0.0007;
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
	{
		if (green >= 1.0)
			return;
		else
			green += 0.0007;
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS)
	{
		if (blue <= 0.0)
			return;
		else
			blue -= 0.0007;
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
	{
		if (blue >= 1.0)
			return;
		else
		{
			blue += 0.0007;
		}
	}

}