#pragma once
#include <vector>
class textbox
{
public:
	size_t len;
	GLsizei width;
	std::vector<int> shapeIdx;
	std::vector<float> cursor;
	textbox(const char* str, GLsizei windowWidth, std::vector<int> fontWidths);
	~textbox();
};
