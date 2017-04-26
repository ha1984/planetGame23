#include "pch.h"
#include "ShapeGenerator.h"
#include <string>
#include <istream>
#include "Vertex.h"
#include <tiny_obj_loader.h>
#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)
using namespace std;
using namespace MathHelper;

ShapeData ShapeGenerator::makeCube() {
	ShapeData ret;

	Vertex myTri[24];
	
	myTri[0].position = vec3(-1.0f, +1.0f, +1.0f); // 0
	myTri[0].normal =  vec3(+0.0f, +1.0f, +0.0f); // normal
	myTri[0].texture = vec2(0.0f, 1.0f);
	myTri[1].position = vec3(+1.0f, +1.0f, +1.0f); // 1
	myTri[1].normal = vec3(+0.0f, +1.0f, +0.0f); // normal
	myTri[1].texture = vec2(1.0f, 1.0f);
	myTri[2].position = vec3(+1.0f, +1.0f, -1.0f); // 2
	myTri[2].normal = vec3(+0.0f, +1.0f, +0.0f); // normal
	myTri[2].texture = vec2(1.0f, 0.0f);
	myTri[3].position = vec3(-1.0f, +1.0f, -1.0f); // 3
	myTri[3].normal = vec3(+0.0f, +1.0f, +0.0f); // normal
	myTri[3].texture = vec2(0.0f, 0.0f);

	myTri[4].position = vec3(-1.0f, +1.0f, -1.0f); // 4
	myTri[4].normal = vec3(+0.0f, +0.0f, -1.0f); // normal
	myTri[4].texture = vec2(0.0f, 1.0f);
	myTri[5].position = vec3(+1.0f, +1.0f, -1.0f); // 5
	myTri[5].normal = vec3(+0.0f, +0.0f, -1.0f); // normal
	myTri[5].texture = vec2(1.0f, 1.0f);
	myTri[6].position = vec3(+1.0f, -1.0f, -1.0f); // 6
	myTri[6].normal = vec3(+0.0f, +0.0f, -1.0f); // normal
	myTri[6].texture = vec2(1.0f, 0.0f);
	myTri[7].position = vec3(-1.0f, -1.0f, -1.0f); // 7
	myTri[7].normal = vec3(+0.0f, +0.0f, -1.0f); // normal
	myTri[7].texture = vec2(0.0f, 0.0f);

	myTri[8].position = vec3(+1.0f, +1.0f, -1.0f); // 8
	myTri[8].normal = vec3(+1.0f, +0.0f, +0.0f); // normal
	myTri[8].texture = vec2(0.0f, 1.0f);
	myTri[9].position = vec3(+1.0f, +1.0f, +1.0f); // 9
	myTri[9].normal = vec3(+1.0f, +0.0f, +0.0f); // normal
	myTri[9].texture = vec2(1.0f, 1.0f);
	myTri[10].position = vec3(+1.0f, -1.0f, +1.0f); // 10
	myTri[10].normal = vec3(+1.0f, +0.0f, +0.0f); // normal
	myTri[10].texture = vec2(1.0f, 0.0f);
	myTri[11].position = vec3(+1.0f, -1.0f, -1.0f); // 11
	myTri[11].normal = vec3(+1.0f, +0.0f, +0.0f); // normal
	myTri[11].texture = vec2(0.0f, 0.0f);

	myTri[12].position = vec3(-1.0f, +1.0f, +1.0f); // 12
	myTri[12].normal = vec3(-1.0f, +0.0f, +0.0f); // normal
	myTri[12].texture = vec2(0.0f, 1.0f);
	myTri[13].position = vec3(-1.0f, +1.0f, -1.0f); // 13
	myTri[13].normal = vec3(-1.0f, +0.0f, +0.0f); // normal
	myTri[13].texture = vec2(1.0f, 1.0f);
	myTri[14].position = vec3(-1.0f, -1.0f, -1.0f); // 14
	myTri[14].normal = vec3(-1.0f, +0.0f, +0.0f); // normal
	myTri[14].texture = vec2(1.0f, 0.0f);
	myTri[15].position = vec3(-1.0f, -1.0f, +1.0f); // 15
	myTri[15].normal = vec3(-1.0f, +0.0f, +0.0f); // normal
	myTri[15].texture = vec2(0.0f, 0.0f);

	myTri[16].position = vec3(+1.0f, +1.0f, +1.0f); // 16
	myTri[16].normal = vec3(+0.0f, +0.0f, +1.0f); // normal
	myTri[16].texture = vec2(0.0f, 1.0f);
	myTri[17].position = vec3(-1.0f, +1.0f, +1.0f); // 17
	myTri[17].normal = vec3(+0.0f, +0.0f, +1.0f); // normal
	myTri[17].texture = vec2(1.0f, 1.0f);
	myTri[18].position = vec3(-1.0f, -1.0f, +1.0f); // 18
	myTri[18].normal = vec3(+0.0f, +0.0f, +1.0f); // normal
	myTri[18].texture = vec2(1.0f, 0.0f);
	myTri[19].position = vec3(+1.0f, -1.0f, +1.0f); // 19
	myTri[19].normal = vec3(+0.0f, +0.0f, +1.0f); // normal
	myTri[19].texture = vec2(0.0f, 0.0f);

	myTri[20].position = vec3(+1.0f, -1.0f, -1.0f); // 20
	myTri[20].normal = vec3(+0.0f, -1.0f, +0.0f); // normal
	myTri[20].texture = vec2(0.0f, 1.0f);
	myTri[21].position = vec3(-1.0f, -1.0f, -1.0f); // 21
	myTri[21].normal = vec3(+0.0f, -1.0f, +0.0f); // normal
	myTri[21].texture = vec2(1.0f, 1.0f);
	myTri[22].position = vec3(-1.0f, -1.0f, +1.0f); // 22
	myTri[22].normal = vec3(+0.0f, -1.0f, +0.0f); // normal
	myTri[22].texture = vec2(1.0f, 0.0f);
	myTri[23].position = vec3(+1.0f, -1.0f, +1.0f); // 23
	myTri[23].normal = vec3(+0.0f, -1.0f, +0.0f); // normal
	myTri[23].texture = vec2(0.0f, 0.0f);


	ret.numVertices = 24;
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, myTri, sizeof(myTri));

	GLushort indices[] = {
		0, 1, 2, 0, 2, 3, // Top
		4, 5, 6, 4, 6, 7, // Front
		8, 9, 10, 8, 10, 11, // Right
		12, 13, 14, 12, 14, 15, // Left
		16, 17, 18, 16, 18, 19, // Back
		20, 22, 21, 20, 23, 22, // Bottom
	};

	ret.numindices = 36;
	ret.indices = new GLushort[ret.numindices];

	memcpy(ret.indices, indices, sizeof(indices));
	return ret;
}

