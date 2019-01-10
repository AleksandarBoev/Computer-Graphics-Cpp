#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include "ComputerGraphicsLibrary.cpp"
#include <iostream>

int main()
{
	//TODO learn about EBO and how to use it to generate more complex figures
	GLFWwindow* window = initializeWindow(800, 600, "More Complex Figures Window", 0.0, 0.0, 0.0);

	int shaderProgram = generateBasicShaderProgram();

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = { 
		 -0.4f, -0.3f, 0.0f,
		 0.0f, 0.8f, 0.0f,
		 0.4f, -0.3f, 0.0f,
		 -0.5f, 0.4f, 0.0f,
		 0.5f, 0.4f, 0.0,
	};

	/*
	original vertices:
		 0.5f,  0.5f, 0.0f,  // top right    0
		 0.5f, -0.5f, 0.0f,  // bottom right 1
		-0.5f, -0.5f, 0.0f,  // bottom left  2
		-0.5f,  0.5f, 0.0f   // top left     3

		original indices
		0, 1, 
		3, 1, 
		2, 3   

		Lines are drawn in pairs. Which would mean that from the above original indices the lines would be:
		line 1 - between coordinates 0 and 1 (0.5f,  0.5f, 0.0f and 0.5f, -0.5f, 0.0f)
		line 2 - between coordinates 3 and 1 (-0.5f,  0.5f, 0.0f and 0.5f, -0.5f, 0.0f)
		line 3 - between coordinates 2 and 3 (-0.5f, -0.5f, 0.0f and -0.5f,  0.5f, 0.0f)

		Note that in the line "glDrawElements(typeOfDraw, numberOfIndices, GL_UNSIGNED_INT, 0);"
		the "typeOfDraw" is important. Build the indices according to the "typeOfDraw" you are going to use,
		because the data is interpret differently.
	*/
	unsigned int indices[] = {  
		0, 2, 4, 3, 1, 0
	};
	int numberOfIndices = 6;

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object (VAO) first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	//Make this buffer the current main GL_ARRAY_BUFFER.
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 

	//Settings for the main current buffer of type GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Make this buffer the current main GL_ELEMENT_ARRAY_BUFFER.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//Settings for the main current buffer of type GL_ELEMENT_ARRAY_BUFFER
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Connects the VBO and EBO with the VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	
	indices[0] = 99; //doesn't do anything
	while (!glfwWindowShouldClose(window))
	{
		processInputEndProgram(window);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//glDrawArrays(GL_TRIANGLES, 0, 6); //draw, using the VBO

		/*
		Draw using the EBO, meaning using the indices. The VAO contains an array of coordinates and an array
		of indices. The indices indicate which vertex from the array of coordinates should be drawn.
		To see how the lines are made in GL_TRIANGLES, GL_TRIANGLE_FAN and GL_TRIANGLE_STRIP - uncomment the glPolygonMode
		Types of drawing:
		GL_LINE_STRIP - connects the points one after another
		GL_TRIANGLES - connects points three by three and fills them
		GL_LINES - connects the points two by two
		GL_TRIANGLE_FAN - connects the points one after another AND three by three. 
			Three by three means - makes an additional connection between index i and index i + 2.
			And finally - fills between the lines
			
		GL_TRIANGLE_STRIP - connects point 0, 1, 2 and fills. Then 1, 2, 3 and fills. Then 2, 3, 4 and fills
			If the indices were {0, 2, 4, 3, 1, 0} then it would connect vertices 0, 2, 4 and fill.
			Then it would connect vertices 2, 4, 3 and fill. 
			Then 4, 3, 1 and fill. And finally - vertices 3, 1, 0 and fill
		*/
		glDrawElements(GL_LINE_STRIP, numberOfIndices, GL_UNSIGNED_INT, 0); //GL_TRIANGLES or GL_LINES or GL_LINE_LOOP

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}
