#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include "ComputerGraphicsLibrary.cpp"

int main()
{
	//TODO learn about EBO and how to use it to generate more complex figures
	GLFWwindow* window = initializeWindow(800, 600, "More Complex Figures Window", 0.0, 0.0, 0.0);

	unsigned int VAO;
	int shaderProgram;

	generateTriangle(basicVertexShaderSource, basicFragmentShaderSource, basicTriangleCoordinates, 9, 
		GL_STATIC_DRAW, VAO, shaderProgram);

	while (!glfwWindowShouldClose(window))
	{
		processInputEndProgram(window);

		drawTriangle(shaderProgram, VAO, GL_TRIANGLES, basicTriangleCoordinates, 9);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}