ShapeData ShapeGenerator::makeQuad(float width, std::vector<float> texCoord) {
	ShapeData ret;

	ret.numVertices = 6;
	ret.vertices = new Vertex[ret.numVertices];

	ret.numindices = 6;
	ret.indices = new GLushort[ret.numindices];

	int k = 0;
	//int half = dimensions / 2;
	int half = 0;
	
	Vertex& thisVert0 = ret.vertices[0];
	thisVert0.position = vec3(-0.8f, 0.40625f, +0.0f);
	thisVert0.normal = vec3(+0.0f, +0.0f, 1.0f);
	thisVert0.texture = vec2(texCoord[0], texCoord[1]);
	ret.indices[k] = (GLushort)k;
	k += 1;

	Vertex& thisVert1 = ret.vertices[1];
	thisVert1.position = vec3(-0.8f + width, +0.5f, +0.0f);
	thisVert1.normal = vec3(+0.0f, +0.0f, 1.0f);
	thisVert1.texture = vec2(texCoord[4], texCoord[5]);
	ret.indices[k] = (GLushort)k;
	k += 1;

	Vertex& thisVert2 = ret.vertices[2];
	thisVert2.position = vec3(-0.8f, 0.5f, +0.0f);
	thisVert2.normal = vec3(+0.0f, +0.0f, 1.0f);
	thisVert2.texture = vec2(texCoord[2], texCoord[3]);
	ret.indices[k] = (GLushort)k;
	k += 1;

	Vertex& thisVert3 = ret.vertices[3];
	thisVert3.position = vec3(-0.8f + width, +0.5f, +0.0f);
	thisVert3.normal = vec3(+0.0f, +0.0f, 1.0f);
	thisVert3.texture = vec2(texCoord[4], texCoord[5]);
	ret.indices[k] = (GLushort)k;
	k += 1;

	Vertex& thisVert4 = ret.vertices[4];
	thisVert4.position = vec3(-0.8f, 0.40625f, +0.0f);
	thisVert4.normal = vec3(+0.0f, +0.0f, 1.0f);
	thisVert4.texture = vec2(texCoord[0], texCoord[1]);
	ret.indices[k] = (GLushort)k;
	k += 1;

	Vertex& thisVert5 = ret.vertices[5];
	thisVert5.position = vec3(-0.8f + width, 0.40625f, +0.0f);
	thisVert5.normal = vec3(+0.0f, +0.0f, 1.0f);
	thisVert5.texture = vec2(texCoord[6], texCoord[7]);
	ret.indices[k] = (GLushort)k;
	k += 1;

	return ret;
}

