#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <vector>

enum GLSHADERTYPE
{
	XGL_SHADER_VERTEX = 1,
	XGL_SHADER_FRAGMENT
};

class XGLShader
{
public:
	XGLShader(char* shaderStr, GLSHADERTYPE type);
	~XGLShader();
public:
	GLuint shader = 0;
};

class XGLProgram
{
public:
	XGLProgram(char* vShaderStr, char* fShaderStr);
	~XGLProgram();

	int UseProgram();

public:
	GLuint program = 0;
};

class XGLVAO
{
public:
	XGLVAO();
	~XGLVAO();
	int AddVertex3D(float* data, int vertexCount,int layout);
	//int AddVertex2D(float *data, int vertexCount);
	int BindVAO();
	int SetIndex(unsigned int *indexData, int indexCount);

	int Draw();
private:
	GLuint vao = 0;
	GLuint ebo = 0;

	int drawTime = 0;

	std::vector<GLuint> vbos;
};