// main.cpp
#include<iostream>
#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include"Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void test() {
	int width, height, nrChannels;  //ͼƬ�Ŀ��ߡ�ͨ����
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load("pic.png", &width, &height, &nrChannels, 0);

	for (size_t i = 0; i < 50; i++) {
		std::cout << (int)data[i] << std::endl;  //�鿴ǰ50�����ص���ɫֵ������
	}
	stbi_image_free(data);  //�ͷ��ڴ�
}

float vertices[] = {
	//--- λ�� ---     --- ��ɫ ---      - �������� -
	0.5f,0.5f,0.0f,    1.0f,0.0f,0.0f,    1.0f, 1.0f,  //����
	0.5f,-0.5f,0.0f,    0.0f,1.0f,0.0f,    1.0f,0.0f,  //����
	-0.5f,-0.5f,0.0f,    0.0f,0.0f,1.0f,  0.0f, 0.0f,  //����
	-0.5f,0.5f,0.0f,   1.0f,1.0f,1.0f,    0.0f, 1.0f  //����
};

unsigned int indices[] = {  //��������
	0,1,3,  // first triangle
	1,2,3  // second triangle
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //�������escape��
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	test();

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

	Shader* shader = new Shader("vertexShaderSource.txt", "fragmentShaderSource.txt");

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
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);  // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);  //�������� data->GL_TEXTURE_2D��texBuffer��
		glGenerateMipmap(GL_TEXTURE_2D);  //Ϊ����GL_TEXTURE_2D�ϵĶ��󣨼�texture�����ɶ༶��Զ����mipmap
	} else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader->use();
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