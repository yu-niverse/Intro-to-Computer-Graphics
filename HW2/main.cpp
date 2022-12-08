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
void LoadTexture(unsigned int& texture, const char* tFileName);
unsigned int ModelVAO(Object* model);
void DrawModel(const char* target, glm::mat4 M, glm::mat4 V, glm::mat4 P);
glm::mat4 getPerspective();
glm::mat4 getView();

Object* catModel = new Object("obj/cat.obj");
Object* boxModel = new Object("obj/CardboardBox1.obj");
unsigned int catVAO, boxVAO;
unsigned int shaderProgram;
bool deformation = false, bigger = false, smaller = false;
int darken_strip = 0, bonus = 0, bonus_trigger = 0;
float scale = 0;
glm::mat4 S = glm::mat4(1.0f);


int windowWidth = 800, windowHeight = 600;

int main()
{
	// Initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFE_OPENGL_FORWARD_COMPACT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "HW2", NULL, NULL);
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

	// Shaders
	unsigned int vertexShader, fragmentShader;
	vertexShader = createShader("vertexShader.vert", "vert");
	fragmentShader = createShader("fragmentShader.frag", "frag");
	shaderProgram = createProgram(vertexShader, fragmentShader);
	glUseProgram(shaderProgram);

	// Texture
	unsigned int catTexture, boxTexture;
	LoadTexture(catTexture, "obj/Cat_diffuse.jpg");
	LoadTexture(boxTexture, "obj/CardboardBox1_Albedo.tga");

	// TO DO:
	// Create VAO, VBO
	catVAO = ModelVAO(catModel);
	boxVAO = ModelVAO(boxModel);


	// Display loop
	// TO DO:
	// Draw the models with shaders
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);

	glm::mat4 view = getView();
	glm::mat4 perspective = getPerspective();
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 box = glm::mat4(1.0f);
	glm::mat4 cat = glm::rotate(box, glm::radians(90.0f), glm::vec3(0, 1, 0));

	float past = glfwGetTime();
	int cnt = 0;




	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float current = glfwGetTime();
		float time_length = current - past;
		float degree = 90.0f * time_length;
		past = current;


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, boxTexture);
		box = glm::rotate(box, glm::radians(degree), glm::vec3(0, 1, 0));
		DrawModel("box", glm::scale(box, glm::vec3(0.0625, 0.05, 0.05)), view, perspective);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, catTexture);
		cat = glm::rotate(cat, glm::radians(degree), glm::vec3(0, 1, 0));

		// deformation effect
		if (deformation) {
			if (scale <= 0.0) {
				bigger = true;
				smaller = false;
			}
			else if (scale >= 0.4) {
				bigger = false;
				smaller = true;
			}

			if (bigger && scale < 0.4) scale += 0.004;
			else if (smaller && scale > 0.0) scale -= 0.004;

			S = glm::scale(model, glm::vec3(1 + scale, 1 + scale, 1 + scale));
		}

		// bonus effect
		if ((bonus_trigger == 1) && (cnt % 10 == 0))
			bonus = (bonus + 1) % 8;

		DrawModel("cat", cat, view, perspective);

		cnt += 1;
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	// apply effect 1
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		deformation = true;
		cout << "Key 1 Pressed: Apply Deformation\n";
	}
	// stop effect 1
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		deformation = false;
		cout << "Key 2 Pressed: Stop Deformation\n";
	}
	// apply effect 2
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		darken_strip = 1;
		cout << "Key 3 Pressed: Apply Darken\n";
	}
	// stop effect 2
	else if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
		darken_strip = 0;
		cout << "Key 4 Pressed: Stop Darken\n";
	}
	// apply bonus
	else if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
		bonus_trigger = 1;
		cout << "Key 5 Pressed: Apply Bonus\n";
	}
	// stop bonus
	else if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
		bonus_trigger = 0;
		bonus = 0;
		cout << "Key 6 Pressed: Stop Bonus\n";
	}
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

void LoadTexture(unsigned int& texture, const char* tFileName) {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(tFileName, &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

glm::mat4 getPerspective()
{
	return glm::perspective(
		glm::radians(45.0f),
		(float)windowWidth / (float)windowHeight, 
		0.1f, 100.0f);
}

glm::mat4 getView()
{
	return glm::lookAt(
		glm::vec3(0, 5, 5),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));
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
	unsigned int mLoc, vLoc, pLoc, sLoc;
	mLoc = glGetUniformLocation(shaderProgram, "M");
	vLoc = glGetUniformLocation(shaderProgram, "V");
	pLoc = glGetUniformLocation(shaderProgram, "P");
	sLoc = glGetUniformLocation(shaderProgram, "S");
	glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(M));
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(V));
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(P));

	if (strcmp(target, "box") == 0)
	{
		glUniform1i(glGetUniformLocation(shaderProgram, "darken"), 0);
		glUniform1i(glGetUniformLocation(shaderProgram, "bonus"), 0);
		glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		glBindVertexArray(boxVAO);
		glDrawArrays(GL_TRIANGLES, 0, boxModel->positions.size());
	}
	else if (strcmp(target, "cat") == 0)
	{
		glUniform1i(glGetUniformLocation(shaderProgram, "darken"), darken_strip);
		glUniform1i(glGetUniformLocation(shaderProgram, "bonus"), bonus);
		glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(S));
		glBindVertexArray(catVAO);
		glDrawArrays(GL_TRIANGLES, 0, catModel->positions.size());
	}
	glBindVertexArray(0);
}
