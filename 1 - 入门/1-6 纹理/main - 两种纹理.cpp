// main - ��������.cpp
#include<iostream>
#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include"Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float vertices[] = {
	//--- λ�� ---     --- ��ɫ ---      - �������� -
	0.5f,0.5f,0.0f,    1.0f,0.0f,0.0f,    1.0f, 1.0f,  //����
	0.5f,-0.5f,0.0f,    0.0f,1.0f,0.0f,    1.0f,0.0f,  //����
	-0.5f,-0.5f,0.0f,    0.0f,0.0f,1.0f,  0.0f, 0.0f,  //����
	-0.5f,0.5f,0.0f,   1.0f,1.0f,1.0f,    0.0f, 1.0f  //����
};

//float vertices[] = {
//	//--- λ�� ---     --- ��ɫ ---      - �������� -
//	0.5f,0.5f,0.0f,    1.0f,0.0f,0.0f,    2.0f, 2.0f,  //����
//	0.5f,-0.5f,0.0f,    0.0f,1.0f,0.0f,    2.0f,0.0f,  //����
//	-0.5f,-0.5f,0.0f,    0.0f,0.0f,1.0f,  0.0f, 0.0f,  //����
//	-0.5f,0.5f,0.0f,   1.0f,1.0f,1.0f,    0.0f, 2.0f  //����
//};

//float vertices[] = {
//	//--- λ�� ---     --- ��ɫ ---      - �������� -
//	0.5f,0.5f,0.0f,    1.0f,0.0f,0.0f,    0.1f, 0.1f,  //����
//	0.5f,-0.5f,0.0f,    0.0f,1.0f,0.0f,    0.1f,0.0f,  //����
//	-0.5f,-0.5f,0.0f,    0.0f,0.0f,1.0f,  0.0f, 0.0f,  //����
//	-0.5f,0.5f,0.0f,   1.0f,1.0f,1.0f,    0.0f, 0.1f  //����
//};

unsigned int indices[] = {  //��������
	0,1,3,  // first triangle
	1,2,3  // second triangle
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

GLfloat mixValue = 0.2f;
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {  //�������escape��
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		mixValue += 0.001f;
		if (mixValue > 1.0f) {
			mixValue = 1.0f;  //��ϱ�������0.0-1.0ʱ��ܹ���
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		mixValue -= 0.001f;
		if (mixValue < 0.0f) {
			mixValue = 0.0f;
		}
	}
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //ʹ��profile

	//Open GLFW Window
	GLFWwindow* window = glfwCreateWindow(800, 600, "My OpenGL Game", NULL, NULL);
	if (window == NULL) {                 //����   
		printf("Open Window Failed!");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);  //tell GLFW to make the context of our window the main context on the current thread
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //ע��ص�����

	//Init GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		printf("Init GLEW Failed!");
		glfwTerminate();
		return -1;
	}

	Shader* shader = new Shader("vertexShaderSource.txt", "fragmentShaderSource - ��������.txt");

	glViewport(0, 0, 800, 600);
	
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);    //vertices�е����ݴ�CPU��GPU��array buffer����VBO
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// posotion attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);  //VBO�е�����->VAO��0�Ŷ�������
	glEnableVertexAttribArray(0);  // ��������ԡ��������������Ա��
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));  //VBO�е���ɫ->VAO��1�Ŷ�������
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));  //VBO�е���������->VAO��2�Ŷ�������
	glEnableVertexAttribArray(2);

	//���ء������������
	unsigned int texture1, texture2;
	// texture1
	// --------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);  // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// set texture filtering parameters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);  //����ͼƬʱ��תy��
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);  //�������� data->GL_TEXTURE_2D��texBuffer��
		glGenerateMipmap(GL_TEXTURE_2D);  //Ϊ����GL_TEXTURE_2D�ϵĶ��󣨼�texture�����ɶ༶��Զ����mipmap
	} else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// texture2
	// --------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);  // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//����һ��float������Ϊ��Ե��ɫ
	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);  //�������� data->GL_TEXTURE_2D��texBuffer��
		glGenerateMipmap(GL_TEXTURE_2D);  //Ϊ����GL_TEXTURE_2D�ϵĶ��󣨼�texture�����ɶ༶��Զ����mipmap
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	shader->use();  //remember to activate the shader before setting uniforms!
	glUniform1i(glGetUniformLocation(shader->ID, "texture1"), 0);
	shader->setInt("texture2", 1);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// binding textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		shader->use();
		shader->setFloat("mixValue", mixValue);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// glfw: swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	//system("pause");
	glfwTerminate();
	return 0;
}