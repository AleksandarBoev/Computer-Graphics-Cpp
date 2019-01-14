#include "stdafx.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
//#include "ComputerGraphicsLibrary.h"

const char *basicVertexShaderSource =
"#version 450 core\n" //version 4.5 of OpenGL
"layout (location = 0) in vec3 aPos;\n" //vec3 --> vector of size 3, each vertex is a 3D coordinate
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" //vec4 is 3D (x, y, z) + "perspective division" (1.0)
"}\0";

const char *textureAndTransformationVertexShaderSource =
"#version 450 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec2 aTexCoord;\n"
"out vec2 TexCoord;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"	gl_Position = transform * vec4(aPos, 1.0f);\n"
"	TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
"}\0";

const char *transformationVertexShaderSource =
"#version 450 core\n"
"layout(location = 0) in vec3 aPos;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"	gl_Position = transform * vec4(aPos, 1.0f);\n"
"}\0";

const char *basicFragmentShaderSource =
"#version 450 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n" //color of triangle:
"   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n" //red, green, blue and alpha (opacity). a.k.a. RGBA
"}\n\0";

/*
	How to change the color:
	1) The VAO and shader program need to be the current/main ones at the moment of changing the color.
	2) The color location is needed
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
	3) Change the color
	glUniform4f(vertexColorLocation, floatValueRed, floatValueGreen, floatValueBlue, 1.0f);
*/
const char *basicUniformFragmentShaderSource = "#version 450 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";

float basicTriangleCoordinates[] =
{
	-0.5f, -0.5f, 0.0f, //bottom left
	 0.5f, -0.5f, 0.0f, //borrom right
	 0.0f,  0.5f, 0.0f //middle top
};

GLFWwindow* initializeWindow(int width, int height, std::string windowName, float red, float green, float blue);
void processInputEndProgram(GLFWwindow *window);
void changeWindowColor(float red, float green, float blue);
void processInputWindowColor(GLFWwindow* window, int redIncreaseButton, int redDecreaseButton,
	int greenIncreaseButton, int greenDecreaseButton,
	int blueIncreaseButton, int blueDecreaseButton,
	float& red, float& green, float& blue, float value);
void generateTriangle(const char *vertexShaderSource, const char *fragmentShaderSource,
	float triangleCoordinates[], int size, int drawProcessType, unsigned int& VAO, int& shaderProgram);
void drawTriangle(int& shaderProgram, unsigned int& VAO, int drawType, float triangleCoordinates[], int size);
void moveXCoordinates(float vertices[], float value, int arraySize);
void moveYCoordinates(float vertices[], float value, int arraySize);
bool moveXCoordinatesAndCheckIfOutOfBounds(float vertices[], float value, int arraySize);
bool moveYCoordinatesAndCheckIfOutOfBounds(float vertices[], float value, int arraySize);
bool moveXCoordinatesStopBeforeBounds(float vertices[], float value, int arraySize);
bool moveYCoordinatesStopBeforeBounds(float vertices[], float value, int arraySize);
int generateShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
int generateBasicShaderProgram();



GLFWwindow* initializeWindow(int width, int height, std::string windowName, float red, float green, float blue)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, windowName.c_str(), NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}

	if (red > 1.0)
		red = 1.0;
	else if (red < 0.0)
		red = 0.0;

	if (green > 1.0)
		green = 1.0;
	else if (green < 0.0)
		green = 0.0;

	if (blue > 1.0)
		blue = 1.0;
	else if (blue < 0.0)
		blue = 0.0;

	glClearColor(red, green, blue, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);

	return window;
}

void processInputEndProgram(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void changeWindowColor(float red, float green, float blue)
{
	//TODO what if colors are outside the bounds of 0.0 and 1.0? 
	//It should not be this functions task to ensure that they are
	glClearColor(red, green, blue, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);
}

//www.glfw.org/docs/latest/group__keys.html, int value of GLFW_KEY_SPACE = 32
void processInputWindowColor(GLFWwindow* window, int redIncreaseButton, int redDecreaseButton,
	int greenIncreaseButton, int greenDecreaseButton,
	int blueIncreaseButton, int blueDecreaseButton,
	float& red, float& green, float& blue, float value)
{
	if (glfwGetKey(window, redDecreaseButton) == GLFW_PRESS)
	{
		if (red <= 0.0)
			return;
		else
			red -= value;
	}
	else if (glfwGetKey(window, redIncreaseButton) == GLFW_PRESS)
	{
		if (red >= 1.0)
			return;
		else
			red += value;
	}
	else if (glfwGetKey(window, greenDecreaseButton) == GLFW_PRESS)
	{
		if (green <= 0.0)
			return;
		else
			green -= value;
	}
	else if (glfwGetKey(window, greenIncreaseButton) == GLFW_PRESS)
	{
		if (green >= 1.0)
			return;
		else
			green += value;
	}
	else if (glfwGetKey(window, blueDecreaseButton) == GLFW_PRESS)
	{
		if (blue <= 0.0)
			return;
		else
			blue -= value;
	}
	else if (glfwGetKey(window, blueIncreaseButton) == GLFW_PRESS)
	{
		if (blue >= 1.0)
			return;
		else
		{
			blue += value;
		}
	}

}

void generateTriangle(const char *vertexShaderSource, const char *fragmentShaderSource,
	float triangleCoordinates[], int size, int drawProcessType, unsigned int& VAO, int& shaderProgram)
{
	shaderProgram = generateShaderProgram(vertexShaderSource, fragmentShaderSource);

	//Creating and setting up the VBO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO); //make this buffer the current main one
	//drawProcessType = GL_STATIC_DRAW or GL_DYNAMIC_DRAW
	glBufferData(GL_ARRAY_BUFFER, size, triangleCoordinates, drawProcessType);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);// Vertex attributes stay the same
	glEnableVertexAttribArray(0);
}

