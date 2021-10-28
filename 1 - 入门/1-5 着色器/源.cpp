#include<iostream>
#define GLEW_STATIC  //glewʹ��static�汾��.lib��������.dll��
#include<GL/glew.h>
#include<GLFW/glfw3.h>

float vertices_triangle1[] = {
	-1.0f,0.0f,0.0f,
	1.0f,0.0f,0.5f,  //����ȣ�����һ�������ػᱻtriangle2��ס
	-0.5f,1.0f,0.0f };

float vertices_triangle2[] = {
	0.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	0.5f,1.0f,0.0f
};

float vertices[] =
{  //����                //��ɫ
	0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
	0.0f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
	0.0f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f,
	1.0f, 0.5f, 0.0f,    1.0f, 1.0f, 0.0f
};

const char* vertexShaderSource = 
"#version 330 core                                        \n"  //alt+a  ,3.3�汾��CORE_PROFILE
"layout(location = 0) in vec3 aPos;                       \n"
"out vec4 vertexColor;                                    \n"
"void main() {                                            \n"
"		gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);  \n"
"		vertexColor = vec4(0.5, 0.0, 0.0, 1.0);}		  \n";

const char* fragmentShaderSource_yellow =
"#version 330 core                                      \n"
"out vec4 FragColor;                                   \n"
"void main(){                                          \n"
"	FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);}         \n";

const char* fragmentShaderSource_blue =
"#version 330 core                                     \n"
"out vec4 FragColor;                                   \n"
"uniform vec4 ourColor;                                \n"
"void main(){                                          \n"
"	FragColor = ourColor;}                             \n";

const char* vertexShaderSource_2 =
"#version 330 core                                        \n"  //alt+a  ,3.3�汾��CORE_PROFILE
"layout(location = 6) in vec3 aPos;                       \n"
"layout(location = 7) in vec3 aColor;                     \n"
"out vec3 ourColor;                                       \n"
"void main() {                                            \n"
"		gl_Position = vec4(aPos, 1.0);                    \n"
"		ourColor = aColor;}		                          \n";

const char* fragmentShaderSource_gradient_color =
"#version 330 core                                     \n"
"out vec4 FragColor;                                   \n"
"in vec3 ourColor;                                       \n"
"void main(){                                          \n"
"	FragColor = vec4(ourColor, 1.0);}                    \n";


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	glfwInit(); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  

	GLFWwindow* window = glfwCreateWindow(800, 600, "My OpenGL Game", NULL, NULL);
	if (window == NULL)                 
	{
		printf("Open Window Failed!");  
		glfwTerminate();  
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;  
	if (glewInit() != GLEW_OK) 
	{
		printf("Init GLEW Failed!");
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);

	unsigned int VAO[3];
	glGenVertexArrays(3, VAO);  

	unsigned int VBO[3];
	glGenBuffers(3, VBO);  

	glBindVertexArray(VAO[0]);  
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);  
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangle1), vertices_triangle1, GL_STATIC_DRAW);  
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);  
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangle2), vertices_triangle2, GL_STATIC_DRAW);  
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  //����->VAO��6�Ŷ�������
	glEnableVertexAttribArray(6);

	glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));  //��ɫ->VAO��7�Ŷ�������
	glEnableVertexAttribArray(7);

	//����shader����
	//1��vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);  
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

	//2��fragment shader yellow
	unsigned int fragmentShader_yellow;
	fragmentShader_yellow = glCreateShader(GL_FRAGMENT_SHADER);  
	glShaderSource(fragmentShader_yellow, 1, &fragmentShaderSource_yellow, NULL);
	glCompileShader(fragmentShader_yellow);

	glGetShaderiv(fragmentShader_yellow, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader_yellow, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//3��fragment shader blue
	unsigned int fragmentShader_blue;
	fragmentShader_blue = glCreateShader(GL_FRAGMENT_SHADER);  
	glShaderSource(fragmentShader_blue, 1, &fragmentShaderSource_blue, NULL);
	glCompileShader(fragmentShader_blue);

	glGetShaderiv(fragmentShader_blue, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader_blue, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//4��vertex shader 2
	unsigned int vertexShader2;
	vertexShader2 = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader2, 1, &vertexShaderSource_2, NULL);
	glCompileShader(vertexShader2);

	glGetShaderiv(vertexShader2, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//5��fragmentShader gradient color
	unsigned int fragmentShader_gradient_color;
	fragmentShader_gradient_color = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader_gradient_color, 1, &fragmentShaderSource_gradient_color, NULL);
	glCompileShader(fragmentShader_gradient_color);

	glGetShaderiv(fragmentShader_gradient_color, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader_gradient_color, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//����shader���Ϊһ��program
	unsigned int shaderProgram1;
	shaderProgram1 = glCreateProgram();
	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader_yellow);
	glLinkProgram(shaderProgram1);

	glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	unsigned int shaderProgram2;
	shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader_blue);
	glLinkProgram(shaderProgram2);

	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	unsigned int shaderProgram3;
	shaderProgram3 = glCreateProgram();
	glAttachShader(shaderProgram3, vertexShader2);
	glAttachShader(shaderProgram3, fragmentShader_gradient_color);
	glLinkProgram(shaderProgram3);

	glGetProgramiv(shaderProgram3, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram3, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window)) 
	{
		processInput(window);  

		glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//triangle_1
		glBindVertexArray(VAO[0]);  
		glUseProgram(shaderProgram1);  
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//triangle_2
		//����uniform vec4 ourColor���ı�FragColor
		//����glGetUniformLocation��λourColor��Ȼ����glUniform4f�� ��λ����ourColor ��ֵ����ֵʱҪ����ourColor���ڵ�shader��
		float timeValue = glfwGetTime();
		float blueValue = sin(timeValue) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram2, "ourColor");
		glUseProgram(shaderProgram2);
		glUniform4f(vertexColorLocation, 0.0, 0.0, blueValue, 1.0f);


		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgram3);
		glBindVertexArray(VAO[2]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);

		glfwPollEvents();

	}

	system("pause");

	glDeleteVertexArrays(3, VAO);
	glDeleteBuffers(3, VBO);

	glfwTerminate();

	return 0;
}