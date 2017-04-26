
#include "Region2.h"
#include <iostream>     // std::cout
#include <fstream> 
#include <istream>
#include <sstream>
#include <iterator>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <tiny_obj_loader.h>
#include <vector>

using namespace glm;
extern AAsset* asset;

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "astroGame.NativeActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "astroGame.NativeActivity", __VA_ARGS__))

Region Region2::load_dialog()
{
	Region region;

	region.dialog1.str1 = "Hallo!";
	region.dialog1.str2 = "1) Wo ist die Stadt hin? Was ist das für eine Ruine da?";
	region.dialog1.str3 = "2) Ich muss mein Schiff reparieren. Kann ich hier irgendwo Ersatzteile kaufen?";

	region.dialog2.str1 = "Ich suche einen roten Pilz. Bring ihn mir, dann helfe ich dir weiter.";
	region.dialog2.str2 = "Ok, ich werde mich auf die Suche machen.";
	region.dialog2.str3 = "";

	region.dialog3.str1 = "Pilz aufgenommen.";
	region.dialog3.str2 = "";
	region.dialog3.str3 = "";

	region.dialog4.str1 = "Vielen Dank für den Pilz. Die Stadt findest du, wenn du durch das Portal im Norden gehst.";
	region.dialog4.str2 = "Ok, ich werde mich auf die Suche machen.";
	region.dialog4.str3 = "";

	region.dialog5.str1 = "An der Seite des Portals ist ein drehbares Rad zu erkennen...";
	region.dialog5.str2 = "Drehe dein Handy um 90°, um zum nächsten Gebiet zu kommen!";
	region.dialog5.str3 = "";

	region.location1 = vec3(0.0, 0.0, 0.0);
	region.location2 = vec3(0.0, 0.0, -15.0);
	region.location3 = vec3(0.0, 0.0, 15.0);

	return region;
}

void Region2::setModelToWorlds(struct engine* engine)
{
	engine->renderer->modelToWorlds.resize(numberOfObj);
	engine->renderer->modelToWorlds[0] = translate(mat4(), vec3(0.0, 20.0, 0.0))* scale(mat4(), vec3(60.0f, 60.0f, 60.0f));
	engine->renderer->modelToWorlds[1] = translate(mat4(), vec3(20.0, -0.15, 15.0))*rotate(mat4(), 1.5f, vec3(0.0, 1.0, 0.0));
	engine->renderer->modelToWorlds[2] = scale(mat4(), vec3(20.0f, 20.0f, 20.0f));
	engine->renderer->modelToWorlds[3] = glm::rotate(mat4(), 3.14f, vec3(0.0, 1.0, 0.0));
	engine->renderer->modelToWorlds[4] = translate(mat4(), vec3(0.0, 0.0, -15.0))*scale(mat4(), vec3(3.0f, 3.0f, 3.0f));
	engine->renderer->modelToWorlds[5] = translate(mat4(), vec3(0.0, 0.0, 30.0))*glm::rotate(mat4(), 3.14f, vec3(0.0, 1.0, 0.0));
	engine->renderer->numberOfObj = numberOfObj;
}

std::vector<ShapeData> Region2::shape_loader(struct engine* engine, std::vector<int>& csvWidths)
{
	std::vector<ShapeData> shapeExternVec;
	shapeExternVec.resize(numberOfShapes);

	float wf = (float)engine->width;
	float hf = (float)engine->height;

	shapeExternVec[0] = load_geometry(engine, "skybox.txt");

	shapeExternVec[1] = load_geometry(engine, "ruinnew.txt");

	shapeExternVec[2] = ShapeGenerator::makePlane(5);

	shapeExternVec[3] = load_geometry(engine, "mundus.txt");

	shapeExternVec[4] = load_geometry(engine, "mush.txt");

	shapeExternVec[5] = load_geometry(engine, "portal.txt");

	shapeExternVec[6] = ShapeGenerator::makeTriangle();

	shapeExternVec[7] = ShapeGenerator::makeBox(vec2(-1.0f, -1.0f), 2.0f, 2.0f);
	//GUI elements and chars:
	shapeExternVec[numberOfObj] = ShapeGenerator::makeBox(vec2(-1.0f, -0.7f), 0.3f*hf / wf, 0.3f); //change answer button

	shapeExternVec[numberOfObj + 1] = ShapeGenerator::makeBox(vec2(-1.0f, -1.0f), 0.3f*hf / wf, 0.3f); //select answer button

	shapeExternVec[numberOfObj + 2] = ShapeGenerator::makeBox(vec2(0.8f, -0.7f), 0.3f*hf / wf, 0.3f); //gun button

	shapeExternVec[numberOfObj + 3] = ShapeGenerator::makeBox(vec2(0.8f, -1.0f), 0.3f*hf / wf, 0.3f); //hand button

	for (GLuint i = 0; i < 256; i++) {
		std::vector<float> texCoord = readCSV(i, csvWidths);
		shapeExternVec[numberOfObj + numberOfGUI + i] = ShapeGenerator::makeQuad((float)csvWidths[i] * 3.0f / (float)engine->width, texCoord);
	}

	shapeExternVec[numberOfShapes - 1] = ShapeGenerator::makeBox(vec2(-0.8f, -1.0f), 1.6f, 0.6f); //textbox

	return shapeExternVec;
}

