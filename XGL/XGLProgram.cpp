#include "XGL.hpp"
#include <cstdio>
#include <cstdlib>

XGLProgram::XGLProgram(char* vShaderStr, char* fShaderStr)
{
	program = glCreateProgram();

	XGLShader vshader(vShaderStr, GLSHADERTYPE::XGL_SHADER_VERTEX);
	XGLShader fshader(fShaderStr, GLSHADERTYPE::XGL_SHADER_FRAGMENT);

	glAttachShader(program, vshader.shader);
	glAttachShader(program, fshader.shader);

	glLinkProgram(program);

	//check if have error
	GLint loglen = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &loglen);
	if (loglen > 0)
	{
		char* log = (char*)malloc(loglen);
		glGetProgramInfoLog(program, loglen, NULL, log);
		printf("program log:\n");
		printf("%s\t", log);

		free(log);
	}

}

int XGLProgram::UseProgram()
{
	glUseProgram(program); //switch opengl on this program
	return 0;
}

XGLProgram::~XGLProgram()
{
	if (program != 0)
	{
		glDeleteProgram(program);
		program = 0;
	}
}