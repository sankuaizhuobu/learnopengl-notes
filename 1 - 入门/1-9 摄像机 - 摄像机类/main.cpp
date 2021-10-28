// main.cpp

#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

#include"Shader.h"

#include<iostream>

// settings
const unsigned int screenWidth = 800;
const unsigned int screenHeight = 600;

//2������������ƶ�
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);  // ע�ӷ���ʼ��Ϊǰ��
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//glm::vec3 cameraUp = glm::vec3(1.0f, 1.0f, 0.0f);  // �����up��Ϊ�����Ϸ���б

// �ƶ��ٶ�
float deltaTime = 0.0f;  // ��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f;  // ��һ֡��ʱ��

// ŷ����
float pitch = 0.0f;  // ������
float yaw = 0.0f;  // ƫ����

// �������
float lastX = screenWidth / 2;  // ���λ�ó�ʼֵ ��Ļ�м�
float lastY = screenHeight / 2;
// �����봰�ں���������
bool firstMouse = true;
// ���������ţ�field of view fov ��Ұ
float fov = 45.0f;

// a cube, 36 points
float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

// world space position of 10 cubes
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	// ��һ�ε��ûص���������ֵ��Ϊ����ʱ��λ�ã�����ûɶ���أ���
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	
	// ���㵱ǰ֡����һ֡���λ�õ�ƫ����
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	// �����ȣ����� offset ������
	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	pitch += yoffset;
	yaw += xoffset;

	// �������ߴ���յ�����
	if (pitch > 89.0) {  // ����90�Ȼᷭת������
		pitch = 89.0;
	}
	if (pitch < -90.0) {
		pitch = -90.0;
	}

	// ͨ�������Ǻ�ƫ���Ǽ����µķ�������front���������ָ��Ŀ��㣩
	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);  // �������ӽǣ���ı� cameraFront������ cameraPos ��y���Ϊ��0
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

	if (fov >= 1.0f && fov <= 45.0f) {
		fov -= yoffset;
	}
	if (fov < 1.0f) {
		fov = 1.0f;
	}
	if (fov > 45.0) {
		fov = 45.0f;
	}
}

void processInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {  //�������escape��
		glfwSetWindowShouldClose(window, true);
	}

	//float cameraSpeed = 0.05f; // adjust accordingly �ٶȻ���������ͬ
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;  // cameraPos.z ��С�����������Ļ���ƶ���ǰ����
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;  // cameraPos.z �������������Ļ���ƶ������ˡ�
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;  // cameraPos.x ��С�����ơ���cameraFront���cameraUp��׼����=1,0,0��������ĵ�λ��������
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;  // cameraPos.x �������ơ�

	cameraPos.y = 0.0;  // �������������ƽ����
}