void Region2::load_Tex(struct engine* engine) {
	AAssetManager* manager = engine->app->activity->assetManager;

	GLuint numberOfTextures = numberOfObj + 9;
	engine->renderer->textureVec.resize(numberOfTextures);

	asset = AAssetManager_open(manager, "skybox.png", AASSET_MODE_STREAMING);
	off_t fileLength = AAsset_getLength(asset);
	unsigned char* pdata = (unsigned char*)(AAsset_getBuffer(asset));
	load_texture(engine, pdata, 0, false, fileLength);
	AAsset_close(asset);

	asset = AAssetManager_open(manager, "ruin.png", AASSET_MODE_STREAMING);
	fileLength = AAsset_getLength(asset);
	pdata = (unsigned char*)(AAsset_getBuffer(asset));
	load_texture(engine, pdata, 1, false, fileLength);
	AAsset_close(asset);

	asset = AAssetManager_open(manager, "grass.png", AASSET_MODE_STREAMING);
	fileLength = AAsset_getLength(asset);
	pdata = (unsigned char*)(AAsset_getBuffer(asset));
	load_texture(engine, pdata, 2, false, fileLength);
	AAsset_close(asset);

	asset = AAssetManager_open(manager, "porcher.png", AASSET_MODE_STREAMING);
	fileLength = AAsset_getLength(asset);
	pdata = (unsigned char*)(AAsset_getBuffer(asset));
	load_texture(engine, pdata, 3, false, fileLength);
	AAsset_close(asset);

	asset = AAssetManager_open(manager, "mushroom.png", AASSET_MODE_STREAMING);
	fileLength = AAsset_getLength(asset);
	pdata = (unsigned char*)(AAsset_getBuffer(asset));
	load_texture(engine, pdata, 4, false, fileLength);
	AAsset_close(asset);

	asset = AAssetManager_open(manager, "portal.png", AASSET_MODE_STREAMING);
	fileLength = AAsset_getLength(asset);
	pdata = (unsigned char*)(AAsset_getBuffer(asset));
	load_texture(engine, pdata, 5, false, fileLength);
	AAsset_close(asset);

	asset = AAssetManager_open(manager, "fire2.png", AASSET_MODE_STREAMING);
	fileLength = AAsset_getLength(asset);
	pdata = (unsigned char*)(AAsset_getBuffer(asset));
	load_texture(engine, pdata, 6, false, fileLength);
	AAsset_close(asset);

	asset = AAssetManager_open(manager, "AstronautBild.png", AASSET_MODE_STREAMING);
	fileLength = AAsset_getLength(asset);
	pdata = (unsigned char*)(AAsset_getBuffer(asset));
	load_texture(engine, pdata, numberOfObj + 1, false, fileLength);
	AAsset_close(asset);

	asset = AAssetManager_open(manager, "FontAtlas5.png", AASSET_MODE_STREAMING);
	fileLength = AAsset_getLength(asset);
	pdata = (unsigned char*)(AAsset_getBuffer(asset));
	load_texture(engine, pdata, numberOfObj + 2, false, fileLength);
	AAsset_close(asset);

	asset = AAssetManager_open(manager, "FontAtlas6.png", AASSET_MODE_STREAMING);
	fileLength = AAsset_getLength(asset);
	pdata = (unsigned char*)(AAsset_getBuffer(asset));
	load_texture(engine, pdata, numberOfObj + 3, false, fileLength);
	AAsset_close(asset);

	asset = AAssetManager_open(manager, "FontAtlas7.png", AASSET_MODE_STREAMING);
	fileLength = AAsset_getLength(asset);
	pdata = (unsigned char*)(AAsset_getBuffer(asset));
	load_texture(engine, pdata, numberOfObj + 4, false, fileLength);
	AAsset_close(asset);

	asset = AAssetManager_open(manager, "black.png", AASSET_MODE_STREAMING);
	fileLength = AAsset_getLength(asset);
	pdata = (unsigned char*)(AAsset_getBuffer(asset));
	load_texture(engine, pdata, numberOfObj + 5, false, fileLength);
	AAsset_close(asset);

	asset = AAssetManager_open(manager, "arrow2.png", AASSET_MODE_STREAMING);
	fileLength = AAsset_getLength(asset);
	pdata = (unsigned char*)(AAsset_getBuffer(asset));
	load_texture(engine, pdata, numberOfObj + 6, true, fileLength);
	AAsset_close(asset);

	asset = AAssetManager_open(manager, "signSelect.png", AASSET_MODE_STREAMING);
	fileLength = AAsset_getLength(asset);
	pdata = (unsigned char*)(AAsset_getBuffer(asset));
	load_texture(engine, pdata, numberOfObj + 7, true, fileLength);
	AAsset_close(asset);

	asset = AAssetManager_open(manager, "gun.png", AASSET_MODE_STREAMING);
	fileLength = AAsset_getLength(asset);
	pdata = (unsigned char*)(AAsset_getBuffer(asset));
	load_texture(engine, pdata, numberOfObj + 8, true, fileLength);
	AAsset_close(asset);

	asset = AAssetManager_open(manager, "hand.png", AASSET_MODE_STREAMING);
	fileLength = AAsset_getLength(asset);
	pdata = (unsigned char*)(AAsset_getBuffer(asset));
	load_texture(engine, pdata, numberOfObj + 9, true, fileLength);
	AAsset_close(asset);

}

