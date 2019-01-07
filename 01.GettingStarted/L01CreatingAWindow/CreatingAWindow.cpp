#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include <iostream>
//#include "InitializeWindow.h"
#include <string>

//For when the user resizes the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow *window);

float calculateColor(int colorValue);

void processColors(GLFWwindow *window, float& red, float& green, float& blue);

int main()
{
	std::cout << "You can control colors!" << std::endl
		<< "Num1 lowers red color strength, Num4 increases red color strength." << std::endl
		<< "Same goes for Num2 and Num5 for green and Num3 and Num6 for blue" << std::endl;
		//<< "Enter a name for the window:";

	float redValue = 0.0;
	float greenValue = 0.0;
	float blueValue = 0.0;
	/*
	std::string windowNameInput;
	std::getline(std::cin, windowNameInput);
	const char* windowName = windowNameInput.c_str();
	//string::c_str

	GLFWwindow* window;
	bool successfulInitialize = initializeWindow(window, 800, 600, windowName);

	if (!successfulInitialize)
	{
		std::cout << std::endl << "Failed to create window!";
	}
	*/
	glfwInit(); //initializes glfw library

	//What these settings do: www.glfw.org/docs/latest/window.html#window_hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //setting for when the "glfwCreateWindow" is called
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //use core-profile and not intermediate
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //just for Mac users

	int startingWidth = 800, startingHeight = 600;

	//Creating a window object and saving its address in a pointer. 
	//First two arguments determine the size of the window (width and height)
	GLFWwindow* window = glfwCreateWindow(startingWidth, startingHeight, "OpenGL Window name", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); //make the context of our window the main context on the current thread
	

	//glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	//GLAD manages function pointers for OpenGL. glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Determines window rectable for rendering. Would be nice to have the same values as the window size.
	glViewport(0, 0, 800, 600); //x, y, width, height. x = 0 --> leftmost, y = 0 --> bottom. values are [-1, 1]

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	//Every time the "window" is resized, call this function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Stop the window from closing automatically
	while (!glfwWindowShouldClose(window)) //iterates for every frame
	{
		processColors(window, redValue, greenValue, blueValue);

		//The first three parameters determine the RGB values. 
		//0.4 * 255 = 102. Same for other two. 
		//To see rgb values of colors: www.rapidtables.com/web/color/RGB_Color.html
		glClearColor(redValue, greenValue, blueValue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//______________________
		glfwSwapBuffers(window); //drawing takes time, so a double buffer is used
		glfwPollEvents(); //triggers the callback events
		//std::cout << glfwGetTime() << std::endl; //slows down the process
	}

	glfwTerminate(); //stop everything that glfw is doing and free up the memory. glfwInitiate() to start again.

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

float calculateColor(int colorValue)
{
	return colorValue / (float)255;
}

//Here are all the keys, that can be used: www.glfw.org/docs/latest/group__keys.html
void processColors(GLFWwindow *window, float& red, float& green, float& blue)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

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
	} else if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
	{
		if (green <= 0.0)
			return;
		else
			green -= 0.0007;
	} else if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
	{
		if (green >= 1.0)
			return;
		else
			green += 0.0007;
	} else if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS)
	{
		if (blue <= 0.0)
			return;
		else
			blue -= 0.0007;
	} else if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
	{
		if (blue >= 1.0)
			return;
		else
		{
			blue += 0.0007;
		}
	}
	
}
//		glfwGetKey(window, GLFW_KEY_KP_0) == GLFW_PRESS)

