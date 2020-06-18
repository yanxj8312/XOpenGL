// XOpenGL.cpp: 定义应用程序的入口点。
//

#include "XOpenGL.h"

//注意，必须要先引入glad 头文件，然后才能引用glfw头文件，因为glfw头文件对glad有依赖。
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "XGL/XGL.hpp"
#include "atlimage.h"
#include <iostream>
#include <fstream>

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
	

	char* vertexShader = R"(
		#version 330
		uniform float a;
		layout(location = 0) in vec3 pos;
		layout(location = 1) in vec3 uvPos;
		out vec3 outPos;
		out vec3 outUVPos;
		void main() 
		{
			outPos = pos;
			outUVPos = uvPos;
			//float _a = 1;
			gl_Position = vec4(pos.x,pos.y,pos.z,1.0);
		}
	)";

	char* fragmentShader = R"(
		#version 330
		out vec4 rgbaColor;
		in vec3 outPos;
		in vec3 outUVPos;
		in vec2 outWandH;

		//外部传入的变量都使用uniform表示
		uniform int width;
		uniform int height;
		uniform sampler2D text;
		void main() 
		{
			vec2 singleStepOffset = vec2(1.0/float(width),1.0/float(height));
			//纹理坐标
			vec2 uv = vec2(outUVPos.x,outUVPos.y);
			vec4 color = texture(text,uv);
			//rgbaColor = vec4(outPos, 1.0);
			rgbaColor = color;
		}
	)";

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

	float vertexUV[] = {
		0.0,1.0,0.0,
		1.0,1.0,0.0,
		1.0,0.0,0.0,
		0.0,0.0,0.0
	};

	unsigned int index[] = {
		0,3,1,
		1,3,2
	};

	XGLVAO* vao = new XGLVAO();
	vao->AddVertex3D(vertex, 4, 0);
	vao->AddVertex3D(vertexUV, 4, 1);
	vao->SetIndex(index, 6);

	XGLProgram* program = new XGLProgram(vertexShader, fragmentShader);

	/*int imgWidth = 2;
	int imgHeight = 2;
	unsigned char imgData[]={
		255,0,0,	0,255,0	,
		0,0,255,	127,127,127
	};*/

	BITMAP bm;
	CImage img;

	HRESULT hr = img.Load("D:\\XSOURCE\\test.jpg");

	if(!SUCCEEDED(hr))
	{
		printf("image file load failed\n");
		return -1;
	}

	HBITMAP hbmp = img;

	if(!GetObject(hbmp,sizeof(bm),&bm))
		return -1;
	


	float aa = 0.0f;

	GLuint textureId = 0;
	glGenTextures(1,&textureId);

	glBindTexture(GL_TEXTURE_2D,textureId);

	//将图片数据传入texture
	//texture setting 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	//big or small setting 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	printf("hello world\n");
	
	//传入数据
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,bm.bmWidth,bm.bmHeight,0,GL_BGR,GL_UNSIGNED_BYTE,bm.bmBits);

	GLuint widthLoc = glGetUniformLocation(program->program,"width");
	glUniform1i(widthLoc,bm.bmWidth);

	GLuint heightLoc = glGetUniformLocation(program->program,"height");
	glUniform1i(heightLoc,bm.bmHeight);

	//将texture 数据传入shader
	while (!glfwWindowShouldClose(window)) //循环处理窗口关闭事件
	{
		//TODO 绘制操作
		glClear(GL_COLOR_BUFFER_BIT); //先清空颜色空间

		program->UseProgram(); //切换program

		//传入uniform变量 注意是在绘制的时候传入该值
		GLuint uniforma = glGetUniformLocation(program->program,"a");
		glUniform1f(uniforma,aa);
		aa+=0.003;

		GLuint textureLoc = glGetUniformLocation(program->program,"text");

		//激活0号纹理单元
		glActiveTexture(GL_TEXTURE0);
		//将0号纹理单元与纹理绑定
		glBindTexture(GL_TEXTURE_2D,textureId);
		//绑定的是纹理单元
		glUniform1i(textureLoc,0);
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

	std::cout << "hello world" << endl;
	return 0;
}
