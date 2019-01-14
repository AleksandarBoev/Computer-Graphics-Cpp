#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
class CgWindow
{
private:
	int width, height;
	std::string windowName;
	float red, green, blue;
	GLFWwindow* window;
	int redIncreaseButton, redDecreaseButton, greenIncreaseButton,
		greenDecreaseButton, blueIncreaseButton, blueDecreaseButton;
	int exitButton;

	void checkAndCorrectColors();
public:
	CgWindow(int width, int height, std::string windowName, float red, float green, float blue);
	~CgWindow();

	GLFWwindow* getWindow() const;

	void refresh();
	
	void changeWindowColor(float red, float green, float blue);

	void setButtons(int redIncreaseButton, int redDecreaseButton, int greenIncreaseButton,
		int greenDecreaseButton, int blueIncreaseButton, int blueDecreaseButton);

	void processInput(float value);

	void setExitButton(int exitButton);
};

