#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "SOIL2\SOIL2.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

const GLint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
//void ScrollCallback(GLFWwindow * window, double xOffset, double yOffset);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = WIDTH / 2.0f;
bool keys[1024];
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, -2.0f);


int multiplas_luzes() {

	glfwInit();

	//Inicializando GLFW com as devidas propriedades
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//Define que janela nao pode ter tamanho alterado
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Inicializa janela
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Todas as luzes com textura", nullptr, nullptr);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	//Verifica se tudo ocorreu bem na criacao da janela
	if (nullptr == window) {
		std::cout << "Falha ao criar janela GLFW" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	//glfwSetScrollCallback(window, ScrollCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	glewExperimental = GL_TRUE;

	//inicializar GLWE

	if (GLEW_OK != glewInit()) {
		std::cout << "Falha ao inicializar GLEW" << std::endl;

		return EXIT_FAILURE;
	}

	//Define as dimensoes da viewport
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Shader ourShader("core.vs", "core.frag");
	Shader lightingShader("lighting.vs", "lighting.frag");
	Shader lampShader("lamp.vs", "lamp.frag");

	// Vertices para Projecao Perspectiva de um cubo

	GLfloat vertices[] = {
		//coordenadas x,y,z		//normal dos vertices	//coordenadas de textura
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	// Posicoes de todos os 10 cubos na cena
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,   0.0f,   0.0f),
		glm::vec3(2.0f,   5.0f,   -15.0f),
		glm::vec3(-1.5f,  -2.2f,  -2.5f),
		glm::vec3(-3.8f,  -2.0f,  -12.3f),
		glm::vec3(2.4f,   -0.4f,  -3.5f),
		glm::vec3(-1.7f,  3.0f,   -7.5f),
		glm::vec3(1.3f,   -2.0f,  -2.5f),
		glm::vec3(1.5f,   2.0f,   -2.5f),
		glm::vec3(1.5f,   0.2f,   -1.5f),
		glm::vec3(-1.3f,  1.0f,   -1.5f)
	};

	//posicoes das luzes pontuais
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)
	};

	GLuint VBO, boxVAO;
	glGenVertexArrays(1, &boxVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Bind o vextex array object primeiro e depois bind e set ponteiros de atributo
	glBindVertexArray(boxVAO);

	//Atributos de posicao para cubo sem textura
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Atributos da noormal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Atributos das coordenadas
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); //Unbind VAO

	
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//Atributos de posicao para cubo
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0); //Unbind VAO


	//Carregando texturas
	GLuint diffuseMap, specularMap, emissionMap;
	glGenTextures(1, &diffuseMap);
	glGenTextures(1, &specularMap);
	glGenTextures(1, &emissionMap);

	int textureWidth, textureHeight;
	unsigned char *image;

	//Diffuse map
	image = SOIL_load_image("container2.png", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	//specular map
	image = SOIL_load_image("container2_specular.png", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 100);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1000);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	while (!glfwWindowShouldClose(window)) {

		//Mover a fonte de luz para longe
		//lightPos.x -= 0.001f;
		//lightPos.z -= 0.001f;

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Observa se qualquer evento foi ativado(ex: teclado, mouse)
		glfwPollEvents();
		DoMovement();

		//cor alterada no colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		//luz direcional
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

		//luz pontual 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);

		//luz pontual 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		//luz pontual 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		//luz pontual 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);


		//spot light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));


		//view
		glm::mat4 view;
		view = camera.GetViewMatrix();

		//Alterado para sem textura
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		glm::mat4 model;
		glBindVertexArray(boxVAO);

		//Desenhando os 10 triangulos
		for (GLuint i = 0; i < 10; i++) {
			model = glm::mat4();
			model = glm::translate(model, cubePositions[i]);
			
			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		glBindVertexArray(0);

		
		lampShader.Use();
	
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glBindVertexArray(lightVAO);
		for (GLuint i = 0; i < 4; i++) {
			model = glm::mat4();
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		//abrindo a janela
		glfwSwapBuffers(window);
	}

	//Desalocando os recursos
	glDeleteVertexArrays(1, &boxVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	//Encerrando o glfw
	glfwTerminate();

	return EXIT_SUCCESS;
}

int luz_direcional() {
	glfwInit();

	//Inicializando GLFW com as devidas propriedades
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//Define que janela nao pode ter tamanho alterado
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Inicializa janela
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Somente Luz Direcional com Textura", nullptr, nullptr);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	//Verifica se tudo ocorreu bem na criacao da janela
	if (nullptr == window) {
		std::cout << "Falha ao criar janela GLFW" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewExperimental = GL_TRUE;

	//inicializar GLWE

	if (GLEW_OK != glewInit()) {
		std::cout << "Falha ao inicializar GLEW" << std::endl;

		return EXIT_FAILURE;
	}

	//Define as dimensoes da viewport
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("lightingDirecional.vs", "lightingDirecional.frag");

	// Vertices para Projecao Perspectiva de um cubo

	GLfloat vertices[] = {
		//coordenadas x,y,z		//normal dos vertices	//coordenadas de textura
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	// Posicoes de todos os 10 cubos na cena
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,   0.0f,   0.0f),
		glm::vec3(2.0f,   5.0f,   -15.0f),
		glm::vec3(-1.5f,  -2.2f,  -2.5f),
		glm::vec3(-3.8f,  -2.0f,  -12.3f),
		glm::vec3(2.4f,   -0.4f,  -3.5f),
		glm::vec3(-1.7f,  3.0f,   -7.5f),
		glm::vec3(1.3f,   -2.0f,  -2.5f),
		glm::vec3(1.5f,   2.0f,   -2.5f),
		glm::vec3(1.5f,   0.2f,   -1.5f),
		glm::vec3(-1.3f,  1.0f,   -1.5f)
	};

	GLuint VBO, boxVAO;
	glGenVertexArrays(1, &boxVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Bind o vextex array object primeiro e depois bind e set ponteiros de atributo
	glBindVertexArray(boxVAO);

	//Atributos de posicao para cubo sem textura
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Atributos da noormal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Atributos das coordenadas
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); //Unbind VAO

	//Carregando texturas
	GLuint diffuseMap, specularMap;
	glGenTextures(1, &diffuseMap);
	glGenTextures(1, &specularMap);

	int textureWidth, textureHeight;
	unsigned char *image;

	//Diffuse map
	image = SOIL_load_image("ufms.jpg", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	//specular map
	image = SOIL_load_image("container2_specular.png", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	while (!glfwWindowShouldClose(window)) {

		//Mover a fonte de luz para longe
		//lightPos.x -= 0.001f;
		//lightPos.z -= 0.001f;

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Observa se qualquer evento foi ativado(ex: teclado, mouse)
		glfwPollEvents();
		DoMovement();

		//cor alterada no colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.Use();
		GLint lightDirLoc = glGetUniformLocation(lightingShader.Program, "light.direction");
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(lightDirLoc, -0.2f, -1.0f, -0.3f);
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);

		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		//view
		glm::mat4 view;
		view = camera.GetViewMatrix();

		//Alterado para sem textura
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		glm::mat4 model;
		glBindVertexArray(boxVAO);

		//Desenhando os 10 triangulos
		for (GLuint i = 0; i < 10; i++) {
			model = glm::mat4();
			model = glm::translate(model, cubePositions[i]);

			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);

		//abrindo a janela
		glfwSwapBuffers(window);
	}

	//Desalocando os recursos
	glDeleteVertexArrays(1, &boxVAO);
	//glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	//Encerrando o glfw
	glfwTerminate();

	return EXIT_SUCCESS;
}

int luz_pontual() {

	glfwInit();

	//Inicializando GLFW com as devidas propriedades
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//Define que janela nao pode ter tamanho alterado
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Inicializa janela
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Somente luz pontual com textura", nullptr, nullptr);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	//Verifica se tudo ocorreu bem na criacao da janela
	if (nullptr == window) {
		std::cout << "Falha ao criar janela GLFW" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	//glfwSetScrollCallback(window, ScrollCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	glewExperimental = GL_TRUE;

	//inicializar GLWE

	if (GLEW_OK != glewInit()) {
		std::cout << "Falha ao inicializar GLEW" << std::endl;

		return EXIT_FAILURE;
	}

	//Define as dimensoes da viewport
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Shader ourShader("core.vs", "core.frag");
	Shader lightingShader("lightingpontual.vs", "lightingpontual.frag");
	Shader lampShader("lamppontual.vs", "lamppontual.frag");

	// Vertices para Projecao Perspectiva de um cubo

	GLfloat vertices[] = {
		//coordenadas x,y,z		//normal dos vertices	//coordenadas de textura
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	// Posicoes de todos os 10 cubos na cena
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,   0.0f,   0.0f),
		glm::vec3(2.0f,   5.0f,   -15.0f),
		glm::vec3(-1.5f,  -2.2f,  -2.5f),
		glm::vec3(-3.8f,  -2.0f,  -12.3f),
		glm::vec3(2.4f,   -0.4f,  -3.5f),
		glm::vec3(-1.7f,  3.0f,   -7.5f),
		glm::vec3(1.3f,   -2.0f,  -2.5f),
		glm::vec3(1.5f,   2.0f,   -2.5f),
		glm::vec3(1.5f,   0.2f,   -1.5f),
		glm::vec3(-1.3f,  1.0f,   -1.5f)
	};

	GLuint VBO, boxVAO;
	glGenVertexArrays(1, &boxVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Bind o vextex array object primeiro e depois bind e set ponteiros de atributo
	glBindVertexArray(boxVAO);

	//Atributos de posicao para cubo sem textura
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Atributos da noormal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Atributos das coordenadas
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); //Unbind VAO


	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Atributos de posicao para cubo
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0); //Unbind VAO


						  //Carregando texturas
	GLuint diffuseMap, specularMap, emissionMap;
	glGenTextures(1, &diffuseMap);
	glGenTextures(1, &specularMap);
	glGenTextures(1, &emissionMap);

	int textureWidth, textureHeight;
	unsigned char *image;

	//Diffuse map
	image = SOIL_load_image("facom.jpg", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	//specular map
	image = SOIL_load_image("container2_specular.png", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	while (!glfwWindowShouldClose(window)) {

		//Mover a fonte de luz para longe
		//lightPos.x -= 0.001f;
		//lightPos.z -= 0.001f;

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Observa se qualquer evento foi ativado(ex: teclado, mouse)
		glfwPollEvents();
		DoMovement();

		//cor alterada no colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.Use();
		GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
		//GLint lightDirLoc = glGetUniformLocation(lightingShader.Program, "light.direction");
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		//glUniform3f(lightDirLoc, -0.2f, -1.0f, -0.3f);
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "light.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "light.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "light.quadratic"), 0.032f);

		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		//view
		glm::mat4 view;
		view = camera.GetViewMatrix();

		//Alterado para sem textura
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		glm::mat4 model;
		glBindVertexArray(boxVAO);

		//Desenhando os 10 triangulos
		for (GLuint i = 0; i < 10; i++) {
			model = glm::mat4();
			model = glm::translate(model, cubePositions[i]);

			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);

		lampShader.Use();

		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		//abrindo a janela
		glfwSwapBuffers(window);
	}

	//Desalocando os recursos
	glDeleteVertexArrays(1, &boxVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	//Encerrando o glfw
	glfwTerminate();

	return EXIT_SUCCESS;
}

int luz_spot(){

	glfwInit();

	//Inicializando GLFW com as devidas propriedades
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//Define que janela nao pode ter tamanho alterado
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Inicializa janela
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Somente luz spot com textura", nullptr, nullptr);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	//Verifica se tudo ocorreu bem na criacao da janela
	if (nullptr == window) {
		std::cout << "Falha ao criar janela GLFW" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	//glfwSetScrollCallback(window, ScrollCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	glewExperimental = GL_TRUE;

	//inicializar GLWE

	if (GLEW_OK != glewInit()) {
		std::cout << "Falha ao inicializar GLEW" << std::endl;

		return EXIT_FAILURE;
	}

	//Define as dimensoes da viewport
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	Shader lightingShader("lightingspot.vs", "lightingspot.frag");

	// Vertices para Projecao Perspectiva de um cubo

	GLfloat vertices[] = {
		//coordenadas x,y,z		//normal dos vertices	//coordenadas de textura
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	// Posicoes de todos os 10 cubos na cena
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,   0.0f,   0.0f),
		glm::vec3(2.0f,   5.0f,   -15.0f),
		glm::vec3(-1.5f,  -2.2f,  -2.5f),
		glm::vec3(-3.8f,  -2.0f,  -12.3f),
		glm::vec3(2.4f,   -0.4f,  -3.5f),
		glm::vec3(-1.7f,  3.0f,   -7.5f),
		glm::vec3(1.3f,   -2.0f,  -2.5f),
		glm::vec3(1.5f,   2.0f,   -2.5f),
		glm::vec3(1.5f,   0.2f,   -1.5f),
		glm::vec3(-1.3f,  1.0f,   -1.5f)
	};

	GLuint VBO, boxVAO;
	glGenVertexArrays(1, &boxVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Bind o vextex array object primeiro e depois bind e set ponteiros de atributo
	glBindVertexArray(boxVAO);

	//Atributos de posicao para cubo sem textura
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Atributos da noormal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Atributos das coordenadas
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); //Unbind VAO

	//Carregando texturas
	GLuint diffuseMap, specularMap, emissionMap;
	glGenTextures(1, &diffuseMap);
	glGenTextures(1, &specularMap);
	glGenTextures(1, &emissionMap);

	int textureWidth, textureHeight;
	unsigned char *image;

	//Diffuse map
	image = SOIL_load_image("container2.png", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	//specular map
	image = SOIL_load_image("container2_specular.png", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	while (!glfwWindowShouldClose(window)) {

		//Mover a fonte de luz para longe
		//lightPos.x -= 0.001f;
		//lightPos.z -= 0.001f;

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Observa se qualquer evento foi ativado(ex: teclado, mouse)
		glfwPollEvents();
		DoMovement();

		//cor alterada no colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.Use();
		GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
		GLint lightSpotDirLoc = glGetUniformLocation(lightingShader.Program, "light.direction");
		GLint lightSpotCutOffLoc = glGetUniformLocation(lightingShader.Program, "light.cutOff");
		GLint lightSpotOuterCutOffLoc = glGetUniformLocation(lightingShader.Program, "light.outerCutOff");
		//GLint lightDirLoc = glGetUniformLocation(lightingShader.Program, "light.direction");
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		//Preciso para luz pontual
		//glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(lightPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(lightSpotDirLoc, camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform1f(lightSpotCutOffLoc, glm::cos(glm::radians(12.5f)));
		glUniform1f(lightSpotOuterCutOffLoc, glm::cos(glm::radians(17.5f)));
		//glUniform3f(lightDirLoc, -0.2f, -1.0f, -0.3f);
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "light.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "light.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "light.quadratic"), 0.032f);

		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		//view
		glm::mat4 view;
		view = camera.GetViewMatrix();

		//Alterado para sem textura
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		glm::mat4 model;
		glBindVertexArray(boxVAO);

		//Desenhando os 10 triangulos
		for (GLuint i = 0; i < 10; i++) {
			model = glm::mat4();
			model = glm::translate(model, cubePositions[i]);

			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);

		//abrindo a janela
		glfwSwapBuffers(window);
	}

	//Desalocando os recursos
	glDeleteVertexArrays(1, &boxVAO);
	//glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	//Encerrando o glfw
	glfwTerminate();

	return EXIT_SUCCESS;
}

int multiplas_luzes_sem_textura() {

	glfwInit();

	//Inicializando GLFW com as devidas propriedades
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//Define que janela nao pode ter tamanho alterado
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Inicializa janela
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Todas as luzes sem textura", nullptr, nullptr);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	//Verifica se tudo ocorreu bem na criacao da janela
	if (nullptr == window) {
		std::cout << "Falha ao criar janela GLFW" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	//glfwSetScrollCallback(window, ScrollCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	glewExperimental = GL_TRUE;

	//inicializar GLWE

	if (GLEW_OK != glewInit()) {
		std::cout << "Falha ao inicializar GLEW" << std::endl;

		return EXIT_FAILURE;
	}

	//Define as dimensoes da viewport
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Shader ourShader("core.vs", "core.frag");
	Shader lightingShader("lightingsemtex.vs", "lightingsemtex.frag");
	Shader lampShader("lamp.vs", "lamp.frag");

	// Vertices para Projecao Perspectiva de um cubo

	GLfloat vertices[] = {
		//coordenadas x,y,z		//normal dos vertices
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,
	};

	// Posicoes de todos os 10 cubos na cena
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,   0.0f,   0.0f),
		glm::vec3(2.0f,   5.0f,   -15.0f),
		glm::vec3(-1.5f,  -2.2f,  -2.5f),
		glm::vec3(-3.8f,  -2.0f,  -12.3f),
		glm::vec3(2.4f,   -0.4f,  -3.5f),
		glm::vec3(-1.7f,  3.0f,   -7.5f),
		glm::vec3(1.3f,   -2.0f,  -2.5f),
		glm::vec3(1.5f,   2.0f,   -2.5f),
		glm::vec3(1.5f,   0.2f,   -1.5f),
		glm::vec3(-1.3f,  1.0f,   -1.5f)
	};

	//posicoes das luzes pontuais
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)
	};

	GLuint VBO, boxVAO;
	glGenVertexArrays(1, &boxVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Bind o vextex array object primeiro e depois bind e set ponteiros de atributo
	glBindVertexArray(boxVAO);

	//Atributos de posicao para cubo sem textura
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Atributos da noormal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); //Unbind VAO


	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);


	//Atributos de posicao para cubo
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0); //Unbind VAO


	lightingShader.Use();
	GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
	GLint lightColorLoc = glGetUniformLocation(lightingShader.Program, "lightColor");
	glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
	glUniform3f(lightColorLoc, 1.0f, 0.5f, 1.0f);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	while (!glfwWindowShouldClose(window)) {

		//Mover a fonte de luz para longe
		//lightPos.x -= 0.001f;
		//lightPos.z -= 0.001f;

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Observa se qualquer evento foi ativado(ex: teclado, mouse)
		glfwPollEvents();
		DoMovement();

		//cor alterada no colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		//luz direcional
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

		//luz pontual 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);

		//luz pontual 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		//luz pontual 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		//luz pontual 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);


		//spot light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));


		//view
		glm::mat4 view;
		view = camera.GetViewMatrix();

		//Alterado para sem textura
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model;
		glBindVertexArray(boxVAO);

		//Desenhando os 10 triangulos
		for (GLuint i = 0; i < 10; i++) {
			model = glm::mat4();
			model = glm::translate(model, cubePositions[i]);

			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);


		lampShader.Use();

		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glBindVertexArray(lightVAO);
		for (GLuint i = 0; i < 4; i++) {
			model = glm::mat4();
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		//abrindo a janela
		glfwSwapBuffers(window);
	}

	//Desalocando os recursos
	glDeleteVertexArrays(1, &boxVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	//Encerrando o glfw
	glfwTerminate();

	return EXIT_SUCCESS;
}

int luz_direcional_sem_textura() {
	glfwInit();

	//Inicializando GLFW com as devidas propriedades
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//Define que janela nao pode ter tamanho alterado
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Inicializa janela
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Somente Luz Direcional sem Textura", nullptr, nullptr);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	//Verifica se tudo ocorreu bem na criacao da janela
	if (nullptr == window) {
		std::cout << "Falha ao criar janela GLFW" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewExperimental = GL_TRUE;

	//inicializar GLWE

	if (GLEW_OK != glewInit()) {
		std::cout << "Falha ao inicializar GLEW" << std::endl;

		return EXIT_FAILURE;
	}

	//Define as dimensoes da viewport
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("lightingDirecionalsemtex.vs", "lightingDirecionalsemtex.frag");

	// Vertices para Projecao Perspectiva de um cubo

	GLfloat vertices[] = {
		//coordenadas x,y,z		//normal dos vertices
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,
	};

	// Posicoes de todos os 10 cubos na cena
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,   0.0f,   0.0f),
		glm::vec3(2.0f,   5.0f,   -15.0f),
		glm::vec3(-1.5f,  -2.2f,  -2.5f),
		glm::vec3(-3.8f,  -2.0f,  -12.3f),
		glm::vec3(2.4f,   -0.4f,  -3.5f),
		glm::vec3(-1.7f,  3.0f,   -7.5f),
		glm::vec3(1.3f,   -2.0f,  -2.5f),
		glm::vec3(1.5f,   2.0f,   -2.5f),
		glm::vec3(1.5f,   0.2f,   -1.5f),
		glm::vec3(-1.3f,  1.0f,   -1.5f)
	};

	GLuint VBO, boxVAO;
	glGenVertexArrays(1, &boxVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Bind o vextex array object primeiro e depois bind e set ponteiros de atributo
	glBindVertexArray(boxVAO);

	//Atributos de posicao para cubo sem textura
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Atributos da noormal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); //Unbind VAO

	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	while (!glfwWindowShouldClose(window)) {

		//Mover a fonte de luz para longe
		//lightPos.x -= 0.001f;
		//lightPos.z -= 0.001f;

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Observa se qualquer evento foi ativado(ex: teclado, mouse)
		glfwPollEvents();
		DoMovement();

		//cor alterada no colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.Use();
		GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(lightingShader.Program, "lightColor");
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(lightColorLoc, 1.0f, 0.5f, 1.0f);
		GLint lightDirLoc = glGetUniformLocation(lightingShader.Program, "light.direction");
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(lightDirLoc, -0.2f, -1.0f, -0.3f);
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);

		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		//view
		glm::mat4 view;
		view = camera.GetViewMatrix();

		//Alterado para sem textura
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model;
		glBindVertexArray(boxVAO);

		//Desenhando os 10 triangulos
		for (GLuint i = 0; i < 10; i++) {
			model = glm::mat4();
			model = glm::translate(model, cubePositions[i]);

			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);

		//abrindo a janela
		glfwSwapBuffers(window);
	}

	//Desalocando os recursos
	glDeleteVertexArrays(1, &boxVAO);
	//glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	//Encerrando o glfw
	glfwTerminate();

	return EXIT_SUCCESS;
}

