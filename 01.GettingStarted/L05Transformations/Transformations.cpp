
#include "ComputerGraphicsLibrary.cpp" //already has the include for the glad and glfw, so no need to include them here
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void processInput(GLFWwindow *window, float& x, float& y, float speed);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	GLFWwindow* window = initializeWindow(900, 900, "Transformations", 0.0, 0.0, 0.0);

	// build and compile our shader zprogram
	// ------------------------------------
	int shaderProgram = generateShaderProgram(transformationVertexShaderSource, basicFragmentShaderSource);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // texture coords
		 0.1f,  0.1f, 0.0f,
		 0.1f, -0.1f, 0.0f,
		-0.1f, -0.1f, 0.0f,
		-0.1f,  0.1f, 0.0f
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	float timePressed = 0.0f;
	float x = 0.0, y = 0.0;
	float depth = 0.0;

	float scale1 = 1.0, scale2 = 1.0, scale3 = 1.0;

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInputEndProgram(window);

		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT);

		// render
		// ------
	//	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//	glClear(GL_COLOR_BUFFER_BIT);

		

		// create transformations
		
		//glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		//transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
		//transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		// get matrix's uniform location and set matrix
		//ourShader.use();
		glUseProgram(shaderProgram);
		unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");

		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		if (true) //glfwGetTime() - timePressed > 0.5f
		{
			glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity ma
			processInput(window, x, y, 0.0007);
			transform = glm::translate(transform, glm::vec3(x, y, 1.0));
			//x -= 0.00001;
			//depth += 0.00005;
			//transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

			//scale 1 looks like it is stretching/flattening on the horizontal. And scale 2 same for the vertical
		//	transform = glm::scale(transform, glm::vec3(scale1, scale2, 1.0)); 
		//	scale1 -= 0.0001;
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
			timePressed = glfwGetTime();
		}

		// render container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, float& x, float& y, float speed)
{
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		x += speed;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		x -= speed;
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		y += speed;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		y -= speed;
	}
}