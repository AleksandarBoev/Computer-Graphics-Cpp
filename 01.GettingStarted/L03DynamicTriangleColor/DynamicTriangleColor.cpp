#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include <iostream>

const char *vertexShaderSource = "#version 450 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
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
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processColors(GLFWwindow *window, float& red, float& green, float& blue, float value, float& timePressed);

int main()
{
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
	float triangle[] = {
		0.5f, 1.0f, 0.0f,  // left
		-0.5f, 1.0f, 0.0f,  // right
		0.0f, 0.0f, 0.0f   // top 
	};

	unsigned int VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO); //set this buffer as current main one
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

	glBindVertexArray(VAO); //connect this object with the current main vbo
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	
	glEnableVertexAttribArray(0);

	glUseProgram(shaderProgram); //make this program the current main one
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); //get the location of the uniform variable
	glBindVertexArray(VAO); //make this vertex array object the current main one, since only 1 object is drawn

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //set the window colors, which the glClear will use

	float red = 1.0f, green = 0.0f, blue = 0.0f;

	float timePressed = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);
		float currentTime = glfwGetTime();
		if (glfwGetTime() - timePressed > 2.0f)
		{
			processColors(window, red, green, blue, 0.3f, timePressed);
			glUniform4f(vertexColorLocation, red, green, blue, 1.0f);
		}

		// render
		// clear the colorbuffer
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// be sure to activate the shader
		//glUseProgram(shaderProgram); //original code

		// update the uniform color
		//float timeValue = glfwGetTime(); //original
		//float greenValue = sin(timeValue) / 2.0f + 0.5f; //original
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); //original code
		//original function is glUniform. 4f is added because 4 floats are inserted into the uniform, which is at the given position
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); //original
		//glUniform4f(vertexColorLocation, red, green, blue, 1.0f); 

		// now render the triangle
		//glBindVertexArray(VAO); //original code
		glDrawArrays(GL_TRIANGLES, 0, 3);

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
	if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS)
	{
		timePressed = glfwGetTime();
		if (red <= 0.0)
			return;
		else
			red -= value;
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
	{
		timePressed = glfwGetTime();
		if (red >= 1.0)
			return;
		else
			red += value;
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
	{
		timePressed = glfwGetTime();
		std::cout << "Button pressed!" << std::endl;
		if (green <= 0.0)
			return;
		else
			green -= value;
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
	{
		timePressed = glfwGetTime();
		std::cout << "Button pressed!" << std::endl;
		if (green >= 1.0)
			return;
		else
			green += value;
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS)
	{
		timePressed = glfwGetTime();
		if (blue <= 0.0)
			return;
		else
			blue -= value;
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
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