int luz_pontual_sem_textura() {

	glfwInit();

	//Inicializando GLFW com as devidas propriedades
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//Define que janela nao pode ter tamanho alterado
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Inicializa janela
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Somente luz pontual sem textura", nullptr, nullptr);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	//Verifica se tudo ocorreu bem na criacao da janela
	if (nullptr == window) {
		std::cout << "Falha ao criar janela GLFW" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	//glfwSetScrollCallback(window, ScrollCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	glewExperimental = GL_TRUE;

	//inicializar GLWE

	if (GLEW_OK != glewInit()) {
		std::cout << "Falha ao inicializar GLEW" << std::endl;

		return EXIT_FAILURE;
	}

	//Define as dimensoes da viewport
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Shader ourShader("core.vs", "core.frag");
	Shader lightingShader("lightingpontualsemtex.vs", "lightingpontualsemtex.frag");
	Shader lampShader("lamppontual.vs", "lamppontual.frag");

	// Vertices para Projecao Perspectiva de um cubo

	GLfloat vertices[] = {
		//coordenadas x,y,z		//normal dos vertices
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,
	};

	// Posicoes de todos os 10 cubos na cena
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,   0.0f,   0.0f),
		glm::vec3(2.0f,   5.0f,   -15.0f),
		glm::vec3(-1.5f,  -2.2f,  -2.5f),
		glm::vec3(-3.8f,  -2.0f,  -12.3f),
		glm::vec3(2.4f,   -0.4f,  -3.5f),
		glm::vec3(-1.7f,  3.0f,   -7.5f),
		glm::vec3(1.3f,   -2.0f,  -2.5f),
		glm::vec3(1.5f,   2.0f,   -2.5f),
		glm::vec3(1.5f,   0.2f,   -1.5f),
		glm::vec3(-1.3f,  1.0f,   -1.5f)
	};

	GLuint VBO, boxVAO;
	glGenVertexArrays(1, &boxVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Bind o vextex array object primeiro e depois bind e set ponteiros de atributo
	glBindVertexArray(boxVAO);

	//Atributos de posicao para cubo sem textura
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Atributos da noormal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); //Unbind VAO


	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Atributos de posicao para cubo
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0); //Unbind VAO


	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	while (!glfwWindowShouldClose(window)) {

		//Mover a fonte de luz para longe
		//lightPos.x -= 0.001f;
		//lightPos.z -= 0.001f;

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Observa se qualquer evento foi ativado(ex: teclado, mouse)
		glfwPollEvents();
		DoMovement();

		//cor alterada no colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.Use();
		GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(lightingShader.Program, "lightColor");
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(lightColorLoc, 1.0f, 0.5f, 1.0f);
		GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "light.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "light.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "light.quadratic"), 0.032f);

		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		//view
		glm::mat4 view;
		view = camera.GetViewMatrix();

		//Alterado para sem textura
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model;
		glBindVertexArray(boxVAO);

		//Desenhando os 10 triangulos
		for (GLuint i = 0; i < 10; i++) {
			model = glm::mat4();
			model = glm::translate(model, cubePositions[i]);

			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		lampShader.Use();

		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		//abrindo a janela
		glfwSwapBuffers(window);
	}

	//Desalocando os recursos
	glDeleteVertexArrays(1, &boxVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	//Encerrando o glfw
	glfwTerminate();

	return EXIT_SUCCESS;
}

