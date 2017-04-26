#pragma once
#include "include/ShapeData.h"
#include "include/ShapeAnim.h"
#include <string>
#include <vector>
#include "bone.h"
class ShapeGenerator
{
public:
	static ShapeData makeCube();
	static ShapeData makeQuad(float width, std::vector<float> texCoord);
	static ShapeData makeBox(glm::vec2 position, float width, float height);
	static ShapeAnim makePlayer(std::istream* in, bone* Bone);
	static ShapeData makeObject(std::istream* in);
	static ShapeData makePlane(int dimensions);
	static ShapeData makeTriangle();
	ShapeGenerator();
	~ShapeGenerator();
};

