#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	// Initialize glew
	if (glewInit() != GLEW_OK) {
		std::cout << "Error" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;
	{ // SCOPE FOR TERMINATION
		float positions[] = {
			-0.5f, -0.5f, 0.0f, 0.0f,
			0.5f, -0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};
		// basic blending
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		

		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);


		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		// shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		Texture texture("res/textures/Geomtry.png");
		texture.Bind(); // texture.Bind(2) requires SetUniform1i(...,2);
		shader.SetUniform1i("u_Texture", 0);

		// reset and bind
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		Renderer renderer;

		float r = 0.0f;
		float increment = 0.05f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			shader.Bind();
			// shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			renderer.Draw(va, ib, shader);
		
			if (r > 1.0f) {
				increment = -0.05f;
			}
			else if (r < 0.0f) {
				increment = 0.05f;
			}
			r += increment;
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

	} // END SCOPE FOR TERMINATION
	glfwTerminate();

	return 0;
}