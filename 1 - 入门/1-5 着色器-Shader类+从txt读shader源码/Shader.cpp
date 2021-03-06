//Shader.cpp
#include "Shader.h"

Shader::Shader(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath)
{
	//1、从文件路径获取顶点/片段着色器
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	//保证ifstream对象可以抛出异常
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);  //failbit：逻辑上打不开文件，badbit：文档坏了
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try  //try-catch：try中遇到exception，就会立即跳入catch
	{
		//打开文件
		vShaderFile.open(vertexShaderPath);
		fShaderFile.open(fragmentShaderPath);
		std::stringstream vShaderStream, fShaderStream;
		//读取文件的缓冲到数据流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//关闭文件处理器
		vShaderFile.close();
		fShaderFile.close();
		//转换数据流到string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure &e)  //出现异常，执行
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " <<
			e.what() << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	//2、编译着色器
	unsigned int vertex, fragment;
	/*int success;
	char infoLog[512];*/

	//顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);  //第三个形参类型：const GLchar *const *
	glCompileShader(vertex);
	//打印编译错误（如果有的话)
	/*glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}*/
	checkCompileErrors(vertex, "VERTEX");

	//片段着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);  //第三个形参类型：const GLchar *const *
	glCompileShader(fragment);
	//打印编译错误（如果有的话)
	/*glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}*/
	checkCompileErrors(fragment, "FRAGMENT");

	//链接为着色器程序
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	//打印链接错误（如果有的话）
	/*glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}*/
	checkCompileErrors(ID, "PROGRAM");

	//删除着色器代码，因为已经链接到程序中了
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::checkCompileErrors(GLuint ID, std::string type) {
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
		if (!success) {
			std::cout << "ERROR::SHADER_COMPILATION_ERROE of type: "
				<< type << "\n" << infoLog << std::endl;
		}
	}
	else {
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: "
				<< type << "\n" << infoLog << std::endl;
		}
	}
}