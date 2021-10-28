//Shader.cpp
#include "Shader.h"

Shader::Shader(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath)
{
	//1�����ļ�·����ȡ����/Ƭ����ɫ��
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	//��֤ifstream��������׳��쳣
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);  //failbit���߼��ϴ򲻿��ļ���badbit���ĵ�����
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try  //try-catch��try������exception���ͻ���������catch
	{
		//���ļ�
		vShaderFile.open(vertexShaderPath);
		fShaderFile.open(fragmentShaderPath);
		std::stringstream vShaderStream, fShaderStream;
		//��ȡ�ļ��Ļ��嵽��������
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//�ر��ļ�������
		vShaderFile.close();
		fShaderFile.close();
		//ת����������string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure &e)  //�����쳣��ִ��
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " <<
			e.what() << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	//2��������ɫ��
	unsigned int vertex, fragment;
	/*int success;
	char infoLog[512];*/

	//������ɫ��
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);  //�������β����ͣ�const GLchar *const *
	glCompileShader(vertex);
	//��ӡ�����������еĻ�)
	/*glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}*/
	checkCompileErrors(vertex, "VERTEX");

	//Ƭ����ɫ��
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);  //�������β����ͣ�const GLchar *const *
	glCompileShader(fragment);
	//��ӡ�����������еĻ�)
	/*glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}*/
	checkCompileErrors(fragment, "FRAGMENT");

	//����Ϊ��ɫ������
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	//��ӡ���Ӵ�������еĻ���
	/*glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}*/
	checkCompileErrors(ID, "PROGRAM");

	//ɾ����ɫ�����룬��Ϊ�Ѿ����ӵ���������
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