﻿// XOpenGL.cpp: 定义应用程序的入口点。
//

#include "XOpenGL.h"

//注意，必须要先引入glad 头文件，然后才能引用glfw头文件，因为glfw头文件对glad有依赖。
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "XGL/XGL.hpp"

#define STRINGIZE(x) #x
#define SHADER(shader) "" STRINGIZE(shader)


using namespace std;

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int windowWidth = 1280;
	int windowHeight = 720;

	//创建窗口
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "opengl window",NULL, NULL);

	//opengl 运行起来是单线程模式 
	//如果要调用opengl的api必须使用同一个线程进行
	//多线程调用opengl是不允许的
	glfwMakeContextCurrent(window); //将openglcontext 与当前线程绑定 绑定成功之后，就必须使用该线程来调用opengl的api

	//glad初始化成功
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	//创建shader之前必须要先初始化glad
	
	//GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER); //创建片源着色器

	char* vertexShader = SHADER(
		#version 330\n
		uniform float a;
		layout(location = 0) in vec3 pos;
		out vec3 outPos;
		void main() 
		{
			outPos = pos;
			float _a = sin(a);
			gl_Position = vec4(pos.x*_a,pos.y*_a,pos.z*_a,1.0);
		}
	);

	char* fragmentShader = SHADER(
		#version 330\n
		out vec4 rgbaColor;
		in vec3 outPos;

		uniform Sample2D texture;

		void main() 
		{
			rgbaColor = vec4(outPos, 1.0);
		}
	);

	//顶点坐标数组 
	//float vertex[] = {
	//0.0f,1.0f,0.0f,
	//-1.0f,-1.0f,0.0f,
	//1.0f,-1.0f,0.0f
	//};

	float vertex[] = {
		-1.0,1.0,0.0,
		1.0,1.0,0.0,
		1.0,-1.0,0.0,
		-1.0,-1.0,0.0
	};

	unsigned int index[] = {
		0,3,1,
		1,3,2
	};

	XGLVAO* vao = new XGLVAO();
	vao->AddVertex3D(vertex, 4, 0);
	vao->SetIndex(index, 6);

	XGLProgram* program = new XGLProgram(vertexShader, fragmentShader);

	float aa = 0.0;
	while (!glfwWindowShouldClose(window)) //循环处理窗口关闭事件
	{
		//TODO 绘制操作
		glClear(GL_COLOR_BUFFER_BIT); //先清空颜色空间

		program->UseProgram(); //切换program

		//传入uniform变量 注意是在绘制的时候传入该值
		GLuint uniforma = glGetUniformLocation(program->program,"a");
		glUniform1f(uniforma,aa);
		aa+=0.1;

		//绘制 使用vertex顶点坐标方式
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//使用ebo的方式绘制顶点坐标
		vao->Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete program;

	delete vao;

	glfwTerminate();

	cout << "hello world" << endl;
	return 0;
}