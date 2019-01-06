#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include "InitializeWindow.h"
#include <string>

bool initializeWindow(GLFWwindow* window, int width, int height, const char* title)
{
	glfwInit(); //initializes glfw library

	//What these settings do: www.glfw.org/docs/latest/window.html#window_hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //setting for when the "glfwCreateWindow" is called
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //use core-profile and not intermediate
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //just for Mac users

	//Creating a window object and saving its address in a pointer. 
	//First two arguments determine the size of the window (width and height)
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window); //make the context of our window the main context on the current thread
	return true;
}