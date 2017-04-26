#pragma once
#include "pch.h"
#include "Region.h"
#include "Engine.h"
#include <png.h>
#include <pngpriv.h>
#include "ShapeGenerator.h"
#include "Dialogue.h"



class Region2
{
public:
	std::vector<int> csvWidths;
	const GLuint numberOfObj = 8;
	const GLuint numberOfGUI = 4;
	const GLuint numberOfChars = 257;
	const GLuint numberOfShapes = numberOfObj + numberOfGUI + numberOfChars;
	void setModelToWorlds(struct engine* engine);
	Region load_dialog();
	std::vector<ShapeData> shape_loader(struct engine* engine, std::vector<int>& csvWidths);
	ShapeData load_geometry(struct engine* engine, const char* filename);
	void load_Tex(struct engine* engine);
	void load_texture(struct engine* engine, unsigned char* pdata, GLuint j, bool alpha, off_t fileLength);
	std::vector<float> readCSV(GLuint i, std::vector<int>& csvWidths);
	static void png_asset_read(png_structp png, png_bytep data, png_size_t size);
	Region2();
	~Region2();
};
