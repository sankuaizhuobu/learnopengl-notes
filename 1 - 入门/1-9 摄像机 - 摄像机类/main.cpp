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

//2、摄像机自由移动
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);  // 注视方向始终为前方
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//glm::vec3 cameraUp = glm::vec3(1.0f, 1.0f, 0.0f);  // 摄像机up轴为向右上方倾斜

// 移动速度
float deltaTime = 0.0f;  // 当前帧与上一帧的时间差
float lastFrame = 0.0f;  // 上一帧的时间

// 欧拉角
float pitch = 0.0f;  // 俯仰角
float yaw = 0.0f;  // 偏航角

// 鼠标输入
float lastX = screenWidth / 2;  // 鼠标位置初始值 屏幕中间
float lastY = screenHeight / 2;
// 鼠标进入窗口后置于中心
bool firstMouse = true;
// 鼠标滚轮缩放，field of view fov 视野
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

	// 第一次调用回调函数，初值设为鼠标此时的位置（好像没啥用呢？）
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	
	// 计算当前帧与上一帧鼠标位置的偏移量
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	// 灵敏度，控制 offset 不过大
	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	pitch += yoffset;
	yaw += xoffset;

	// 限制视线从天空到脚下
	if (pitch > 89.0) {  // 到达90度会翻转（？）
		pitch = 89.0;
	}
	if (pitch < -90.0) {
		pitch = -90.0;
	}

	// 通过俯仰角和偏航角计算新的方向向量front（从摄像机指向目标点）
	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);  // 鼠标控制视角，会改变 cameraFront，所以 cameraPos 的y会变为非0
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

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {  //如果按下escape键
		glfwSetWindowShouldClose(window, true);
	}

	//float cameraSpeed = 0.05f; // adjust accordingly 速度会因处理器不同
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;  // cameraPos.z 减小，摄像机向屏幕内移动【前进】
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;  // cameraPos.z 增大，摄像机向屏幕外移动【后退】
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;  // cameraPos.x 减小【右移】（cameraFront叉乘cameraUp标准化后=1,0,0即摄像机的单位右向量）
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;  // cameraPos.x 增大【左移】

	cameraPos.y = 0.0;  // 将摄像机限制在平面上
}

// 自己实现 lookAt矩阵
glm::mat4 lookAtMatrix(glm::vec3 cameraPos, glm::vec3 cameraTarget, glm::vec3 worldUp) {
	
	glm::vec3 cameraDirection = -glm::normalize(cameraTarget - cameraPos);
	glm::vec3 cameraRight = glm::normalize(glm::cross(cameraUp, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	// In glm we access elements as mat[col][row] due to column-major layout
	// ** glm 中的矩阵是以列为主

	// 构造旋转矩阵，用 camera 的右（x）、上（y）、方向矩阵（z）
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

	// 构造平移矩阵，用 cameraPos
	glm::mat4 translate = glm::mat4(1.0);
	translate[3][0] = -cameraPos.x;
	translate[3][1] = -cameraPos.y;
	translate[3][2] = -cameraPos.z;

	return rotate * translate;  // 先平移，再旋转
}

int main() {

	glfwInit();  // 初始化
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // 指定主版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // 指定副版本（3.3）
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //使用profile

	// Open GLFW Window
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Can't show Chinese", NULL, NULL);
	if (window == NULL) {                 //宽，高   
		printf("Open Window Failed!");
		glfwTerminate();
		return -1;
	}
	// tell GLFW to make the context of our window the main context on the current thread
	glfwMakeContextCurrent(window);
	
	// 注册回调函数
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
	glEnable(GL_DEPTH_TEST);  //开启深度测试，如果一个片段的z值比其他片段小，则会被挡住（即抛弃）
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader* shader = new Shader("vertexShaderSource.txt", "fragmentShaderSource.txt");

	glViewport(0, 0, 800, 600);  // 视口（绘画区域）

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);  // 产生 VAO
	unsigned int VBO;
	glGenBuffers(1, &VBO);  // 产生 VBO

	glBindVertexArray(VAO);  // 将生成的 VAO 绑定到 VertexArray

	glBindBuffer(GL_ARRAY_BUFFER, VBO);  // 将生成的 VBO 绑定到 Buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);    // vertices 中的数据从 CPU 到 GPU 的 array buffer，即 VBO

	// posotion attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);  // VBO 中的坐标->VAO，0号顶点属性
	glEnableVertexAttribArray(0);  // 激活顶点属性。参数：顶点属性编号
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));  // VBO 中的纹理坐标->VAO，1号顶点属性
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
	stbi_set_flip_vertically_on_load(true);  // 加载图片时翻转y轴
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);  // 生成纹理 data->GL_TEXTURE_2D（texBuffer）
		glGenerateMipmap(GL_TEXTURE_2D);  // 为绑定在GL_TEXTURE_2D上的对象（即texture）生成多级渐远纹理mipmap
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);  // 生成纹理 data->GL_TEXTURE_2D（texBuffer）
		glGenerateMipmap(GL_TEXTURE_2D);  // 为绑定在GL_TEXTURE_2D上的对象（即texture）生成多级渐远纹理mipmap
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

		// 移动速度
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
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(80.0f), glm::vec3(1.0f, 0.0f, 0.0f));  // 顺着x轴指向看，逆时针旋转
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));  // 观察矩阵（相当于往后退了3，+z指向屏幕内）
		shader->setMat4("view", view);
		glm::mat4 projection;
		//projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		projection = glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

		shader->use();

		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));  // value_ptr：获取起始地址（？）
		shader->setMat4("projection", projection);

		// camera/view transformation
	
		// 1、摄像机绕着原点水平旋转
		float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;  // 摄像机的 x 坐标
		float camZ = cos(glfwGetTime()) * radius;  // 摄像机的 z 坐标
		//view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		// 参数1：摄像机位置向量 2：目标位置向量 3：up向量
		// 改变位置：
		//view = glm::lookAt(glm::vec3(camX, 0.0f, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		// 改变目标：
		//view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//shader->setMat4("view", view);

		//2、摄像机自由移动
		// 参数2：注视的点随 cameraPos 变化而变化，cameraPos 与注视点的相对位置不变
		//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		// 使用自己实现的 lookAt矩阵
		view = lookAtMatrix(cameraPos, cameraPos + cameraFront, cameraUp);
		shader->setMat4("view", view);

		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; ++i) {
			glm::mat4 model = glm::mat4(1.0f);  // 新定义的 model，没用之前定义的 model
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));  // 让每个箱子有初始角度
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