int luz_spot_sem_textura() {

	glfwInit();

	//Inicializando GLFW com as devidas propriedades
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//Define que janela nao pode ter tamanho alterado
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Inicializa janela
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Somente luz spot sem textura", nullptr, nullptr);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	//Verifica se tudo ocorreu bem na criacao da janela
	if (nullptr == window) {
		std::cout << "Falha ao criar janela GLFW" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	//glfwSetScrollCallback(window, ScrollCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	glewExperimental = GL_TRUE;

	//inicializar GLWE

	if (GLEW_OK != glewInit()) {
		std::cout << "Falha ao inicializar GLEW" << std::endl;

		return EXIT_FAILURE;
	}

	//Define as dimensoes da viewport
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	Shader lightingShader("lightingspotsemtex.vs", "lightingspotsemtex.frag");

	// Vertices para Projecao Perspectiva de um cubo

	GLfloat vertices[] = {
		//coordenadas x,y,z		//normal dos vertices
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,   
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,		
	};

	// Posicoes de todos os 10 cubos na cena
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,   0.0f,   0.0f),
		glm::vec3(2.0f,   5.0f,   -15.0f),
		glm::vec3(-1.5f,  -2.2f,  -2.5f),
		glm::vec3(-3.8f,  -2.0f,  -12.3f),
		glm::vec3(2.4f,   -0.4f,  -3.5f),
		glm::vec3(-1.7f,  3.0f,   -7.5f),
		glm::vec3(1.3f,   -2.0f,  -2.5f),
		glm::vec3(1.5f,   2.0f,   -2.5f),
		glm::vec3(1.5f,   0.2f,   -1.5f),
		glm::vec3(-1.3f,  1.0f,   -1.5f)
	};

	GLuint VBO, boxVAO;
	glGenVertexArrays(1, &boxVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Bind o vextex array object primeiro e depois bind e set ponteiros de atributo
	glBindVertexArray(boxVAO);

	//Atributos de posicao para cubo sem textura
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Atributos da noormal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


	glBindVertexArray(0); //Unbind VAO

	lightingShader.Use();
	GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
	GLint lightColorLoc = glGetUniformLocation(lightingShader.Program, "lightColor");
	glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
	glUniform3f(lightColorLoc, 1.0f, 0.5f, 1.0f);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	while (!glfwWindowShouldClose(window)) {

		//Mover a fonte de luz para longe
		//lightPos.x -= 0.001f;
		//lightPos.z -= 0.001f;

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Observa se qualquer evento foi ativado(ex: teclado, mouse)
		glfwPollEvents();
		DoMovement();

		//cor alterada no colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.Use();
		GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
		GLint lightSpotDirLoc = glGetUniformLocation(lightingShader.Program, "light.direction");
		GLint lightSpotCutOffLoc = glGetUniformLocation(lightingShader.Program, "light.cutOff");
		GLint lightSpotOuterCutOffLoc = glGetUniformLocation(lightingShader.Program, "light.outerCutOff");
		//GLint lightDirLoc = glGetUniformLocation(lightingShader.Program, "light.direction");
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		//Preciso para luz pontual
		//glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(lightPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(lightSpotDirLoc, camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform1f(lightSpotCutOffLoc, glm::cos(glm::radians(12.5f)));
		glUniform1f(lightSpotOuterCutOffLoc, glm::cos(glm::radians(17.5f)));
		//glUniform3f(lightDirLoc, -0.2f, -1.0f, -0.3f);
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "light.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "light.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "light.quadratic"), 0.032f);

		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		//view
		glm::mat4 view;
		view = camera.GetViewMatrix();

		//Alterado para sem textura
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model;
		glBindVertexArray(boxVAO);

		//Desenhando os 10 triangulos
		for (GLuint i = 0; i < 10; i++) {
			model = glm::mat4();
			model = glm::translate(model, cubePositions[i]);

			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);

		//abrindo a janela
		glfwSwapBuffers(window);
	}

	//Desalocando os recursos
	glDeleteVertexArrays(1, &boxVAO);
	//glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	//Encerrando o glfw
	glfwTerminate();

	return EXIT_SUCCESS;
}

