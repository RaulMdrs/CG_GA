#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include "Shader.h"
#include "Camera.h"
#include "Points.h"
#include "CurveControl.h"
#include "CurveToOBJ.h"

#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
float ConvertToCartesian(float value, float min, float max);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 900;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//Points
Points points;

Shader* ourShader;
CurveControl* curveControl = new CurveControl();
CurveToOBJ* curveToOBJ = new CurveToOBJ();

bool teste = false;
GLuint VAO = 0, VAOBSpline = 0, VAOInternal = 0, VAOExternal = 0;
int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed no init GLEW." << std::endl;
		return EXIT_FAILURE;
	}

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	//stbi_set_flip_vertically_on_load(true);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	ourShader = new Shader("1.model_loading.vs", "1.model_loading.fs");

	ourShader->use();

	curveControl->InitializeVAOVBO();



	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		// per-frame time logic
		//processInput(window);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}	
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			float x = 0, y = 0, z = 0;
			x = ConvertToCartesian(lastX, 0, SCR_WIDTH);
			y = ConvertToCartesian(lastY, 0, SCR_HEIGHT) * -1;
			
			float depthValue;
			glReadPixels(lastX, lastY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depthValue);

			// Mapear o valor do buffer de profundidade para o intervalo [-1, 1]
			depthValue = (depthValue * 2.0f) - 1.0f;


			points.AddControlPoint(glm::vec3(x, y, z));
			VAO = curveControl->CreateLines(points);

			if (points.GetControlPoints().size() > 4)
			{
				curveControl->CreateBSPline(points);
				curveControl->CreateInternalPoints(0.2);
				curveControl->CreateExternalPoints(0.2);
			}
				

			std::cout << "Mouse button pressed in " << x << " , " << y << " , " << z << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			curveToOBJ->createBSplinePointsTxt(curveControl);
			curveToOBJ->createObj(curveControl);
		}

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// view/projection transformations
		glm::mat4 ortho = glm::ortho(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader->setMat4("ortho", ortho);
		ourShader->setMat4("view", view);
		
		ourShader->use();
		ourShader->setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));

		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_STRIP, 0, points.GetControlPoints().size());
		glDrawArrays(GL_POINTS, 0, points.GetControlPoints().size());
		glBindVertexArray(0);

		if (points.GetControlPoints().size() > 4) {

			//BSpline
			ourShader->setVec3("color", glm::vec3(0.0f, 1.0f, 0.0f));
			VAOBSpline = curveControl->BsplineToVBO();
			glBindVertexArray(VAOBSpline);
			glDrawArrays(GL_LINE_STRIP, 0, curveControl->GetBSplinePoints().size() / 3);
			glBindVertexArray(0);

			//Internal

			ourShader->setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
			VAOInternal = curveControl->InternalToVBO();
			glBindVertexArray(VAOInternal);
			glDrawArrays(GL_LINE_STRIP, 0, curveControl->GetInternalPoints().size() / 3);
			glBindVertexArray(0);

			//External

			ourShader->setVec3("color", glm::vec3(0.0f, 0.0f, 1.0f));
			VAOExternal = curveControl->ExternalToVBO();
			glBindVertexArray(VAOExternal);
			glDrawArrays(GL_LINE_STRIP, 0, curveControl->GetExternalPoints().size() / 3);
			glBindVertexArray(0);

		}


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		float x = 0, y = 0;
		x = ConvertToCartesian(lastX, 0, SCR_WIDTH);
		y = ConvertToCartesian(lastY, 0 , SCR_HEIGHT) * -1;
	
		points.AddControlPoint(glm::vec3(x, y, 0));
		VAO = curveControl->CreateLines(points);

		if (points.GetControlPoints().size() > 4)
		{
			curveControl->CreateBSPline(points);
		}
			

		std::cout << "Mouse button pressed in " << x << " , " << y << std::endl;
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

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

float ConvertToCartesian(float value, float rangeMin, float rangeMax) {
	// Calcula a diferença entre o valor máximo e mínimo do intervalo
	float range = rangeMax - rangeMin;

	// Calcula a metade do intervalo
	float halfRange = range / 2.0f;

	// Calcula o valor central do intervalo
	float center = (rangeMax + rangeMin) / 2.0f;

	// Aplica a fórmula de conversão
	return ((value - center) / halfRange);
}