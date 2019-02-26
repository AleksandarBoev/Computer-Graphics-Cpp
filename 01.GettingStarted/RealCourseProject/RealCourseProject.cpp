#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

const char *transformationVertexShaderSource =
"#version 450 core\n"
"layout(location = 0) in vec3 aPos;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"	gl_Position = transform * vec4(aPos, 1.0f);\n"
"}\0";

/*
	"ourColor" of type vec4 (a vector/array with 4 elements of type float) is a global variable
for the shader program.
*/
const char *fragmentShaderSource = "#version 450 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

void processInput(GLFWwindow *window);
void processInput(GLFWwindow *window, float& x, float& y, float speed, float& scale1, float& scale2, float& scale3,
	float& rotateValue, int transformLoc);
void processWindowColors(GLFWwindow *window, float& red, float& green, float& blue);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processColors(GLFWwindow *window, float& red, float& green, float& blue, float value, float& timePressed);

int main()
{
	std::cout << "Q, A, S, W, E, D - change object color" << std::endl
		<< "O, P - rotate object" << std::endl
		<< "Z, X, C, V - change object scales" << std::endl
		<< "Arrow keys - move object" << std::endl
		<< "Num Lock numbers 1-6 - change window color";
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &transformationVertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return -1;
	}

	//fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return -1;
	}

	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//vao and vbo
	float vertices[] = {
		// positions          // texture coords
		 0.3f,  0.3f, 0.0f,
		 0.3f, -0.3f, 0.0f,
		-0.3f, -0.3f, 0.0f,
		-0.3f,  0.3f, 0.0f
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	int numberOfIndices = 6;
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

	glUseProgram(shaderProgram); //make this program the current main one
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); //get the location of the uniform variable

	float objectRed = 1.0f, objectGreen = 1.0f, objectBlue = 1.0f;
	float windowRed = 0.0f, windowGreen = 0.0f, windowBlue = 0.0f;

	float timePressed = 0.0f;

	unsigned int transformLocation = glGetUniformLocation(shaderProgram, "transform");

	float x = 0.0, y = 0.0;
	float scale1 = 1.0, scale2 = 1.0, scale3 = 1.0;
	float rotateValue = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);
		processInput(window, x, y, 0.0007f, scale1, scale2, scale3, rotateValue, transformLocation);
		processWindowColors(window, windowRed, windowGreen, windowBlue);
		float currentTime = glfwGetTime();
		if (glfwGetTime() - timePressed > 0.5f)
		{
			processColors(window, objectRed, objectGreen, objectBlue, 0.17f, timePressed);
			glUniform4f(vertexColorLocation, objectRed, objectGreen, objectBlue, 1.0f);
		}

		// render
		// clear the colorbuffer
		glClearColor(windowRed, windowGreen, windowBlue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0); //GL_TRIANGLES or GL_LINES or GL_LINE_LOOP

		// swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void processColors(GLFWwindow *window, float& red, float& green, float& blue, float value, float& timePressed)
{
	//These are the num keys (1, 2, 3, 4, 5, 6)
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		timePressed = glfwGetTime();
		if (red <= 0.0)
			return;
		else
			red -= value;
	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		timePressed = glfwGetTime();
		if (red >= 1.0)
			return;
		else
			red += value;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		timePressed = glfwGetTime();
		if (green <= 0.0)
			return;
		else
			green -= value;
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		timePressed = glfwGetTime();
		if (green >= 1.0)
			return;
		else
			green += value;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		timePressed = glfwGetTime();
		if (blue <= 0.0)
			return;
		else
			blue -= value;
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		timePressed = glfwGetTime();
		if (blue >= 1.0)
			return;
		else
		{
			blue += value;
		}
	}
}

void processInput(GLFWwindow *window, float& x, float& y, float speed, float& scale1, float& scale2, float& scale3,
	float& rotateValue, int transformLoc)
{
	//NEEDS to be here. Tried initializing it outside the cicle and passing it as a parameter, but
	//for some reason the program does not draw the object unless the transform variable is initialized 
	//for each cicle. The transformLoc variable does not share this problem and can be outside the cicle.
	glm::mat4 transform2 = glm::mat4(1.0f);

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		x += speed;
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		x -= speed;
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		y += speed;
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		y -= speed;
	else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		scale1 -= speed;
	else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		scale1 += speed;
	else if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		scale2 -= speed;
	else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		scale2 += speed;
	else if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		scale3 -= speed;
	else if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		scale3 += speed;
	else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		rotateValue += speed;
	else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		rotateValue -= speed;

	transform2 = glm::scale(transform2, glm::vec3(scale1, scale2, scale3));
	transform2 = glm::translate(transform2, glm::vec3(x, y, 1.0));
	transform2 = glm::rotate(transform2, rotateValue, glm::vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform2));
}

void processWindowColors(GLFWwindow *window, float& red, float& green, float& blue)
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
