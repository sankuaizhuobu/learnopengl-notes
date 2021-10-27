#include<iostream>
#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW/glfw3.h>

float vertices_triangle1[] = {
	-1.0f,0.0f,0.0f,
	1.0f,0.0f,0.5f,  //有深度，一部分像素被triangle2遮住
	-0.5f,1.0f,0.0f };

float vertices_triangle2[] = {
	0.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	0.5f,1.0f,0.0f
};
//0,1,2   2,1,3,

//unsigned int indices[] = {  //索引数组。索引从0开始
//	0,1,2,
//	1,2,3
//};

//shader代码 
const char* vertexShaderSource =  //字符串源代码 
"#version 330 core                                        \n"  //alt+a
"layout(location = 0) in vec3 aPos;                       \n"
"void main() {                                            \n"
"		gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);} \n";

const char* fragmentShaderSource_yellow =
"#version 330 core                                      \n"
"out vec4 FragColor;                                    \n"
"void main(){                                           \n"
"	FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);}          \n";

const char* fragmentShaderSource_blue =
"#version 330 core                                      \n"
"out vec4 FragColor;                                   \n"
"void main(){                                          \n"
"	FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);}         \n";


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
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

	//Open GLFW Window
	GLFWwindow* window = glfwCreateWindow(800, 600, "My OpenGL Game", NULL, NULL);
	if (window == NULL)                  //宽，高   
	{
		printf("Open Window Failed!");  //开窗失败
		glfwTerminate();  //终止
		return -1;
	}
	glfwMakeContextCurrent(window);  //tell GLFW to make the context of our window the main context on the current thread

	//Init GLEW
	glewExperimental = true;  //实验性质？？
	if (glewInit() != GLEW_OK)  //#define GLEW_OK 0
	{
		printf("Init GLEW Failed!");
		glfwTerminate();
		return -1;
	}

	//Before we can start rendering we have to do one last thing. 
	//We have to tell OpenGL the size of the rendering window 
	//so OpenGL knows how we want to display the data and coordinates with respect to the window. 
	//We can set those dimensions via the glViewport function
	glViewport(0, 0, 800, 600);
	//The first two parameters of glViewport set the location of the lower left corner of the window. 
	//The third and fourth parameter set the width and height of the rendering window in pixels, 
	//which we set equal to GLFW's window size.

	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //回调函数，每次窗口改变大小时，调用回调函数改变视口大小

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);  //剔除正面，三角形逆时针绘制为正面，顺时针绘制为背面。
	//glCullFace(GL_BACK);    //剔除背面：

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  //线框模式

	unsigned int VAO[2];
	glGenVertexArrays(2, VAO);  //产生一个VAO buffer
	//unsigned int VAO[10];
	//glGenBuffers(10, VAO);  //产生多个buffer，第二个参数是接收返回值的首地址

	unsigned int VBO[2];
	glGenBuffers(2, VBO);  //产生VBO

	glBindVertexArray(VAO[0]);  //绑定VAO和vertex array
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);  //array buffer - VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangle1), vertices_triangle1, GL_STATIC_DRAW);  //vertices数组 -> VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);  //array buffer - VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangle2), vertices_triangle2, GL_STATIC_DRAW);  //vertices数组 -> VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//unsigned int EBO;
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);  //element array buffer - EBO
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//编译shader代码
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);  //建立一个vertex shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShader_yellow;
	fragmentShader_yellow = glCreateShader(GL_FRAGMENT_SHADER);  //建立一个fragment shader
	glShaderSource(fragmentShader_yellow, 1, &fragmentShaderSource_yellow, NULL);
	glCompileShader(fragmentShader_yellow);

	unsigned int fragmentShader_blue;
	fragmentShader_blue = glCreateShader(GL_FRAGMENT_SHADER);  //建立一个fragment shader
	glShaderSource(fragmentShader_blue, 1, &fragmentShaderSource_blue, NULL);
	glCompileShader(fragmentShader_blue);

	//两个shader组合为一个program
	unsigned int shaderProgram1;
	shaderProgram1 = glCreateProgram();
	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader_yellow);
	glLinkProgram(shaderProgram1);

	glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int shaderProgram2;
	shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader_blue);
	glLinkProgram(shaderProgram2);

	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//glUseProgram(shaderProgram1);

	//渲染回圈（循环）
	while (!glfwWindowShouldClose(window)) //不关窗的时候
	{
		processInput(window);  //glfwPollEvents()需要时间

		//We can clear the screen's color buffer using glClear where we pass in buffer bits to specify which buffer we would like to clear. 
		//The possible bits we can set are GL_COLOR_BUFFER_BIT（颜色）, GL_DEPTH_BUFFER_BIT（深度） and GL_STENCIL_BUFFER_BIT.
		glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);//清哪个buffer

		glBindVertexArray(VAO[0]);  //VAO绑定到当前上下文
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
		glUseProgram(shaderProgram1);  //用这个shader program来画VAO
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(VAO[1]);
		glUseProgram(shaderProgram2);  //用这个shader program来画VAO
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(window);

		glfwPollEvents();

	}

	//此时glfwWindowShouldClose(window) == true
	system("pause");

	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);

	glfwTerminate();

	return 0;
}