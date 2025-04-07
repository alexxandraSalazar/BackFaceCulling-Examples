//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------

#include"Model.h"
#include "GLFW/glfw3.h"


const unsigned int width = 800;
const unsigned int height = 800;

void on_offBackFaceCulling(GLFWwindow* window);


int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "BackFaceCulling", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);


	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);



	

	//// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);


	// Creates camera object
	// Original
	Camera camera(width, height, glm::vec3(-0.50f, 0.05f, -0.10f));
	camera.rotateZ(90);
	camera.rotateY(-90);




	/*
	* I'm doing this relative path thing in order to centralize all the resources into one folder and not
	* duplicate them between tutorial folders. You can just copy paste the resources from the 'Resources'
	* folder and then give a relative path from this folder to whatever resource you want to get to.
	* Also note that this requires C++17, so go to Project Properties, C/C++, Language, and select C++17
	*/
	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	std::string modelPath = "/Resources/modelsBackFace/models/bunny/scene.gltf";
	
	// Load in a model
	Model model((parentDir + modelPath).c_str());


	// Variables to create periodic event for FPS displaying
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	// Keeps track of the amount of frames in timeDiff
	unsigned int counter = 0;
	// Original code from the tutorial
	// Model model("models/bunny/scene.gltf");



	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Updates counter and times
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;

		if (timeDiff >= 1.0 / 30.0)
		{
			// Creates new title
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = "BackFaceCulling -> " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());

			// Resets times and counter
			prevTime = crntTime;
			counter = 0;

			// Use this if you have disabled VSync
			//camera.Inputs(window);
		}

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handles camera inputs
		camera.Inputs(window);

		on_offBackFaceCulling(window);

		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		// Draw a model
		model.Draw(shaderProgram, camera);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}

void on_offBackFaceCulling(GLFWwindow* window)
{
	// variable bandera para si esta activo
	static bool cullingActive = false;
	// bool para evitar que se active constantemente
	static bool bWasPressed = false;

	
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	{
		// Si antes NO estaba presionada, significa que se acaba de pulsar B
		if (!bWasPressed)
		{
			bWasPressed = true;              // Marcamos que ahora B esta presionada
			cullingActive = !cullingActive;  // Invertimos el estado (toggle) si es true pasa a false y si esta false pasa a true

			if (cullingActive)
			{
				// Si acabamos de activarlo
				glEnable(GL_CULL_FACE);
				glFrontFace(GL_CW);
				glCullFace(GL_BACK);
				std::cout << "Back Face Culling ACTIVADO\n";
			}
			else
			{
				// Si acabamos de desactivarlo
				glDisable(GL_CULL_FACE);
				std::cout << "Back Face Culling DESACTIVADO\n";
			}
		}
	}
	else
	{
		// Si la tecla B no esta presionada, reiniciamos la variable
		bWasPressed = false;
	}
}
