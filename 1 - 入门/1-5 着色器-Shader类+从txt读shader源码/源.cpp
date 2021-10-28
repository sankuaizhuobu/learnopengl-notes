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
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //如果按下escape键
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	glfwInit();  //初始化函式库
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //hint提示，主版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  //副版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //使用profile

	const char* version = (const char*)glGetString(GL_VERSION);
	printf("OpenGL Version : %s\n", version);  //为什么输出(NULL)？？？

	const GLubyte* name = glGetString(GL_VENDOR); //返回负责当前OpenGL实现厂商的名字
	const GLubyte* biaoshifu = glGetString(GL_RENDERER); //返回一个渲染器标识符，通常是个硬件平台
	const GLubyte* OpenGLVersion = glGetString(GL_VERSION); //返回当前OpenGL实现的版本号

	printf("OpenGL实现厂商的名字：%s\n", name);  //为什么输出(NULL)？？？
	printf("渲染器标识符：%s\n", biaoshifu);  //为什么输出(NULL)？？？
	printf("OpenGL实现的版本号：%s\n", OpenGLVersion);  //为什么输出(NULL)？？？

	//Open GLFW Window
	GLFWwindow* window = glfwCreateWindow(800, 600, "My OpenGL Game", NULL, NULL);
	if (window == NULL)                  //宽，高   
	{
		printf("Open Window Failed!"); 
		glfwTerminate();  
		return -1;
	}
	glfwMakeContextCurrent(window);  //tell GLFW to make the context of our window the main context on the current thread
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //回调函数

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);    //vertices中的数据从CPU到GPU的array buffer，即VBO
	
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  //VBO中的坐标->VAO，6号顶点属性
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));  //VBO中的颜色->VAO，7号顶点属性
	glEnableVertexAttribArray(1);

	//渲染循环
	while (!glfwWindowShouldClose(window)) 
	{
		processInput(window);

		float offset = 0.5f;
		shader->setFloat("xOffset", offset);
		
		glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);//清哪个buffer

		shader->use();  //使用shader program
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