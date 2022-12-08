#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Object.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
unsigned int createShader(const char* filename, const char* type);
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader);
unsigned int ModelVAO(Object* model);
void DrawModel(const char* target, glm::mat4 M, glm::mat4 V, glm::mat4 P);

Object* catModel = new Object("obj/cat.obj");
Object* baseModel = new Object("obj/platform.obj");
Object* teapotModel = new Object("obj/teapot.obj");
Object* ballModel = new Object("obj/tennis_ball.obj");
Object* diskModel = new Object("obj/disk.obj");
unsigned int catVAO, baseVAO, ballVAO, teapotVAO, diskVAO;
unsigned int shaderProgram;
int windowWidth = 800, windowHeight = 600;
float speed = 1;

glm::mat4 orbit(float speed) {

	glm::mat4 matrix = glm::mat4(1.0f);
	matrix = glm::rotate(matrix, glm::radians(-0.7f * speed), glm::vec3(0, 1, 0));
	return matrix;
}


int main()
{
	// Initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "HW1", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// VAO, VBO
	catVAO = ModelVAO(catModel);
	baseVAO = ModelVAO(baseModel);
	ballVAO = ModelVAO(ballModel);
	teapotVAO = ModelVAO(teapotModel);
	diskVAO = ModelVAO(diskModel);

	// Shaders
	unsigned int vertexShader, fragmentShader;
	vertexShader = createShader("vertexShader.vert", "vert");
	fragmentShader = createShader("fragmentShader.frag", "frag");
	shaderProgram = createProgram(vertexShader, fragmentShader);
	glUseProgram(shaderProgram);

	// TO DO:
	// Enable depth test, face culling
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Display loop
	// TO DO:
	// Draw the rotating music box
	glViewport(0, 0, windowWidth, windowHeight);
	glm::mat4 base = glm::mat4(1.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0, 12, 12), glm::vec3(0, -1, 0), glm::vec3(0, 1, 0));
	glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
	glm::mat4 tea = glm::translate(base, glm::vec3(-1, 1.5, 0));
	glm::mat4 tea_orbit = glm::rotate(base, glm::radians(-0.2f), glm::vec3(0, 1, 0));
	glm::mat4 disk = glm::translate(base, glm::vec3(0, 1.3, 0));
	glm::mat4 cat = glm::translate(disk, glm::vec3(2.5, 0, 0));
	glm::mat4 ball = glm::translate(cat, glm::vec3(0, 0.5, 2));


	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// TO DO:
		// Create model, view, and perspective matrix

		// base
		base = glm::rotate(base, glm::radians(-0.2f), glm::vec3(0, 1, 0));
		DrawModel("base", glm::scale(base, glm::vec3(1.25, 1, 1.25)), view, perspective);

		// teapot & teapot disk
		tea = glm::rotate(tea, glm::radians(-1.0f), glm::vec3(0, 1, 0));
		tea = tea_orbit * tea;
		DrawModel("disk", glm::scale(tea, glm::vec3(1, 1, 1)), view, perspective);
		DrawModel("teapot", glm::scale(tea, glm::vec3(0.3, 0.3, 0.3)), view, perspective);

		// disk
		disk = orbit(speed) * disk;
		DrawModel("disk", glm::scale(disk, glm::vec3(4, 1, 4)), view, perspective);

		// cat
		cat = orbit(speed) * cat;
		DrawModel("cat", glm::scale(cat, glm::vec3(1, 1, 1)), view, perspective);

		// ball
		ball = glm::rotate(ball, glm::radians(1.2f), glm::vec3(1, 0, 0));
		ball = orbit(speed) * ball;
		DrawModel("ball", glm::scale(ball, glm::vec3(1.2, 1.2, 1.2)), view, perspective);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// TO DO:
// Add key callback
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (key == GLFW_KEY_1) speed *= 2;
	if (key == GLFW_KEY_2) speed /= 2;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}

unsigned int createShader(const char* filename, const char* type)
{
	unsigned int shader;
	if (strcmp(type, "vert") == 0)
		shader = glCreateShader(GL_VERTEX_SHADER);
	else if (strcmp(type, "frag") == 0)
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	else
	{
		cout << "Unknown Shader Type.\n";
		return 0;
	}

	FILE* fp = fopen(filename, "rb");
	fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);  //same as rewind(fp);

	char* source = (char*)malloc(sizeof(char) * (fsize + 1));
	fread(source, fsize, 1, fp);
	fclose(fp);

	source[fsize] = 0;

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::" << type << "::COMPLIATION_FAILED\n" << infoLog << endl;
		return -1;
	}

	return shader;
}

unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
	unsigned int program = glCreateProgram();

	//Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	//Link our program
	glLinkProgram(program);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	int success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		int maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		char* infoLog = (char*)malloc(sizeof(char) * (maxLength));
		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);

		//We don't need the program anymore.
		glDeleteProgram(program);
		//Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		puts(infoLog);
		free(infoLog);

		return -1;
	}

	//Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	return program;
}

unsigned int ModelVAO(Object* model)
{
	unsigned int VAO, VBO[3];
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(3, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model->positions.size()), &(model->positions[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model->normals.size()), &(model->normals[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model->texcoords.size()), &(model->texcoords[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}

void DrawModel(const char* target, glm::mat4 M, glm::mat4 V, glm::mat4 P)
{
	unsigned int mLoc, vLoc, pLoc;
	mLoc = glGetUniformLocation(shaderProgram, "M");
	vLoc = glGetUniformLocation(shaderProgram, "V");
	pLoc = glGetUniformLocation(shaderProgram, "P");
	glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(M));
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(V));
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(P));
	if (strcmp(target, "base") == 0)
	{
		glBindVertexArray(baseVAO);
		glDrawArrays(GL_TRIANGLES, 0, baseModel->positions.size());
	}
	else if (strcmp(target, "cat") == 0)
	{
		glBindVertexArray(catVAO);
		glDrawArrays(GL_TRIANGLES, 0, catModel->positions.size());
	}
	else if (strcmp(target, "ball") == 0)
	{
		glBindVertexArray(ballVAO);
		glDrawArrays(GL_TRIANGLES, 0, ballModel->positions.size());
	}
	else  if (strcmp(target, "teapot") == 0)
	{
		glBindVertexArray(teapotVAO);
		glDrawArrays(GL_TRIANGLES, 0, teapotModel->positions.size());
	}
	else if (strcmp(target, "disk") == 0)
	{
		glBindVertexArray(diskVAO);
		glDrawArrays(GL_TRIANGLES, 0, diskModel->positions.size());
	}
	glBindVertexArray(0);
}
