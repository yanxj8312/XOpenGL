#include "XGL.hpp"
#include <cstdio>
#include <cstdlib>
XGLShader::XGLShader(char* shaderStr, GLSHADERTYPE type)
{
	if (type == GLSHADERTYPE::XGL_SHADER_VERTEX)
	{
		shader = glCreateShader(GL_VERTEX_SHADER); //create vertex shader
	}
	else if (type == GLSHADERTYPE::XGL_SHADER_FRAGMENT)
	{
		shader = glCreateShader(GL_FRAGMENT_SHADER); // create fragmetn shader
	}
	else
	{
		shader = glCreateShader(GL_VERTEX_SHADER); //create vertex shader
	}
	glShaderSource(shader, 1, &shaderStr, NULL);

	//compile shader
	glCompileShader(shader);

	//check if have error
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	GLint loglen = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &loglen);
	if (loglen > 0)
	{
		char* log = (char*)malloc(loglen);
		glGetShaderInfoLog(shader, loglen, NULL, log);
		printf("shader log:\n");
		printf("%s\t", log);

		free(log);
	}
}

XGLShader::~XGLShader()
{
	if (shader != 0)
	{
		glDeleteShader(shader);
		shader = 0;
	}
}