void drawTriangle(int& shaderProgram, unsigned int& VAO, int drawType, float triangleCoordinates[], int size)
{
	//for this to work, the VBO needs to be active
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, triangleCoordinates, GL_DYNAMIC_DRAW);

	glUseProgram(shaderProgram);

	glBindVertexArray(VAO);
	glDrawArrays(drawType, 0, 3); //GL_TRIANGLE / GL_LINES / GL_POINTS
	glBindVertexArray(0);
}

void moveXCoordinates(float vertices[], float value, int arraySize)
{
	//gotta move [0], [3], [6], [9] and so on...
	int numberOfExes = arraySize / 3;

	for (int i = 0; i < arraySize; i++)
	{
		vertices[0 + i * 3] += value;
	}
}

void moveYCoordinates(float vertices[], float value, int arraySize)
{
	//gotta move [1], [4], [7], [10] and so on...
	int numberOfY = arraySize / 3;

	for (int i = 0; i < arraySize; i++)
	{
		vertices[1 + i * 3] += value;
	}
}
/*
	!!!IMPORTANT!!!
	This is just playing around. Moving the triangle around like this is a very costly operation.
	There are much more optimized ways of moving objects around.
*/
//TODO maybe add a function parameter which will be how to move the coordinates (normally/until out of bounds/in bounds)
void processInputMove(GLFWwindow *window, float vertices[], int size,
	int moveRightButton, int moveLeftButton, int moveUpButton, int moveDownButton, float moveSpeed)
{
	if (glfwGetKey(window, moveRightButton) == GLFW_PRESS) //GLFW_KEY_RIGHT
	{
		moveXCoordinates(vertices, moveSpeed, size);
	}
	else if (glfwGetKey(window, moveLeftButton) == GLFW_PRESS) //GLFW_KEY_LEFT
	{
		moveXCoordinates(vertices, -1 * moveSpeed, size);
	}
	else if (glfwGetKey(window, moveUpButton) == GLFW_PRESS) //GLFW_KEY_UP
	{
		moveYCoordinates(vertices, moveSpeed, size);
	}
	else if (glfwGetKey(window, moveDownButton) == GLFW_PRESS) //GLFW_KEY_DOWN
	{
		moveYCoordinates(vertices, -1 * moveSpeed, size);
	}
}

bool moveXCoordinatesAndCheckIfOutOfBounds(float vertices[], float value, int arraySize)
{
	int numberOfExes = arraySize / 3;
	bool allXCoordinatesOutOfBounds = true;

	for (int i = 0; i < arraySize; i++)
	{
		vertices[0 + i * 3] += value;
		if (vertices[0 + i * 3] > 0.0 && vertices[0 + i * 3] < 1.0)
			allXCoordinatesOutOfBounds = false;
	}

	return allXCoordinatesOutOfBounds;
}

/*
	How to be used inside of rendering iteration:
	bool flag = false;
	while (condition)
	{
	...
		while (!flag)
			flag = moveYCoordinates(parameters);
	...
	}
*/
bool moveYCoordinatesAndCheckIfOutOfBounds(float vertices[], float value, int arraySize)
{
	int numberOfY = arraySize / 3;
	bool allYCoordinatesOutOfBounds = true;

	for (int i = 0; i < arraySize; i++)
	{
		vertices[1 + i * 3] += value;
		if (vertices[1 + i * 3] > 0.0 && vertices[1 + i * 3] < 1.0) //is in bounds
			allYCoordinatesOutOfBounds = false;
	}

	return allYCoordinatesOutOfBounds;
}

/*
	Checks if changing coordinates would set the object out of bounds. If it does, the coordinates
	stay the same and FALSE is returned
*/
bool moveXCoordinatesStopBeforeBounds(float vertices[], float value, int arraySize)
{
	int numberOfExes = arraySize / 3;
	bool outOfBounds = false;

	for (int i = 0; i < arraySize; i++)
	{
		if (vertices[0 + i * 3] + value < 0.0 && vertices[0 + i * 3] + value > 1.0)
			outOfBounds = true;
	}

	if (!outOfBounds)
		moveXCoordinates(vertices, value, arraySize);

	return outOfBounds;
}

/*
	Checks if changing coordinates would set the object out of bounds. If it does, the coordinates
	stay the same and FALSE is returned
*/
bool moveYCoordinatesStopBeforeBounds(float vertices[], float value, int arraySize)
{
	int numberOfY = arraySize / 3;
	bool outOfBounds = false;

	for (int i = 0; i < arraySize; i++)
	{
		if (vertices[1 + i * 3] + value < 0.0 && vertices[1 + i * 3] + value > 1.0)
			outOfBounds = true;
	}

	if (!outOfBounds)
		moveYCoordinates(vertices, value, arraySize);

	return outOfBounds;
}

int generateBasicShaderProgram()
{
	return generateShaderProgram(basicVertexShaderSource, basicFragmentShaderSource);
}

int generateShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

