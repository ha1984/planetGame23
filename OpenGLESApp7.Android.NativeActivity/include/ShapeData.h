#pragma once
//#include <gl\glew.h>
#include "../Vertex.h"
#include <GLES2/gl2.h>

struct ShapeData {
	 //constructor to initialize members:
	ShapeData() :
		vertices(0), numVertices(0),
		indices(0), numindices(0) {}
	Vertex* vertices;
	GLuint numVertices;
	GLushort* indices;
	GLuint numindices;
	GLsizeiptr vertexBufferSize() const {
		return numVertices * sizeof(Vertex);
	}
	GLsizeiptr indexBufferSize() const {
		return numindices * sizeof(GLushort);
	}
};