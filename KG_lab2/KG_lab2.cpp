
#include <iostream>

#include<GL/glew.h>
#include<gl/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaders.h"
#include "Camera.h"
#include <iostream>
#include <soil.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);



// Константы
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 400;

// Камера
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Тайминги
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Освещение
glm::vec3 lightPos(1.2f, 0.5f, 2.0f);
glm::vec3 lightPos1(0.0f, 1.0f, 1.0f);
glm::vec3 objectPos(0.0f, 0.0f, 0.0f);

int main()
{
	// glfw: инициализация и конфигурирование
//	glfwInit();
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	// glfw: создание окна
	GLFWwindow* window = glfwCreateWindow(1100, 800, "Hello Cube", nullptr, 0);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Сообщаем GLFW, чтобы он захватил наш курсор
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	// Конфигурирование глобального состояния OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Компилирование нашей шейдерной программы
	Shader lightingShader("vsBasicLighting.txt", "fsBasicLighting.txt");
	Shader lightingShader1("vsBasicLighting.txt", "fsBasicLighting.txt");
	Shader lampShader("vsLamp.txt", "fsLamp.txt");
	Shader lampShader1("vsLamp.txt", "fsLamp.txt");

		//0, 1, 7, //лево
		//0, 5, 7,

		//5, 7, 6,
		//5, 4, 6,

		//6, 4, 0,
		//6, 1, 0,

		//1, 2, 9, //верх
		//1, 7, 9,

		//7, 9, 8,
		//7, 6, 8,

		//8, 6, 1,
		//8, 2, 1,

		//2, 3, 11, //право
		//2, 9, 11,

		//9, 11, 10,
		//9, 8, 10,

		//10, 8, 2,
		//10, 3, 2,

		//3, 0, 5, //низ
		//3, 11, 5,

		//11, 5, 4,
		//11, 10, 4,

		//4, 10, 3,
		//4, 0, 3


	//float vertices[] = {
	//		 0.0f,  0.0f,  0.0f,  0.0f,  0.5f, -1.0f,  0.0f, 0.0f, //0
	//		 0.0f,  0.3f,  0.0f,  0.0f,  0.5f, -1.0f,  1.0f, 0.0f, //1
	//		 0.3f,  0.3f,  0.0f,  0.0f,  0.5f, -1.0f,  1.0f, 1.0f, //2
	//		 0.3f,  0.0f,  0.0f,  0.0f,  0.5f, -1.0f,  1.0f, 1.0f, //3
	//		-0.2f, -0.2f,  0.3f,  0.0f,  0.5f, -1.0f,  0.0f, 1.0f, //4
	//		-0.2f, -0.2f, -0.3f,  0.0f,  0.5f, -1.0f,  0.0f, 0.0f, //5
	//		-0.2f,  0.5f,  0.3f,  0.0f,  0.5f, -1.0f,  0.0f, 0.0f, //6
	//		-0.2f,  0.5f, -0.3f,  0.0f,  0.5f, -1.0f,  0.5f, 1.0f, //7
	//		 0.5f,  0.5f,  0.3f,  0.0f,  0.5f, -1.0f,  1.0f, 0.0f, //8
	//		 0.5f,  0.5f, -0.3f,  0.0f,  0.5f, -1.0f,  0.0f, 0.0f, //9
	//		 0.5f, -0.2f,  0.3f,  0.0f,  0.5f, -1.0f,  0.5f, 1.0f, //10
	//		 0.5f, -0.2f, -0.3f,  0.0f,  0.5f, -1.0f,  1.0f, 0.0f, //11
	//};

	float vertices[] = {
			 0.0f,  0.0f,  0.0f,   0.5f,  0.0f, -1.0f,  0.0f, 1.0f,
			 0.0f,  0.3f,  0.0f,   0.5f,  0.0f, -1.0f,  0.0f, 0.0f,
			 -0.2f,  0.5f, -0.3f,  0.5f,  0.0f, -1.0f,  1.0f, 1.0f,

			 0.0f,  0.0f,  0.0f,   0.5f,  0.0f, -1.0f,  0.0f, 1.0f,
			 -0.2f, -0.2f, -0.3f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f,
			 -0.2f,  0.5f, -0.3f,  0.5f,  0.0f, -1.0f,  1.0f, 1.0f,

			 -0.2f, -0.2f, -0.3f,  -1.0f,  0.0f, 0.0f,   0.0f, 1.0f,
			 -0.2f,  0.5f, -0.3f,  -1.0f,  0.0f, 0.0f,  0.0f, 0.0f,
			 -0.2f,  0.5f,  0.3f,  -1.0f,  0.0f, 0.0f,  1.0f, 1.0f,

			 -0.2f, -0.2f, -0.3f,  -1.0f,  0.0f, 0.0f,   0.0f, 1.0f,
			 -0.2f, -0.2f,  0.3f,  -1.0f,  0.0f, 0.0f,  0.0f, 0.0f,
			 -0.2f,  0.5f,  0.3f,  -1.0f,  0.0f, 0.0f,  1.0f, 1.0f,

			 -0.2f,  0.5f,  0.3f,  0.5f,  0.0f, 1.0f,   0.0f, 1.0f,
			 -0.2f, -0.2f,  0.3f,  0.5f,  0.0f, 1.0f,  0.0f, 0.0f,
			  0.0f,  0.0f,  0.0f,  0.5f,  0.0f, 1.0f,  1.0f, 1.0f,

			  -0.2f,  0.5f,  0.3f, 0.5f,  0.0f, 1.0f,   0.0f, 1.0f,
			  0.0f,  0.3f,  0.0f,  0.5f,  0.0f, 1.0f,  0.0f, 0.0f,
			  0.0f,  0.0f,  0.0f,  0.5f,  0.0f, 1.0f,  1.0f, 1.0f,

			  0.0f,  0.3f,  0.0f,  0.0f,  -0.5f, -1.0f,   0.0f, 1.0f,
			  0.3f,  0.3f,  0.0f,  0.0f,  -0.5f, -1.0f,  0.0f, 0.0f,
			  0.5f,  0.5f, -0.3f,  0.0f,  -0.5f, -1.0f,  1.0f, 1.0f,

			  0.0f,  0.3f,  0.0f,  0.0f, -0.5f, -1.0f,   0.0f, 1.0f,
			  -0.2f,  0.5f, -0.3f, 0.0f, -0.5f, -1.0f,  0.0f, 0.0f,
			  0.5f,  0.5f, -0.3f,  0.0f, -0.5f, -1.0f,  1.0f, 1.0f,

			  -0.2f,  0.5f, -0.3f, 0.0f,  1.0f, 0.0f,   0.0f, 1.0f,
			  0.5f,  0.5f, -0.3f,  0.0f,  1.0f, 0.0f,  0.0f, 0.0f,
			  0.5f,  0.5f,  0.3f,  0.0f,  1.0f, 0.0f,  1.0f, 1.0f,

			  -0.2f,  0.5f, -0.3f,  0.0f,  1.0f, 0.0f,  0.0f, 1.0f,
			  -0.2f,  0.5f,  0.3f,  0.0f,  1.0f, 0.0f,  0.0f, 0.0f,
			  0.5f,  0.5f,  0.3f,   0.0f,  1.0f, 0.0f,  1.0f, 1.0f,

			  0.5f,  0.5f,  0.3f,   0.0f,  -0.5f, 1.0f,   0.0f, 1.0f,
			  -0.2f,  0.5f,  0.3f,  0.0f,  -0.5f, 1.0f,  0.0f, 0.0f,
			  0.0f,  0.3f,  0.0f,   0.0f,  -0.5f, 1.0f,  1.0f, 1.0f,

			   0.5f,  0.5f,  0.3f, 0.0f,  -0.5f, 1.0f,   0.0f, 1.0f,
			  0.3f,  0.3f,  0.0f,  0.0f,  -0.5f, 1.0f,  0.0f, 0.0f,
			  0.0f,  0.3f,  0.0f,  0.0f,  -0.5f, 1.0f,  1.0f, 1.0f,

			  0.3f,  0.3f,  0.0f,  -0.5f,  0.0f, -1.0f,   0.0f, 1.0f,
			  0.3f,  0.0f,  0.0f,  -0.5f,  0.0f, -1.0f,  0.0f, 0.0f,
			  0.5f, -0.2f, -0.3f,  -0.5f,  0.0f, -1.0f,  1.0f, 1.0f,

			  0.3f,  0.3f,  0.0f,  -0.5f,  0.0f, -1.0f,   0.0f, 1.0f,
			  0.5f,  0.5f, -0.3f,  -0.5f,  0.0f, -1.0f,  0.0f, 0.0f,
			  0.5f, -0.2f, -0.3f,  -0.5f,  0.0f, -1.0f,  1.0f, 1.0f,

			  0.5f,  0.5f, -0.3f,  1.0f,  0.0f, 0.0f,   0.0f, 1.0f,
			  0.5f, -0.2f, -0.3f,  1.0f,  0.0f, 0.0f,  0.0f, 0.0f,
			  0.5f, -0.2f,  0.3f,  1.0f,  0.0f, 0.0f,  1.0f, 1.0f,

			  0.5f,  0.5f, -0.3f,  1.0f,  0.0f, 0.0f,   0.0f, 1.0f,
			  0.5f,  0.5f,  0.3f,  1.0f,  0.0f, 0.0f,  0.0f, 0.0f,
			  0.5f, -0.2f,  0.3f,  1.0f,  0.0f, 0.0f,  1.0f, 1.0f,

			  0.5f, -0.2f,  0.3f,  -0.5f,  0.0f, 1.0f,   0.0f, 1.0f,
			  0.5f,  0.5f,  0.3f,  -0.5f,  0.0f, 1.0f,  0.0f, 0.0f,
			  0.3f,  0.3f,  0.0f,  -0.5f,  0.0f, 1.0f,  1.0f, 1.0f,

			  0.5f, -0.2f,  0.3f,  -0.5f,  0.0f, 1.0f,   0.0f, 1.0f,
			  0.3f,  0.0f,  0.0f,  -0.5f,  0.0f, 1.0f,  0.0f, 0.0f,
			  0.3f,  0.3f,  0.0f,  -0.5f,  0.0f, 1.0f,  1.0f, 1.0f,

			  0.3f,  0.0f,  0.0f,  0.0f,  0.5f, -1.0f,   0.0f, 1.0f,
			  0.0f,  0.0f,  0.0f,  0.0f,  0.5f, -1.0f,  0.0f, 0.0f,
			  -0.2f, -0.2f, -0.3f, 0.0f,  0.5f, -1.0f,  1.0f, 1.0f,

			  0.3f,  0.0f,  0.0f,  0.0f,  0.5f, -1.0f,   0.0f, 1.0f,
			  0.5f, -0.2f, -0.3f,  0.0f,  0.5f, -1.0f,  0.0f, 0.0f,
			  -0.2f, -0.2f, -0.3f, 0.0f,  0.5f, -1.0f,  1.0f, 1.0f,

			  0.5f, -0.2f, -0.3f,  0.0f,  -1.0f, 0.0f,   0.0f, 1.0f,
			  -0.2f, -0.2f, -0.3f, 0.0f,  -1.0f, 0.0f,  0.0f, 0.0f,
			  -0.2f, -0.2f,  0.3f, 0.0f,  -1.0f, 0.0f,  1.0f, 1.0f,

			  0.5f, -0.2f, -0.3f,  0.0f,  -1.0f, 0.0f,   0.0f, 1.0f,
			  0.5f, -0.2f,  0.3f,  0.0f,  -1.0f, 0.0f,  0.0f, 0.0f,
			  -0.2f, -0.2f,  0.3f, 0.0f,  -1.0f, 0.0f,  1.0f, 1.0f,

			  -0.2f, -0.2f,  0.3f, 0.0f,  0.5f, 1.0f,  0.0f, 1.0f,
			  0.5f, -0.2f,  0.3f,  0.0f,  0.5f, 1.0f,  0.0f, 0.0f,
			  0.3f,  0.0f,  0.0f,  0.0f,  0.5f, 1.0f,  1.0f, 1.0f,

			  -0.2f, -0.2f,  0.3f, 0.0f,  0.5f, 1.0f,  0.0f, 1.0f,
			  0.0f,  0.0f,  0.0f,  0.0f,  0.5f, 1.0f,  0.0f, 0.0f,
			  0.3f,  0.0f,  0.0f,  0.0f,  0.5f, 1.0f,  1.0f, 1.0f
	};

	


	// 1. Настраиваем VAO (и VBO)
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	/*glBindVertexArray(cubeVAO);*/

	// Координатные атрибуты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Атрибуты нормалей
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Атрибуты текстуры
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// Загрузка и создание текстур
	unsigned int texture1;

	// Текстура №1 - цветок
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Установка параметров наложения текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Установка параметров фильтрации текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Загрузка изображения, создание текстуры и генерирование мипмап-уровней
	int width, height, nrChannels;
	unsigned char* data = SOIL_load_image("texture.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}




	// 2. Настраиваем VAO света (VBO остается неизменным; вершины те же и для светового объекта, который также является пирамидкой)
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Обратите внимание, что мы обновляем шаг атрибута положения лампы, чтобы отразить обновленные данные буфера
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// Цикл рендеринга
	while (!glfwWindowShouldClose(window))
	{
		// Логическая часть работы со временем для каждого кадра
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Обработка ввода
		processInput(window);

		// Рендеринг
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 0.4f / 0.3f, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos1); // раскомментируй для движения света
		/*model = glm::translate(model, objectPos - lightPos);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, -(objectPos - lightPos));*/
		model = glm::scale(model, glm::vec3(0.2f));
		lampShader.setMat4("model", model);


		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 72);

		lampShader1.use();
		lampShader1.setMat4("projection", projection);
		lampShader1.setMat4("view", view);
		glm::mat4 model1 = glm::mat4(1.0f);
		model1 = glm::translate(model1, lightPos); // раскомментируй для движения света
		/*model1 = glm::translate(model1, objectPos - lightPos);
		model1 = glm::rotate(model1, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model1 = glm::translate(model1, -(objectPos - lightPos));*/
		model1 = glm::scale(model1, glm::vec3(0.2f));
		lampShader1.setMat4("model", model1);


		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 72);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);


		// Убеждаемся, что активировали шейдер прежде, чем настраивать uniform-переменные/объекты_рисования
		lightingShader.use();
		lightingShader.setInt("texture1", 0);
		lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("lightPos", glm::vec3(model[3]));
		lightingShader.setVec3("lightPos1", glm::vec3(model1[3]));
		lightingShader.setVec3("viewPos", camera.Position);

		// Преобразования Вида/Проекции
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		// Мировое преобразование
		model = glm::mat4(1.0f);
		model = glm::translate(model, objectPos);
		lightingShader.setMat4("model", model);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 72);

		// glfw: обмен содержимым front- и back- буферов. Отслеживание событий ввода/вывода (была ли нажата/отпущена кнопка, перемещен курсор мыши и т.п.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Опционально: освобождаем все ресурсы, как только они выполнили свое предназначение
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	// glfw: завершение, освобождение всех выделенных ранее GLFW-реcурсов
	glfwTerminate();
	return 0;
}

// Обработка всех событий ввода: запрос GLFW о нажатии/отпускании кнопки мыши в данном кадре и соответствующая обработка данных событий
void processInput(GLFWwindow* window)
{
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

// glfw: всякий раз, когда изменяются размеры окна (пользователем или операционной системой), вызывается данная callback-функция
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Убеждаемся, что окно просмотра соответствует новым размерам окна.
	// Обратите внимание, ширина и высота будут значительно больше, чем указано, на Retina-дисплеях
	glViewport(0, 0, width, height);
}


// glfw: всякий раз, когда перемещается мышь, вызывается данная callback-функция
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // перевернуто, так как y-координаты идут снизу вверх

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: всякий раз, когда прокручивается колесико мыши, вызывается данная callback-функция
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
