#include "pch.h"
#include "ShapeGenerator.h"
#include <string>
#include <istream>
#include "Vertex.h"
#include <tiny_obj_loader.h>
using namespace std;
using namespace MathHelper;

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

ShapeGenerator::ShapeGenerator()
{
}


ShapeGenerator::~ShapeGenerator()
{
}
