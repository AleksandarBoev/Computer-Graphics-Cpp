#include "pch.h"
#include <iostream>
#include "Testing.h"

void moveXCoordinates(float vertices[], float value, int arraySize)
{
	//gotta move [0], [3] and [6]
	int numberOfExes = arraySize / 3;

	for (int i = 0; i < arraySize; i++)
	{
		vertices[0 + i * 3] += value;
	}
}

void changeStuff(float vertices[]);

void print(float vertices[]);

float getInfo(float arrayF[]);

int main()
{
	/*
	float vertices[] = { //triangle coordinates
	-0.5f, -0.5f, 0.0f, //bottom left
	 0.5f, -0.5f, 0.0f, //borrom right
	 0.0f,  0.5f, 0.0f //middle top
	};
	print(vertices);
	std::cout << std::endl;
	moveXCoordinates(vertices, 0.09, 9);
	print(vertices);
	*/
	//std::cout << fiveValue;

	float coordinates[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
	std::cout << sizeof(coordinates) << std::endl << getInfo(coordinates);

	return 0;
}

void changeStuff(float vertices[])
{
	for (int i = 0; i < 3; i++)
	{
		vertices[i] = 99.0;
	}
}


void print(float vertices[])
{
	for (int i = 0; i < 9; i++)
		std::cout << vertices[i] << " ";

	std::cout << std::endl;
}

float getInfo(float arrayF[])
{
	return sizeof(arrayF);
}




