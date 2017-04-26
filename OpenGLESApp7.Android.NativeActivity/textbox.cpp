#include "pch.h"
#include "textbox.h"
#include <iostream>
#include <string>
#include <vector>
#include "../OpenGLESApp7.Shared/SimpleRenderer.h"
using namespace std;

textbox::textbox(const char* str, GLsizei windowWidth, std::vector<int> fontWidths)
{
	width = windowWidth;
	//convert text string to vector of character integers b:
	len = strlen(str); //len = anzahl buchstaben inkl. Leerzeichen
					   //read in width of b[i]-th character from csv:

					   //float width = 0.0625f;
	float curs = 0;
	shapeIdx.resize(len);
	cursor.resize(len);
	
	for (int i = 0; i < len; i++) {
		shapeIdx[i] = (int)str[i];
		cursor[i] = curs;
		curs = cursor[i] + (float)fontWidths[shapeIdx[i]] * 3.0f / (float)width;
		//std::cout << shapeIdx[i] << std::endl;
	}

	//then render ShapeDataVec[7+ shapeIdx[i]] in drawing()

	//calculate 4 vec2 with texcoords for every character:

	//calculate 4 vec2 with vertex-positions for every character (size of quad depends on width of character):


}

textbox::~textbox()
{
}