ShapeData ShapeGenerator::makeBox(glm::vec2 position, float width, float height) {
	ShapeData ret;

	ret.numVertices = 6;
	ret.vertices = new Vertex[ret.numVertices];

	ret.numindices = 6;
	ret.indices = new GLushort[ret.numindices];

	int k = 0;
	//int half = dimensions / 2;
	int half = 0;
    

	Vertex& thisVert0 = ret.vertices[0];
	thisVert0.position = vec3(position.x, position.y, +0.0f);
	thisVert0.normal = vec3(+0.0f, +0.0f, +1.0f);
	thisVert0.texture = vec2(0.0f, 0.0f);
	ret.indices[k] = (GLushort)k;
	k += 1;

	Vertex& thisVert1 = ret.vertices[1];
	thisVert1.position = vec3(position.x + width, position.y+height, +0.0f);
	thisVert1.normal = vec3(+0.0f, +0.0f, +1.0f);
	thisVert1.texture = vec2(1.0f, 1.0f);
	ret.indices[k] = (GLushort)k;
	k += 1;

	Vertex& thisVert2 = ret.vertices[2];
	thisVert2.position = vec3(position.x, position.y+height, +0.0f);
	thisVert2.normal = vec3(+0.0f, +0.0f, +1.0f);
	thisVert2.texture = vec2(0.0f, 1.0f);
	ret.indices[k] = (GLushort)k;
	k += 1;

	Vertex& thisVert3 = ret.vertices[3];
	thisVert3.position = vec3(position.x + width, position.y + height, +0.0f);
	thisVert3.normal = vec3(+0.0f, +0.0f, +1.0f);
	thisVert3.texture = vec2(1.0f, 1.0f);
	ret.indices[k] = (GLushort)k;
	k += 1;

	Vertex& thisVert4 = ret.vertices[4];
	thisVert4.position = vec3(position.x, position.y, +0.0f);
	thisVert4.normal = vec3(+0.0f, +0.0f, +1.0f);
	thisVert4.texture = vec2(0.0f,0.0f);
	ret.indices[k] = (GLushort)k;
	k += 1;

	Vertex& thisVert5 = ret.vertices[5];
	thisVert5.position = vec3(position.x + width, position.y, +0.0f);
	thisVert5.normal = vec3(+0.0f, +0.0f, +1.0f);
	thisVert5.texture = vec2(1.0f, 0.0f);
	ret.indices[k] = (GLushort)k;
	k += 1;

	return ret;
}

