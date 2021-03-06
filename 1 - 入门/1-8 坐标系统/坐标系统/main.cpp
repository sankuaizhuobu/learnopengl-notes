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

void test01()
{
	glm::vec4 vec(3.0f, 0.0f, 0.0f, 1.0f);  //初始化顶点位置3,0,0
	glm::mat4 trans = glm::mat4(1.0f);  //位移矩阵，刚开始是单位矩阵
	trans = glm::translate(trans, glm::vec3(2.0f, 1.0f, 0.0f));  //创建位移矩阵
	vec = trans * vec;
	std::cout << vec.x << vec.y << vec.z << std::endl;  //510
}

//float vertices[] = {
//	//--- 位置 ---         - 纹理坐标 -
//	 0.5f, 0.5f, 0.0f,    1.0f, 1.0f,  //右上
//	 0.5f,-0.5f, 0.0f,    1.0f, 0.0f,  //右下
//	-0.5f,-0.5f, 0.0f,    0.0f, 0.0f,  //左下
//	-0.5f, 0.5f, 0.0f,    0.0f, 1.0f   //左上
//};

//unsigned int indices[] = {  //索引数组
//	0, 1, 3,  // first triangle
//	1, 2, 3   // second triangle
//};

//float vertices[] = {
//	 0.5f, 0.5f, 0.0f,    1.0f, 1.0f,  //右上
//   0.5f,-0.5f, 0.0f,    1.0f, 0.0f,  //右下
//	-0.5f, 0.5f, 0.0f,    0.0f, 1.0f,   //左上
//
//	 0.5f,-0.5f, 0.0f,    1.0f, 0.0f,  //右下
//	-0.5f,-0.5f, 0.0f,    0.0f, 0.0f,  //左下
//	-0.5f, 0.5f, 0.0f,    0.0f, 1.0f,   //左上
//};

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

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {  //如果按下escape键
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	test01();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //使用profile

	//Open GLFW Window
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Can't show Chinese", NULL, NULL);
	if (window == NULL) {                 //宽，高   
		printf("Open Window Failed!");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);  //tell GLFW to make the context of our window the main context on the current thread
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //注册回调函数

	//Init GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		printf("Init GLEW Failed!");
		glfwTerminate();
		return -1;
	}

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);  //开启深度测试，如果一个片段的z值比其他片段小，则会被挡住（即抛弃）

	Shader* shader = new Shader("vertexShaderSource.txt", "fragmentShaderSource.txt");

	glViewport(0, 0, 800, 600);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int EBO;
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);    //vertices中的数据从CPU到GPU的array buffer，即VBO

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// posotion attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);  //VBO中的坐标->VAO，0号顶点属性
	glEnableVertexAttribArray(0);  // 激活顶点属性。参数：顶点属性编号
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));  //VBO中的纹理坐标->VAO，1号顶点属性
	glEnableVertexAttribArray(1);

	//加载、创建纹理对象
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
	stbi_set_flip_vertically_on_load(true);  //加载图片时翻转y轴
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);  //生成纹理 data->GL_TEXTURE_2D（texBuffer）
		glGenerateMipmap(GL_TEXTURE_2D);  //为绑定在GL_TEXTURE_2D上的对象（即texture）生成多级渐远纹理mipmap
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);  //生成纹理 data->GL_TEXTURE_2D（texBuffer）
		glGenerateMipmap(GL_TEXTURE_2D);  //为绑定在GL_TEXTURE_2D上的对象（即texture）生成多级渐远纹理mipmap
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
		processInput(window);

		glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// binding textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// create transformations
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(80.0f), glm::vec3(1.0f, 0.0f, 0.0f));  //顺着x轴指向看，逆时针旋转
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		//projection = glm::perspective(glm::radians(60.0f), (float)screenWidth / (float)screenHeight * 2, 0.1f, 3.f);
		
		shader->use();

		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));  //value_ptr：获取起始地址（？）
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		shader->setMat4("view", view);
		//glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		shader->setMat4("projection", projection);

		glBindVertexArray(VAO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		for (unsigned int i = 0; i < 10; ++i) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i + 100.0f;
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle) * (i % 3 == 0), glm::vec3(1.0f, 0.3f, 0.5f));
			shader->setMat4("model",model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// glfw: swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}