#include "CgWindow.h"

int main()
{
	float red = 0.0f;
	CgWindow wat(800, 600, "Course project", red, 0.0f, 0.0f);
	
	while (!glfwWindowShouldClose(wat.getWindow()))
	{
		wat.processInput(0.0002f);
		wat.refresh();
	}


	return 0;
}