Region2::Region2()
{

}

Region2::~Region2()
{
}

void Region2::png_asset_read(png_structp png, png_bytep data, png_size_t size) {
	AAsset_read(asset, data, size);
}

void Region2::load_texture(struct engine* engine, unsigned char* pdata, GLuint j, bool alpha, off_t fileLength) {

	unsigned char* pBuffer = new unsigned char[fileLength];

	off_t bufferLength = sizeof(pBuffer);

	memcpy(pBuffer, pdata, fileLength * sizeof(char));

	// new for loading png image data:
	int HEADER_SIZE = 8;
	png_byte* buffer = new png_byte[HEADER_SIZE];
	int numBytesRead = AAsset_read(asset, buffer, HEADER_SIZE);
	int numBytesRemaining = AAsset_getRemainingLength(asset);

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		LOGI("Unable to create PNG structure: %s");
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		LOGI("Unable to create png info : %s");
	}

	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		LOGI("Unable to create png end info : %s");
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		LOGI("Error during setjmp : %s");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	}
	png_set_read_fn(png_ptr, NULL, png_asset_read);
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);

	int bit_depth, color_type;
	png_uint_32 twidth, theight;
	png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
		NULL, NULL, NULL);
	LOGI("Width: %d, height: %d.", twidth, theight);

	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	LOGI("Row size: %d bytes.", rowbytes);

	// Allocate the image_data as a big block, to be given to opengl
	png_byte *image_data = new png_byte[rowbytes * theight];
	if (!image_data) {
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		LOGI("Unable to allocate image_data while loading %s ");
	}

	//row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep *row_pointers = new png_bytep[theight];
	if (!row_pointers) {
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		delete[] image_data;
		LOGI("Unable to allocate row_pointer while loading %s ");
	}
	// set the individual row_pointers to point at the correct offsets of image_data
	for (int i = 0; i < theight; ++i)
		row_pointers[theight - 1 - i] = image_data + i * rowbytes;

	//read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

	for (int i = 0; i < 10; i++) {
		LOGI("Pixel %d: %d %d %d %d", i, image_data[i * 4], image_data[i * 4 + 1],
			image_data[i * 4 + 2], image_data[i * 4 + 3]);
	}

	//Now generate the OpenGL texture object
	/*	Image* image = new Image((unsigned char*)image_data, twidth, theight,
	twidth * theight * 4, 0);*/

	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	delete[] row_pointers;

	//
	glEnable(GL_TEXTURE_2D);

	if (!alpha) {
		engine->renderer->sendTextureToOpenGL((unsigned char*)image_data, engine->renderer->textureVec[j], twidth, theight);
	}
	else {
		engine->renderer->sendTextureWithAlpha((unsigned char*)image_data, engine->renderer->textureVec[j], twidth, theight);
	}
}

std::vector<float> Region2::readCSV(GLuint i, std::vector<int>& csvWidths) {

	//cout << csvWidths[i] << endl;
	std::vector<float> TexCoord(8);
	float height = 0.0625f;
	float width = (float)csvWidths[i] / 512.0f;
	TexCoord[0] = (float)(i % 16)*0.0625f;
	TexCoord[1] = 1.0f - (float)floor(i / 16)*0.0625f - height;
	TexCoord[2] = (float)(i % 16)*0.0625f;
	TexCoord[3] = 1.0f - (float)floor(i / 16)*0.0625f;
	TexCoord[4] = (float)(i % 16)*0.0625f + width;
	TexCoord[5] = 1.0f - (float)floor(i / 16)*0.0625f;
	TexCoord[6] = (float)(i % 16)*0.0625f + width;
	TexCoord[7] = 1.0f - (float)floor(i / 16)*0.0625f - height;

	return TexCoord;
}

ShapeData Region2::load_geometry(struct engine* engine, const char* filename) {
	AAssetManager* manager = engine->app->activity->assetManager;
	asset = AAssetManager_open(manager, filename, AASSET_MODE_STREAMING);
	unsigned char* pdata = (unsigned char*)(AAsset_getBuffer(asset));

	std::istringstream in((char*)pdata);

	ShapeData shape = ShapeGenerator::makeObject(&in);

	AAsset_close(asset);

	return shape;
}