// �Լ�ʵ�� lookAt����
glm::mat4 lookAtMatrix(glm::vec3 cameraPos, glm::vec3 cameraTarget, glm::vec3 worldUp) {
	
	glm::vec3 cameraDirection = -glm::normalize(cameraTarget - cameraPos);
	glm::vec3 cameraRight = glm::normalize(glm::cross(cameraUp, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	// In glm we access elements as mat[col][row] due to column-major layout
	// ** glm �еľ���������Ϊ��

	// ������ת������ camera ���ң�x�����ϣ�y�����������z��
	glm::mat4 rotate = glm::mat4(1.0);
	rotate[0][0] = cameraRight.x;
	rotate[1][0] = cameraRight.y;
	rotate[2][0] = cameraRight.z;
	rotate[0][1] = cameraUp.x;
	rotate[1][1] = cameraUp.y;
	rotate[2][1] = cameraUp.z;
	rotate[0][2] = cameraDirection.x;
	rotate[1][2] = cameraDirection.y;
	rotate[2][2] = cameraDirection.z;

	// ����ƽ�ƾ����� cameraPos
	glm::mat4 translate = glm::mat4(1.0);
	translate[3][0] = -cameraPos.x;
	translate[3][1] = -cameraPos.y;
	translate[3][2] = -cameraPos.z;

	return rotate * translate;  // ��ƽ�ƣ�����ת
}

int main() {

	glfwInit();  // ��ʼ��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // ָ�����汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // ָ�����汾��3.3��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //ʹ��profile

	// Open GLFW Window
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Can't show Chinese", NULL, NULL);
	if (window == NULL) {                 //����   
		printf("Open Window Failed!");
		glfwTerminate();
		return -1;
	}
	// tell GLFW to make the context of our window the main context on the current thread
	glfwMakeContextCurrent(window);
	
	// ע��ص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Init GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		printf("Init GLEW Failed!");
		glfwTerminate();
		return -1;
	}

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);  //������Ȳ��ԣ����һ��Ƭ�ε�zֵ������Ƭ��С����ᱻ��ס����������
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader* shader = new Shader("vertexShaderSource.txt", "fragmentShaderSource.txt");

	glViewport(0, 0, 800, 600);  // �ӿڣ��滭����

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);  // ���� VAO
	unsigned int VBO;
	glGenBuffers(1, &VBO);  // ���� VBO

	glBindVertexArray(VAO);  // �����ɵ� VAO �󶨵� VertexArray

	glBindBuffer(GL_ARRAY_BUFFER, VBO);  // �����ɵ� VBO �󶨵� Buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);    // vertices �е����ݴ� CPU �� GPU �� array buffer���� VBO

	// posotion attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);  // VBO �е�����->VAO��0�Ŷ�������
	glEnableVertexAttribArray(0);  // ��������ԡ��������������Ա��
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));  // VBO �е���������->VAO��1�Ŷ�������
	glEnableVertexAttribArray(1);

	//���ء������������
	unsigned int texture1, texture2;
	// texture1
	// --------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);  // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);  // ����ͼƬʱ��תy��
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);  // �������� data->GL_TEXTURE_2D��texBuffer��
		glGenerateMipmap(GL_TEXTURE_2D);  // Ϊ����GL_TEXTURE_2D�ϵĶ��󣨼�texture�����ɶ༶��Զ����mipmap
	} else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// texture2
	// --------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);  // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);  // �������� data->GL_TEXTURE_2D��texBuffer��
		glGenerateMipmap(GL_TEXTURE_2D);  // Ϊ����GL_TEXTURE_2D�ϵĶ��󣨼�texture�����ɶ༶��Զ����mipmap
	} else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	shader->use();  //remember to activate the shader before setting uniforms!
	shader->setInt("texture1", 0);
	shader->setInt("texture2", 1);

	// render loop
	while (!glfwWindowShouldClose(window)) {

		// �ƶ��ٶ�
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// binding textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// create transformations
		glm::mat4 model = glm::mat4(1.0f);  // make sure to initialize matrix to identity matrix first
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(80.0f), glm::vec3(1.0f, 0.0f, 0.0f));  // ˳��x��ָ�򿴣���ʱ����ת
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));  // �۲�����൱����������3��+zָ����Ļ�ڣ�
		shader->setMat4("view", view);
		glm::mat4 projection;
		//projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		projection = glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

		shader->use();

		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));  // value_ptr����ȡ��ʼ��ַ������
		shader->setMat4("projection", projection);

		// camera/view transformation
	
		// 1�����������ԭ��ˮƽ��ת
		float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;  // ������� x ����
		float camZ = cos(glfwGetTime()) * radius;  // ������� z ����
		//view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		// ����1�������λ������ 2��Ŀ��λ������ 3��up����
		// �ı�λ�ã�
		//view = glm::lookAt(glm::vec3(camX, 0.0f, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		// �ı�Ŀ�꣺
		//view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//shader->setMat4("view", view);

		//2������������ƶ�
		// ����2��ע�ӵĵ��� cameraPos �仯���仯��cameraPos ��ע�ӵ�����λ�ò���
		//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		// ʹ���Լ�ʵ�ֵ� lookAt����
		view = lookAtMatrix(cameraPos, cameraPos + cameraFront, cameraUp);
		shader->setMat4("view", view);

		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; ++i) {
			glm::mat4 model = glm::mat4(1.0f);  // �¶���� model��û��֮ǰ����� model
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));  // ��ÿ�������г�ʼ�Ƕ�
			shader->setMat4("model",model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// glfw: swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}