ShapeAnim ShapeGenerator::makePlayer(std::istream * in, bone* Bone)
{
	ShapeAnim ret;

	//std::string inputfile = objfile;
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool retr = tinyobj::LoadObj2(&attrib, &shapes, &materials, &err, in, false);

	//total number of vertices = 3*(number of triangles in shape 0):
	int arraysize = 0;
	for (int i = 0; i < shapes.size(); i++) {
		arraysize += 3 * shapes[i].mesh.num_face_vertices.size();
	}
	ret.numVertices = arraysize;
	ret.vertices = new VertexAnimate[ret.numVertices];

	ret.numindices = arraysize;
	ret.indices = new GLushort[ret.numindices];

	int k = 0;
	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		//for (size_t s = 0; s < 2; s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];
			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				if (k < arraysize) {
					ret.vertices[k].position[0] = attrib.vertices[3 * idx.vertex_index + 0];
					ret.vertices[k].position[1] = attrib.vertices[3 * idx.vertex_index + 1];
					ret.vertices[k].position[2] = attrib.vertices[3 * idx.vertex_index + 2];
					ret.vertices[k].normal[0] = attrib.normals[3 * idx.normal_index + 0];
					ret.vertices[k].normal[1] = attrib.normals[3 * idx.normal_index + 1];
					ret.vertices[k].normal[2] = attrib.normals[3 * idx.normal_index + 2];
					ret.vertices[k].texture[0] = attrib.texcoords[2 * idx.texcoord_index + 0];
					ret.vertices[k].texture[1] = 1.0f-attrib.texcoords[2 * idx.texcoord_index + 1];
					ret.indices[k] = (GLushort)k;
				}
				k += 1;
			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}

    //fill skin indices and weights of bone array into vertexanim array:
	for (int j = 0; j < 14; j++) {
		for (int i = 0; i < Bone[j].indices.size(); i++) {
			int idx = Bone[j].indices[i];
			if (ret.vertices[idx].index.x < 0) {
				ret.vertices[idx].index.x = (float)j;
				ret.vertices[idx].weights.x = Bone[j].weights[i];
			}
			else if (ret.vertices[idx].index.y < 0) {
				ret.vertices[idx].index.y = (float)j;
				ret.vertices[idx].weights.y = Bone[j].weights[i];
			}
			else if (ret.vertices[idx].index.z < 0) {
				ret.vertices[idx].index.z = (float)j;
				ret.vertices[idx].weights.z = Bone[j].weights[i];
			}
			else {}
		}
    }

	return ret;
}

ShapeData ShapeGenerator::makeObject(std::istream* in)
{
	ShapeData ret;

	//std::string inputfile = objfile;
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool retr = tinyobj::LoadObj2(&attrib, &shapes, &materials, &err, in, false);

	//total number of vertices = 3*(number of triangles in shape 0):
	int arraysize = 0;
	for (int i = 0; i < shapes.size(); i++) {
		arraysize += 4 * shapes[i].mesh.num_face_vertices.size();
	}
	ret.numVertices = arraysize;
	ret.vertices = new Vertex[ret.numVertices];

	ret.numindices = arraysize;
	ret.indices = new GLushort[ret.numindices];

	int k = 0;
	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		//for (size_t s = 0; s < 2; s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];
			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				if (k < arraysize) {
					ret.vertices[k].position.m[0] = attrib.vertices[3 * idx.vertex_index + 0];
					ret.vertices[k].position.m[1] = attrib.vertices[3 * idx.vertex_index + 1];
					ret.vertices[k].position.m[2] = attrib.vertices[3 * idx.vertex_index + 2];
					ret.vertices[k].normal.m[0] = attrib.normals[3 * idx.normal_index + 0];
					ret.vertices[k].normal.m[1] = attrib.normals[3 * idx.normal_index + 1];
					ret.vertices[k].normal.m[2] = attrib.normals[3 * idx.normal_index + 2];
					ret.vertices[k].texture.m[0] = attrib.texcoords[2 * idx.texcoord_index + 0];
					ret.vertices[k].texture.m[1] = attrib.texcoords[2 * idx.texcoord_index + 1];
					ret.indices[k] = (GLushort)k;
				}
				k += 1;
			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}

	//std::memcpy(ret.vertices, myTri, sizeof(myTri));

	//std::memcpy(ret.indices, indices, sizeof(indices));
	return ret;
}

