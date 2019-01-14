#include "CgWindow.h"

void CgWindow::checkAndCorrectColors()
{
	if (this->red > 1.0)
		this->red = 1.0;
	else if (this->red < 0.0)
		this->red = 0.0;

	if (this->green > 1.0)
		this->green = 1.0;
	else if (this->green < 0.0)
		this->green = 0.0;

	if (this->blue > 1.0)
		this->blue = 1.0;
	else if (this->blue < 0.0)
		this->blue = 0.0;
}

CgWindow::CgWindow(int width, int height, std::string windowName, float red, float green, float blue) :
	width(width),
	height(height),
	windowName(windowName),
	red(red),
	green(green),
	blue(blue),
	redIncreaseButton(GLFW_KEY_KP_4),
	redDecreaseButton(GLFW_KEY_KP_1),
	greenIncreaseButton(GLFW_KEY_KP_5),
	greenDecreaseButton(GLFW_KEY_KP_2),
	blueIncreaseButton(GLFW_KEY_KP_6),
	blueDecreaseButton(GLFW_KEY_KP_3),
	exitButton(GLFW_KEY_ESCAPE)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(this->width, this->height, this->windowName.c_str(), NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cout << "Failed to initialize GLAD" << std::endl;
	

	this->checkAndCorrectColors();

	glClearColor(this->red, this->green, this->blue, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);

	this->window = window;
}


CgWindow::~CgWindow()
{
	delete this->window;
}



GLFWwindow* CgWindow::getWindow() const
{
	return this->window;
}

void CgWindow::refresh()
{
	glClearColor(this->red, this->green, this->blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(this->window);
	glfwPollEvents();
}

void CgWindow::changeWindowColor(float red, float green, float blue)
{
	this->red = red;
	this->green = green;
	this->blue = blue;

	this->checkAndCorrectColors();
	//glClearColor(red, green, blue, 1.0f);
}

void CgWindow::setButtons(int redIncreaseButton, int redDecreaseButton, int greenIncreaseButton,
	int greenDecreaseButton, int blueIncreaseButton, int blueDecreaseButton)
{
	this->redIncreaseButton = redIncreaseButton;
	this->redIncreaseButton = redIncreaseButton;

	this->greenIncreaseButton = greenIncreaseButton;
	this->greenIncreaseButton = greenIncreaseButton;

	this->blueIncreaseButton = blueIncreaseButton;
	this->blueIncreaseButton = blueIncreaseButton;
}


void CgWindow::processInput(float value)
{
	if (glfwGetKey(this->window, this->exitButton) == GLFW_PRESS)
		glfwSetWindowShouldClose(this->window, true);

	if (glfwGetKey(this->window, this->redDecreaseButton) == GLFW_PRESS)
		this->red -= value;
	else if (glfwGetKey(this->window, this->redIncreaseButton) == GLFW_PRESS)
		this->red += value;
	else if (glfwGetKey(this->window, this->greenDecreaseButton) == GLFW_PRESS)
		this->green -= value;
	else if (glfwGetKey(this->window, this->greenIncreaseButton) == GLFW_PRESS)
		this->green += value;
	else if (glfwGetKey(this->window, this->blueDecreaseButton) == GLFW_PRESS)
		this->blue -= value;
	else if (glfwGetKey(this->window, this->blueIncreaseButton) == GLFW_PRESS)
		this->blue += value;

	this->checkAndCorrectColors();
}

void CgWindow::setExitButton(int exitButton)
{
	this->exitButton = exitButton;
}
