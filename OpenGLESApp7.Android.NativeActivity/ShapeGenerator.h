#pragma once
#include "include/ShapeData.h"
#include <string>
#include <vector>
class ShapeGenerator
{
public:
	static ShapeData makeObject(std::istream* in);
	static ShapeData makePlane(int dimensions);
	ShapeGenerator();
	~ShapeGenerator();
};

