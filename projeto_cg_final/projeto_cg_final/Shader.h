#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
public:

	GLuint Program;
	//Construtor para gerar o shader
	Shader(const GLchar *vertexPath, const GLchar *fragmentPath)
	{
		//Recupera o vertext e fragment shader da pasta
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		//garante que ifstream pode lancar excecoes
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		try {

			//Abre os arquivos
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			//Le os arquivos em stream
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			//fecha os manipuladores de arquivo
			vShaderFile.close();
			fShaderFile.close();
			//Converte de stream para string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "Erro: Arquivos de shader nao foram lidos com sucesso" << std::endl;
		}
		const GLchar *vShaderCode = vertexCode.c_str();
		const GLchar *fShaderCode = fragmentCode.c_str();

		//Compilando shaders
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];
		//Vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "Erro no Vertex Shader: Compilacao Falhou\n" << infoLog << std::endl;
		}

		//Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

		if (!success) {

			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "Erro no Fragment Shader: Compilacao Falhou\n" << infoLog << std::endl;
		}

		//Shader Program
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		glLinkProgram(this->Program);

		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
			std::cout << "Erro no Shader Program: Linking Falhou\n" << infoLog << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	//Usando o shader corrente
	void Use() {
		glUseProgram(this->Program);
	}
};

#endif