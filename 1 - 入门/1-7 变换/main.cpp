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

void test01()
{
	glm::vec4 vec(3.0f, 0.0f, 0.0f, 1.0f);  //��ʼ������λ��3,0,0
	glm::mat4 trans = glm::mat4(1.0f);  //λ�ƾ��󣬸տ�ʼ�ǵ�λ����
	trans = glm::translate(trans, glm::vec3(2.0f, 1.0f, 0.0f));  //����λ�ƾ���
	vec = trans * vec;
	std::cout << vec.x << vec.y << vec.z << std::endl;  //510
}

float vertices[] = {
	//--- λ�� ---         - �������� -
	 0.5f, 0.5f, 0.0f,    1.0f, 1.0f,  //����
	 0.5f,-0.5f, 0.0f,    1.0f, 0.0f,  //����
	-0.5f,-0.5f, 0.0f,    0.0f, 0.0f,  //����
	-0.5f, 0.5f, 0.0f,    0.0f, 1.0f   //����
};

unsigned int indices[] = {  //��������
	0, 1, 3,  // first triangle
	1, 2, 3   // second triangle
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {  //�������escape��
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	test01();

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);  //VBO�е�����->VAO��0�Ŷ�������
	glEnableVertexAttribArray(0);  // ��������ԡ��������������Ա��
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));  //VBO�е���������->VAO��1�Ŷ�������
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);  //�������� data->GL_TEXTURE_2D��texBuffer��
		glGenerateMipmap(GL_TEXTURE_2D);  //Ϊ����GL_TEXTURE_2D�ϵĶ��󣨼�texture�����ɶ༶��Զ����mipmap
	} else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	shader->use();  //remember to activate the shader before setting uniforms!
	glUniform1i(glGetUniformLocation(shader->ID, "texture1"), 0);
	shader->setInt("texture2", 1);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// binding textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// create transformations
		glm::mat4 trans = glm::mat4(1.0f);  //��ʼ��Ϊ��λ����
		//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		
		shader->use();
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));  //value_ptr����ȡ��ʼ��ַ������
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//�ڶ�������
		trans = glm::mat4(1.0f);  //��ʼ��Ϊ��λ����
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		float scale = sinf(glfwGetTime()) / 2 + 0.5f;
		trans = glm::scale(trans, glm::vec3(scale, scale, scale));

		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "transform"), 1, GL_FALSE, &trans[0][0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// glfw: swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}