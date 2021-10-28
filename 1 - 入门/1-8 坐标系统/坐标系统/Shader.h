//Shader.h
#pragma once

#ifndef SHADER_H
#define SHADER_H

#include<GL/glew.h>
#include<glm.hpp>

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

class Shader
{
public:
	//shader program ID
	unsigned int ID;
	//���캯������ȡshader���롢������ɫ��
	Shader(const GLchar* vertexShaderPath, const GLchar* frgmentSahderPath);
	//ʹ��/����shader program
	void use();
	//uniform���ߺ���
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4& mat) const;
		
private:
	void checkCompileErrors(GLuint ID, std::string type);
};

#endif