void resetarCamera() {
	lastX = WIDTH / 2.0f;
	lastY = WIDTH / 2.0f;
	firstMouse = true;

	deltaTime = 0.0f;
	lastFrame = 0.0f;
}

int main() {

	int retorno;

	//Com textura e material generico
	retorno = multiplas_luzes();
	resetarCamera();
	if (retorno == EXIT_FAILURE) {
		return retorno;
	}

	retorno = luz_direcional();
	resetarCamera();
	if (retorno == EXIT_FAILURE) {
		return retorno;
	}

	retorno = luz_pontual();
	resetarCamera();
	if (retorno == EXIT_FAILURE) {
		return retorno;
	}

	retorno = luz_spot();
	resetarCamera();
	if (retorno == EXIT_FAILURE) {
		return retorno;
	}

	//Sem textura e material generico
	retorno = multiplas_luzes_sem_textura();
	resetarCamera();
	if (retorno == EXIT_FAILURE) {
		return retorno;
	}

	retorno = luz_direcional_sem_textura();
	resetarCamera();
	if (retorno == EXIT_FAILURE) {
		return retorno;
	}

	retorno = luz_pontual_sem_textura();
	resetarCamera();
	if (retorno == EXIT_FAILURE) {
		return retorno;
	}

	retorno = luz_spot_sem_textura();
	resetarCamera();
	if (retorno == EXIT_FAILURE) {
		return retorno;
	}

	return EXIT_SUCCESS;
}

//Move e altera a posicao da cameta baseado na entrada do usuario na janela
void DoMovement()
{
	// Constroles da camera
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

//Eh chamado quando qualquer telca eh pressionada ou solta
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}