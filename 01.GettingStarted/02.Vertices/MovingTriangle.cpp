#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

/*
	!!!IMPORTANT!!!
	This is just playing around. Moving the triangle around like this is a very costly operation.
	There are much more optimized ways of moving objects around.
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void processInput(GLFWwindow *window, float vertices[], int size);
void moveXCoordinates(float vertices[], float value, int arraySize);
void moveYCoordinates(float vertices[], float value, int arraySize);

void print(float vertices[], int size);

void drawStaticTriangle(GLFWwindow *window, float vertices[], int& shaderProgram, int VAO);
void drawDynamicTriangle(GLFWwindow *window, float vertices[], int arraySize, int& shaderProgram, unsigned int& VAO);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//Shader - a mini program for the videocard. Written in GLSL (OpenGL Shading Language)
const char *vertexShaderSource =
"#version 450 core\n" //version 4.5 of OpenGL
"layout (location = 0) in vec3 aPos;\n" //vec3 --> vector of size 3, each vertex is a 3D coordinate
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" //vec4 is 3D (x, y, z) + "perspective division" (1.0)
"}\0";

const char *fragmentShaderSource =
"#version 450 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n" //color of triangle:
"   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n" //red, green, blue and alpha (opacity). a.k.a. RGBA
"}\n\0";

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Triangle", NULL, NULL);
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
	int vertexShader = glCreateShader(GL_VERTEX_SHADER); //creating a vertex object, referenced by this integer (its id)
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //replace the shader source code to the object via id
	glCompileShader(vertexShader); //compile the code
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
	glDeleteShader(vertexShader); //after linking them to the program, they are no longer needed
	glDeleteShader(fragmentShader);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	//0.0, 0.0 = x, y --> meaning at the center. x = 0.5 means right from the center. y = -0.5 means lower than center
	float vertices[] = { //triangle coordinates
	-0.5f, -0.5f, 0.0f, //bottom left
	 0.5f, -0.5f, 0.0f, //borrom right
	 0.0f,  0.5f, 0.0f //middle top
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	//Dertermines type of buffer in vbo. The arraybuffer is best suited for vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Any settings made on GL_ARRAY_BUFFER affect vbo (obv)
	//Copies the vertices data into the buffer memory
	/*
	GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
	GL_DYNAMIC_DRAW: the data is likely to change a lot.
	GL_STREAM_DRAW: the data will change every time it is drawn.
	*/

	/*
	Param 1 = 0, because "layout (location = 0) in vec3 aPos;\n" in the vertex shader source code
	Param 2 = 3, because of vec3
	Param 3 = GL_FLOAT - float type of data
	Param 4 = should the data be normalized? For now, it is not needed
	Param 5 = The fifth argument is known as the stride and tells us the space between
		consecutive vertex attributes. Since the next set of position data is located exactly
		3 times the size of a float away we specify that value as the stride.
	Param 6 = something weird. Shows where the beginning is.
	Result from this function - specified how OpenGL should interpret the vertex data
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind


	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	//TODO should I bind the vbo of the triangle i want to move around?
	glBindBuffer(GL_ARRAY_BUFFER, 0); //can be unbinded
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //and binded multiple times. Needs to be binded to move the triangle around

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		drawDynamicTriangle(window, vertices, 9, shaderProgram, VAO);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

void drawStaticTriangle(GLFWwindow *window, float vertices[], int& shaderProgram, int VAO)
{
	print(vertices, 9);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind buffer, as it is not going to be used.
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_LINE_STRIP, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0); // no need to unbind it every time 

	   // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	   // -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
void drawDynamicTriangle(GLFWwindow *window, float vertices[], int arraySize, int& shaderProgram, unsigned int& VAO)
{
		processInput(window, vertices, 9);
		//sizeof(vertices) doesn't work in functions, because vertices here is a pointer.
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arraySize, vertices, GL_DYNAMIC_DRAW); 

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time.
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glBindVertexArray(0); //no need to unbind it every time tho. But if this is present, then glBindVertexArray is needed		
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void moveXCoordinates(float vertices[], float value, int arraySize)
{
	//gotta move [0], [3] and [6]
	int numberOfExes = arraySize / 3;

	for (int i = 0; i < arraySize; i++)
	{
		vertices[0 + i * 3] += value;
	}
}
void moveYCoordinates(float vertices[], float value, int arraySize)
{
	//gotta move [1], [4] and [7]
	int numberOfY = arraySize / 3;

	for (int i = 0; i < arraySize; i++)
	{
		vertices[1 + i * 3] += value;
	}
}

/*
Changes coordinates depending on keys pressed
*/
void processInput(GLFWwindow *window, float vertices[], int size)
{
	processInput(window);

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		moveXCoordinates(vertices, 0.007f, 9);
		print(vertices, size);
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		moveXCoordinates(vertices, -0.007f, 9);
		print(vertices, size);
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		moveYCoordinates(vertices, 0.007f, 9);
		print(vertices, size);
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		moveYCoordinates(vertices, -0.007f, 9);
		print(vertices, size);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void print(float vertices[], int size)
{
	for (int i = 0; i < size; i++)
	{
		char currentCoordinate;
		switch (i % 3)
		{
		case 0:
			currentCoordinate = 'x';
			break;

		case 1:
			currentCoordinate = 'y';
			break;

		case 2:
			currentCoordinate = 'z';
			break;
		}
		std::cout << currentCoordinate << i / 3 + 1 << " = " << vertices[i] << " ";
	}
	std::cout << std::endl;
}