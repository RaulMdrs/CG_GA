#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Animacao.h"

#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void createBullet(Shader& shader, Model& obj);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(0.0f, 20.0f, 0.0f);

// Animacao
Animacao* animacao = new Animacao();


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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	Shader ourShader("1.model_loading.vs", "1.model_loading.fs");

	// load models
	// -----------

	//Model ourTeapot("Modelos/teapot1.obj");
	//Model ourCube("Modelos/cube.obj");
	//Model ourBullet("Modelos/cube.obj");
	//Model ourBackpack("Modelos/backpack.obj");
	//Model ourTower("Modelos/torreDiPisa.obj");
	//Model ourTrout("Modelos/trout.obj");
	Model ourPista("Modelos/track.obj");
	Model ourCar("Modelos/car.obj");

	animacao->LerPoints("Modelos/bspline.txt");
	int timeBullet = 0;

	///// Teste de luz abaixo

	//unsigned int VBO;
	//glGenBuffers(1, &VBO);

	//unsigned int lightCubeVAO;
	//glGenVertexArrays(1, &lightCubeVAO);
	//glBindVertexArray(lightCubeVAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	///// Teste de luz acima
	int i = 0;

	glm::mat4 car = glm::mat4(1.0f);
	car = glm::translate(car, glm::vec3(0.0f, 0.0f, 0.0f)); // Commented out translation
	car = glm::scale(car, glm::vec3(0.05f, 0.05f, 0.05f));
	ourShader.setMat4("model", car);


	glm::vec3 target = animacao->GetAnimacaoPoints()[i]; // Target position
	glm::vec3 translation = glm::vec3(car[3][0], car[3][1], car[3][2]);

	float speed = 0.4f; // Adjust this value to control the speed of movement
	float factor = 0.0f; // Initialization of movement factor


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		//processInput(window, ourShader, ourCube);
		processInput(window);

		// render
		// ------
		glClearColor(0.05f, 0.5f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// TESTE LuZZZZZZZZZZZZZZZZZZ
		ourShader.use();
		ourShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		ourShader.setVec3("lightPos", lightPos);
		ourShader.setVec3("viewPos", camera.Position);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// world transformation
		glm::mat4 modeLight = glm::mat4(1.0f);
		ourShader.setMat4("model", modeLight);

		// criar o ponto de luz
		modeLight = glm::mat4(1.0f);
		modeLight = glm::translate(modeLight, lightPos);
		modeLight = glm::scale(modeLight, glm::vec3(0.2f)); // a smaller cube

		//  glBindVertexArray(lightCubeVAO);
		 // glDrawArrays(GL_TRIANGLES, 0, 36);



		  // TESTE LuZZZZZZZZZZZZZZZZZZ ^^^^^^


		  // don't forget to enable shader before setting uniforms
		//  ourShader.use();

		  // view/projection transformations
		//  glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		 // glm::mat4 view = camera.GetViewMatrix();
		  //ourShader.setMat4("projection", projection);
	   //   ourShader.setMat4("view", view);

		  // render the loaded model

		glm::mat4 pista = glm::mat4(1.0f);
		pista = glm::translate(pista, glm::vec3(-0.20f, 0.0f, 0.20f));
		pista = glm::scale(pista, glm::vec3(1.0f, 1.0f, 1.0f));
		ourShader.setMat4("model", pista);
		ourPista.Draw(ourShader);
		
		if (factor < 1.0f) {
			glm::vec3 interpolatedTranslation = glm::mix(translation, target, factor);
			car[3] = glm::vec4(interpolatedTranslation, 1.0f); // Update the translation component of the matrix
		}
		ourShader.setVec3("objectColor", 0.0f, 0.0f, 1.0f);
		ourShader.setMat4("model", car);
		ourCar.Draw(ourShader);

		// Increment the factor and update the target when movement is complete
		factor += speed;
		if (factor >= 1.0f) {
			i++;
			if (i >= animacao->GetAnimacaoPoints().size()) {
				i = 0;
			}
			target = animacao->GetAnimacaoPoints()[i];
			translation = glm::vec3(car[3][0], car[3][1], car[3][2]);
			factor = 0.0f;
		}
		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//ourShader.setMat4("model", model);
		//ourTeapot.Draw(ourShader);

		//glm::mat4 model1 = glm::mat4(1.0f);
		//model1 = glm::translate(model1, glm::vec3(5.0f, 0.0f, 5.0f));
		//model1 = glm::scale(model1, glm::vec3(0.5f, 0.5f, 0.5f));
		//ourShader.setMat4("model", model1);
		//ourCube.Draw(ourShader);

		//glm::mat4 model1Bullet = glm::mat4(1.0f);
		//model1Bullet = glm::translate(model1Bullet, glm::vec3(5.0f, 0.0f, 5.0f));
		//model1Bullet = glm::scale(model1Bullet, glm::vec3(0.5f, 0.5f, 0.5f));
		//ourShader.setMat4("model", model1Bullet);
		//ourBullet.Draw(ourShader);

		//glm::mat4 model2 = glm::mat4(1.0f);
		//model2 = glm::translate(model2, glm::vec3(-5.0f, 0.0f, -5.0f));
		//model2 = glm::scale(model2, glm::vec3(0.5f, 0.5f, 0.5f));
		//ourShader.setMat4("model", model2);
		//ourBackpack.Draw(ourShader);

		//glm::mat4 model3 = glm::mat4(1.0f);
		//model3 = glm::translate(model3, glm::vec3(5.0f, 5.0f, 0.0f));
		//model3 = glm::scale(model3, glm::vec3(0.2f, 0.2f, 0.2f));
		//ourShader.setMat4("model", model3);
		//ourTower.Draw(ourShader);

		//glm::mat4 model4 = glm::mat4(1.0f);
		//model4 = glm::translate(model4, glm::vec3(-5.0f, -5.0f, 0.0f));
		//model4 = glm::scale(model4, glm::vec3(0.2f, 0.2f, 0.2f));
		//ourShader.setMat4("model", model4);
		//ourTrout.Draw(ourShader);


		//glm::mat4 bullet = glm::mat4(1.0f);
		//bullet = glm::translate(bullet, glm::vec3(15.0f, -5.0f, 0.0f)); // setar uma position 
		//bullet = glm::scale(bullet, glm::vec3(0.2f, 0.2f, 0.2f));
		//ourShader.setMat4("model", bullet);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------

		if (glfwGetMouseButton(window, 0) == GLFW_PRESS) {
			timeBullet = 1000;

		}

		if (timeBullet > 0) {
			glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
			//   bullet = 

			timeBullet -= 1 * deltaTime;
			//ourCube.Draw(ourShader);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

void createBullet(Shader& shader, Model& obj) {

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
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

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

