#include "XGL.hpp"

XGLVAO::XGLVAO()
{
	glGenVertexArrays(1 ,&vao);

}

XGLVAO::~XGLVAO()
{

	for (unsigned int i = 0; i < vbos.size(); i++)
	{
		GLuint vbo = vbos[i];
		glDeleteBuffers(1, &vbo);
	}
	vbos.clear();

	if (ebo != 0)
	{
		glDeleteBuffers(1,&ebo);
	}

	if (vao != 0)
	{
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}
	
}

int XGLVAO::BindVAO()
{
	glBindVertexArray(vao);
	return 0;
}

int XGLVAO::AddVertex3D(float* data, int vertexCount,int layout)
{
	//bind vao
	glBindVertexArray(vao);

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);

	//bind vbo buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo); 

	//incoming vertex data
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), data, GL_STATIC_DRAW);
	//give shader vbo data
	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	//active vertex attribet array 
	glEnableVertexAttribArray(layout);
	//unbind vao
	glBindVertexArray(0);

	vbos.push_back(vbo);
	return 0;
}

int XGLVAO::SetIndex(unsigned int *indexData, int indexCount)
{
	//bind to vao 
	glBindVertexArray(vao);

	//create ebo
	glGenBuffers(1, &ebo);
	//bind ebo buffer 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	//give ebo buffer souce data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,  indexCount * sizeof(unsigned int), indexData, GL_STATIC_DRAW);


	glBindVertexArray(0);
	drawTime = indexCount;
	return 0;
}

int XGLVAO::Draw()
{
	BindVAO();

	glDrawElements(GL_TRIANGLES, drawTime, GL_UNSIGNED_INT, 0);

	return 0;
}