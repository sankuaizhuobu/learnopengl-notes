//main.cpp
#include<iostream>
#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include"Shader.h"
#include<cstdio>

float vertices[] = {
	0.5f,-0.5f,0.0f,    1.0f,0.0f,0.0f,
	-0.5f,-0.5f,0.0f,    0.0f,1.0f,0.0f,
	0.0f,0.5f,0.0f,   0.0f,0.0f,1.0f 
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

int main()
{
	glfwInit();  //��ʼ����ʽ��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //hint��ʾ�����汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  //���汾��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //ʹ��profile

	const char* version = (const char*)glGetString(GL_VERSION);
	printf("OpenGL Version : %s\n", version);  //Ϊʲô���(NULL)������

	const GLubyte* name = glGetString(GL_VENDOR); //���ظ���ǰOpenGLʵ�ֳ��̵�����
	const GLubyte* biaoshifu = glGetString(GL_RENDERER); //����һ����Ⱦ����ʶ����ͨ���Ǹ�Ӳ��ƽ̨
	const GLubyte* OpenGLVersion = glGetString(GL_VERSION); //���ص�ǰOpenGLʵ�ֵİ汾��

	printf("OpenGLʵ�ֳ��̵����֣�%s\n", name);  //Ϊʲô���(NULL)������
	printf("��Ⱦ����ʶ����%s\n", biaoshifu);  //Ϊʲô���(NULL)������
	printf("OpenGLʵ�ֵİ汾�ţ�%s\n", OpenGLVersion);  //Ϊʲô���(NULL)������

	//Open GLFW Window
	GLFWwindow* window = glfwCreateWindow(800, 600, "My OpenGL Game", NULL, NULL);
	if (window == NULL)                  //����   
	{
		printf("Open Window Failed!"); 
		glfwTerminate();  
		return -1;
	}
	glfwMakeContextCurrent(window);  //tell GLFW to make the context of our window the main context on the current thread
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //�ص�����

	//Init GLEW
	glewExperimental = true;  
	if (glewInit() != GLEW_OK)  
	{
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
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);    //vertices�е����ݴ�CPU��GPU��array buffer����VBO
	
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  //VBO�е�����->VAO��6�Ŷ�������
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));  //VBO�е���ɫ->VAO��7�Ŷ�������
	glEnableVertexAttribArray(1);

	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window)) 
	{
		processInput(window);

		float offset = 0.5f;
		shader->setFloat("xOffset", offset);
		
		glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);//���ĸ�buffer

		shader->use();  //ʹ��shader program
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	system("pause");
	glfwTerminate();
	return 0;
}