ShapeData ShapeGenerator::makePlane(int dimensions) {
	ShapeData ret;

	ret.numVertices = 6 * dimensions*dimensions;
	ret.vertices = new Vertex[ret.numVertices];

	ret.numindices = 6 * dimensions*dimensions;
	ret.indices = new GLushort[ret.numindices];

	int k = 0;
	int half = dimensions / 2;
	//int half = 0;
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			Vertex& thisVert0 = ret.vertices[6 * (i*dimensions + j) + 0];
			thisVert0.position = vec3((j - half)*2.0f, +0.0f, (i - half)*2.0f);
			thisVert0.normal = vec3(+0.0f, +1.0f, +0.0f);
			thisVert0.texture = vec2(0.0f, 0.0f);
			ret.indices[k] = (GLushort)k;
			k += 1;

			Vertex& thisVert1 = ret.vertices[6 * (i*dimensions + j) + 1];
			thisVert1.position = vec3((j - half)*2.0f, +0.0f, +2.0f + (i - half)*2.0f);
			thisVert1.normal = vec3(+0.0f, +1.0f, +0.0f);
			thisVert1.texture = vec2(0.0f, 1.0f);
			ret.indices[k] = (GLushort)k;
			k += 1;

			Vertex& thisVert2 = ret.vertices[6 * (i*dimensions + j) + 2];
			thisVert2.position = vec3(+2.0f + (j - half)*2.0f, +0.0f, +2.0f + (i - half)*2.0f);
			thisVert2.normal = vec3(+0.0f, +1.0f, +0.0f);
			thisVert2.texture = vec2(1.0f, 1.0f);
			ret.indices[k] = (GLushort)k;
			k += 1;

			Vertex& thisVert3 = ret.vertices[6 * (i*dimensions + j) + 3];
			thisVert3.position = vec3(+2.0f + (j - half)*2.0f, +0.0f, +2.0f + (i - half)*2.0f);
			thisVert3.normal = vec3(+0.0f, +1.0f, +0.0f);
			thisVert3.texture = vec2(1.0f, 1.0f);
			ret.indices[k] = (GLushort)k;
			k += 1;

			Vertex& thisVert4 = ret.vertices[6 * (i*dimensions + j) + 4];
			thisVert4.position = vec3(+2.0f + (j - half)*2.0f, +0.0f, (i - half)*2.0f);
			thisVert4.normal = vec3(+0.0f, +1.0f, +0.0f);
			thisVert4.texture = vec2(+1.0f, 0.0f);
			ret.indices[k] = (GLushort)k;
			k += 1;

			Vertex& thisVert5 = ret.vertices[6 * (i*dimensions + j) + 5];
			thisVert5.position = vec3((j - half)*2.0f, +0.0f, (i - half)*2.0f);
			thisVert5.normal = vec3(+0.0f, +1.0f, +0.0f);
			thisVert5.texture = vec2(0.0f, 0.0f);
			ret.indices[k] = (GLushort)k;
			k += 1;
		}
	}

	return ret;
}

ShapeData ShapeGenerator::makeTriangle() {
	ShapeData ret;

	Vertex myTri[3];

	myTri[0].position = vec3(-1.0f, -1.0f, +0.0f);
	myTri[0].normal  = vec3(+0.0f, +0.0f, +1.0f);
	myTri[0].texture = vec2(0.4f, 0.0f);

	myTri[1].position = vec3(+0.0f, +1.0f, +0.0f);
	myTri[1].normal = vec3(+0.0f, +0.0f, +1.0f);
	myTri[1].texture = vec2(0.45f, 0.1f);

	myTri[2].position = vec3(+1.0f, -1.0f, +0.0f);
	myTri[2].normal = vec3(+0.0f, +0.0f, +1.0f);
	myTri[2].texture = vec2(0.5f, 0.0f);


	ret.numVertices = 3;
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, myTri, sizeof(myTri));

	GLushort indices[] = { 0, 1, 2 };
	ret.numindices = 3;
	ret.indices = new GLushort[ret.numindices];

	memcpy(ret.indices, indices, sizeof(indices));
	return ret;
}

ShapeGenerator::ShapeGenerator()
{
}


ShapeGenerator::~ShapeGenerator()
{
}
