#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <cmath>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/gtc/constants.hpp> // glm::pi

#include "Shader.h"
#include "Camera.h"
#include "Sphere.h"
#include "Model.h"
#include "Particle_generator.h"

// Function prototypes
GLFWwindow* envInit();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
GLuint screenWidth = 800, screenHeight = 600;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat morphRate = 0.0f;

// The MAIN function, from here we start the application and run the game loop
int main()
{
	
	GLFWwindow* window = envInit();

	
	Sphere sphere(100, 100);

	Shader dstShader("vs_new.txt", "fragment_new.txt");
	Shader srcShader("src_vs.txt", "src_frag.txt");

	Model dstModel("toad/toad.obj");
	Model srcModel("f16/f16.obj");

	GLuint count = 0;
	/*for (GLuint i = 0; i < dstModel.meshes.size(); i++){
		for (GLuint j = 0; j < dstModel.meshes[i].vertices.size(); j++){
			//std::cout << dstModel.meshes[i].vertices[j].Position.x << " " << dstModel.meshes[i].vertices[j].Position.y << " " << dstModel.meshes[i].vertices[j].Position.z << endl;
			//std::cout << sphere.vertices[sphere.indices[count % sphere.indices.size()]].x << " " << sphere.vertices[sphere.indices[count % sphere.indices.size()]].y << " " << sphere.vertices[sphere.indices[count % sphere.indices.size()]].z << endl;
			dstModel.meshes[i].vertices[j].OriginPos.x = sphere.vertices[sphere.indices[count % sphere.indices.size()]].x;
			dstModel.meshes[i].vertices[j].OriginPos.y = sphere.vertices[sphere.indices[count % sphere.indices.size()]].y;
			dstModel.meshes[i].vertices[j].OriginPos.z = sphere.vertices[sphere.indices[count % sphere.indices.size()]].z;
			//dstModel.meshes[i].vertices[j].OriginPos = sphere.vertices[sphere.indices[count % sphere.indices.size()]];
			dstModel.meshes[i].vertices[j].OriginColor.x = 1.0f;
			dstModel.meshes[i].vertices[j].OriginColor.y = 0.5f;
			dstModel.meshes[i].vertices[j].OriginColor.z = 0.2f;
			//dstModel.meshes[i].vertices[j].OriginColor = glm::vec3(1.0f, 0.5f, 0.2f);
			count++;
		}
	}*/

	std::vector<Vertex> srcVertices;
	for (GLuint i = 0; i < srcModel.meshes.size(); i++){
		for (GLuint j = 0; j < srcModel.meshes[i].indices.size(); j++){
			Vertex tempver = srcModel.meshes[i].vertices[srcModel.meshes[i].indices[j]];
			srcVertices.push_back(tempver);
		}
	}

	std::vector<Vertex> dstVertices;
	for (GLuint i = 0; i < dstModel.meshes.size(); i++){
		for (GLuint j = 0; j < dstModel.meshes[i].indices.size(); j++){
			Vertex tempver = dstModel.meshes[i].vertices[dstModel.meshes[i].indices[j]];
			dstVertices.push_back(tempver);
		}
	}
	bool flag;
	GLuint num = 0;
	for (GLuint i = 0; i < dstModel.meshes.size(); i++)
		num += dstModel.meshes[i].vertices.size();
	if (srcVertices.size() < num)
	{
		morphRate = 0.0f;
		flag = true;
		for (GLuint i = 0; i < dstModel.meshes.size(); i++){
			for (GLuint j = 0; j < dstModel.meshes[i].vertices.size(); j++){
				//std::cout << dstModel.meshes[i].vertices[j].Position.x << " " << dstModel.meshes[i].vertices[j].Position.y << " " << dstModel.meshes[i].vertices[j].Position.z << endl;
				//std::cout << sphere.vertices[sphere.indices[count % sphere.indices.size()]].x << " " << sphere.vertices[sphere.indices[count % sphere.indices.size()]].y << " " << sphere.vertices[sphere.indices[count % sphere.indices.size()]].z << endl;
				dstModel.meshes[i].vertices[j].OriginPos.x = srcVertices[count % srcVertices.size()].Position.x;
				dstModel.meshes[i].vertices[j].OriginPos.y = srcVertices[count % srcVertices.size()].Position.y;
				dstModel.meshes[i].vertices[j].OriginPos.z = srcVertices[count % srcVertices.size()].Position.z;
				//dstModel.meshes[i].vertices[j].OriginPos = sphere.vertices[sphere.indices[count % sphere.indices.size()]];
				dstModel.meshes[i].vertices[j].OriginTex.x = srcVertices[count % srcVertices.size()].TexCoords.x;
				dstModel.meshes[i].vertices[j].OriginTex.y = srcVertices[count % srcVertices.size()].TexCoords.y;
				//dstModel.meshes[i].vertices[j].OriginColor.z = 0.2f;
				//dstModel.meshes[i].vertices[j].OriginColor = glm::vec3(1.0f, 0.5f, 0.2f);
				count++;
			}
		}

		for (GLuint i = 0; i < dstModel.meshes.size(); i++){
			dstModel.meshes[i].setupMesh();
		}
	}
	else{
		morphRate = 1.0f;
		flag = false;
		for (GLuint i = 0; i < srcModel.meshes.size(); i++){
			for (GLuint j = 0; j < srcModel.meshes[i].vertices.size(); j++){
				//std::cout << dstModel.meshes[i].vertices[j].Position.x << " " << dstModel.meshes[i].vertices[j].Position.y << " " << dstModel.meshes[i].vertices[j].Position.z << endl;
				//std::cout << sphere.vertices[sphere.indices[count % sphere.indices.size()]].x << " " << sphere.vertices[sphere.indices[count % sphere.indices.size()]].y << " " << sphere.vertices[sphere.indices[count % sphere.indices.size()]].z << endl;
				srcModel.meshes[i].vertices[j].OriginPos.x = dstVertices[count % dstVertices.size()].Position.x;
				srcModel.meshes[i].vertices[j].OriginPos.y = dstVertices[count % dstVertices.size()].Position.y;
				srcModel.meshes[i].vertices[j].OriginPos.z = dstVertices[count % dstVertices.size()].Position.z;
				//dstModel.meshes[i].vertices[j].OriginPos = sphere.vertices[sphere.indices[count % sphere.indices.size()]];
				srcModel.meshes[i].vertices[j].OriginTex.x = dstVertices[count % dstVertices.size()].TexCoords.x;
				srcModel.meshes[i].vertices[j].OriginTex.y = dstVertices[count % dstVertices.size()].TexCoords.y;
				//srcModel.meshes[i].vertices[j].OriginColor.z = 0.2f;
				//dstModel.meshes[i].vertices[j].OriginColor = glm::vec3(1.0f, 0.5f, 0.2f);
				count++;
			}
		}

		for (GLuint i = 0; i < srcModel.meshes.size(); i++){
			srcModel.meshes[i].setupMesh();
		}
	}
	/*for (GLuint i = 0; i < dstModel.meshes.size(); i++){
		dstModel.meshes[i].setupMesh();
	}*/

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	
	//glm::vec3 sphere_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	// Game loop

	Shader parShader("par_vs.txt", "par_frag.txt");
	ParticleGenerator particleGenerator = ParticleGenerator(parShader, dstModel);

	GLfloat x = 0;
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		//GLfloat x = glm::sin(glfwGetTime()/4)*sin(glfwGetTime()/4);
		//GLfloat x = 0;
		if (x <= 1.0f)
			x += 0.0001f;
		GLfloat t;
		if (flag)
			t = x;
		else
			t = 1 - x;
		morphRate = t*t*(3 - 2 * t);
		//morphRate = x*x*(3-2*x);
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		Do_Movement();

		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		dstShader.Use();   // <-- Don't forget this one!
		// Transformation matrices
		glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(dstShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(dstShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Draw the loaded model
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(glGetUniformLocation(dstShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glUniform1f(glGetUniformLocation(dstShader.Program, "morphRate"), morphRate);
		if (flag){
			glActiveTexture(GL_TEXTURE0 + dstModel.meshes.size() + 1);
			glUniform1i(glGetUniformLocation(dstShader.Program, "newTexture"), dstModel.meshes.size() + 1);
			glBindTexture(GL_TEXTURE_2D, srcModel.meshes[0].textures[0].id);
			dstModel.Draw(dstShader);
		}
		else{
			glActiveTexture(GL_TEXTURE0 + srcModel.meshes.size() + 1);
			glUniform1i(glGetUniformLocation(dstShader.Program, "newTexture"), srcModel.meshes.size() + 1);
			glBindTexture(GL_TEXTURE_2D, dstModel.meshes[0].textures[0].id);
			srcModel.Draw(dstShader);
		}
		if (x >= 0.98f){
			parShader.Use();
			glUniformMatrix4fv(glGetUniformLocation(parShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(glGetUniformLocation(parShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(parShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			particleGenerator.Draw(projection, view, model);
			particleGenerator.Update(deltaTime, dstModel, 500);
		}

		// Swap the buffers
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}

GLFWwindow* envInit()
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);

	return window;
}

void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_Q])
		morphRate = 1.0f;
	if (keys[GLFW_KEY_E])
		morphRate = 0.0f;
	if (keys[GLFW_KEY_T])
		glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
	if (keys[GLFW_KEY_R])
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}