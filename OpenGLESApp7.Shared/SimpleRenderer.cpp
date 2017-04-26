//
// This file is used by the template to render a basic scene using GL.
//
#include "pch.h"

#include "SimpleRenderer.h"
#include "../OpenGLESApp7.Android.NativeActivity/include/MathHelper.h"
#include "../OpenGLESApp7.Android.NativeActivity/ShapeGenerator.h"
#include "../OpenGLESApp7.Android.NativeActivity/ShaderFile.h"
#include "../OpenGLESApp7.Android.NativeActivity/VertexAnimate.h"

#include "../OpenGLESApp7.Android.NativeActivity/BoneRotation.h"
#include "../OpenGLESApp7.Android.NativeActivity/GenerateWalk.h"
#include "../OpenGLESApp7.Android.NativeActivity/textBox.h"
#include "../OpenGLESApp7.Android.NativeActivity/Game.h"
// These are used by the shader compilation methods.
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstdlib>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "astroGame.NativeActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "astroGame.NativeActivity", __VA_ARGS__))
using namespace glm;
GLuint VERTEX_BYTE_SIZE = 8*sizeof(float);

BoneRotation walk[1];

extern Game game;

GLuint CompileShader(GLenum type, const std::string &source)
{
    GLuint shader = glCreateShader(type);

    const char *sourceArray[1] = { source.c_str() };
    glShaderSource(shader, 1, sourceArray, NULL);
    glCompileShader(shader);

    GLint compileResult;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

    if (compileResult == 0)
    {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        std::vector<GLchar> infoLog(infoLogLength);
        glGetShaderInfoLog(shader, (GLsizei)infoLog.size(), NULL, infoLog.data());

        std::string errorMessage = std::string("Shader compilation failed: ");
        errorMessage += std::string(infoLog.begin(), infoLog.end()); 

        throw std::runtime_error(errorMessage.c_str());
    }

    return shader;
}

GLuint CompileProgram(const std::string &vsSource, const std::string &fsSource)
{
    GLuint program = glCreateProgram();

    if (program == 0)
    {
        throw std::runtime_error("Program creation failed");
    }

    GLuint vs = CompileShader(GL_VERTEX_SHADER, vsSource);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fsSource);

    if (vs == 0 || fs == 0)
    {
        glDeleteShader(fs);
        glDeleteShader(vs);
        glDeleteProgram(program);
        return 0;
    }

    glAttachShader(program, vs);
    glDeleteShader(vs);

    glAttachShader(program, fs);
    glDeleteShader(fs);

    glLinkProgram(program);

    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

    if (linkStatus == 0)
    {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        std::vector<GLchar> infoLog(infoLogLength);
        glGetProgramInfoLog(program, (GLsizei)infoLog.size(), NULL, infoLog.data());

        std::string errorMessage = std::string("Program link failed: ");
        errorMessage += std::string(infoLog.begin(), infoLog.end()); 

        throw std::runtime_error(errorMessage.c_str());
    }

    return program;
}

void SimpleRenderer::renderGUI()
{
	glUseProgram(textProgram);

	GLint cursorxUniformLocation = glGetUniformLocation(textProgram, "cursorX");
	GLint cursoryUniformLocation = glGetUniformLocation(textProgram, "cursorY");

	float cursorx;
	float cursory;

	//arrow:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureVec[numberOfObj + 6]);
	glUniform1i(glGetUniformLocation(textProgram, "ourTexture1"), 0);

	cursory = 0.0f;
	glUniform1fv(cursoryUniformLocation, 1, &cursory);
	cursorx = 0.0f;
	glUniform1fv(cursorxUniformLocation, 1, &cursorx);
	glBindVertexArray(VAOIdVec[numberOfObj]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[numberOfObj], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[numberOfObj]));
	//select button:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureVec[numberOfObj + 7]);
	glUniform1i(glGetUniformLocation(textProgram, "ourTexture1"), 0);

	glUniform1fv(cursoryUniformLocation, 1, &cursory);
	glUniform1fv(cursorxUniformLocation, 1, &cursorx);
	glBindVertexArray(VAOIdVec[numberOfObj +1]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[numberOfObj +1], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[numberOfObj +1]));
	//gun:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureVec[numberOfObj + 8]);
	glUniform1i(glGetUniformLocation(textProgram, "ourTexture1"), 0);

	glUniform1fv(cursoryUniformLocation, 1, &cursory);
	glUniform1fv(cursorxUniformLocation, 1, &cursorx);
	glBindVertexArray(VAOIdVec[numberOfObj +2]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[numberOfObj +2], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[numberOfObj +2]));
	//hand:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureVec[numberOfObj + 9]);
	glUniform1i(glGetUniformLocation(textProgram, "ourTexture1"), 0);

	glUniform1fv(cursoryUniformLocation, 1, &cursory);
	glUniform1fv(cursorxUniformLocation, 1, &cursorx);
	glBindVertexArray(VAOIdVec[numberOfObj +3]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[numberOfObj +3], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[numberOfObj+3]));

	glUseProgram(mProgram);
}

void SimpleRenderer::renderText(const char* str1, const char* str2, const char* str3) {
	glUseProgram(textProgram);

	textbox* text = new textbox(str1, mWindowWidth, fontWidths);

	GLuint numberofObjects = ShapeDataVec.size() - 257;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureVec[numberOfObj + 2]);
	glUniform1i(glGetUniformLocation(textProgram, "ourTexture1"), 0);
	GLint cursorxUniformLocation = glGetUniformLocation(textProgram, "cursorX");
	GLint cursoryUniformLocation = glGetUniformLocation(textProgram, "cursorY");
	int index;
	float cursorx;
	float cursory = 0.9f;
	glUniform1fv(cursoryUniformLocation, 1, &cursory);
	for (int i = 0; i < text->len; i++) {
		index = numberofObjects + text->shapeIdx[i];
		cursorx = text->cursor[i];
		glUniform1fv(cursorxUniformLocation, 1, &cursorx);
		glBindVertexArray(VAOIdVec[index]);
		glDrawElements(GL_TRIANGLES, numIndicesVec[index], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[index]));
	}

	if (!answer2) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureVec[numberOfObj + 3]);
		glUniform1i(glGetUniformLocation(textProgram, "ourTexture1"), 0);
	}
	else {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureVec[numberOfObj + 4]);
		glUniform1i(glGetUniformLocation(textProgram, "ourTexture1"), 0);
	}

	textbox* textb = new textbox(str2, mWindowWidth, fontWidths);
	cursory = 1.3f;
	glUniform1fv(cursoryUniformLocation, 1, &cursory);
	for (int i = 0; i < textb->len; i++) {
		index = numberofObjects + textb->shapeIdx[i];
		cursorx = textb->cursor[i];
		glUniform1fv(cursorxUniformLocation, 1, &cursorx);
		glBindVertexArray(VAOIdVec[index]);
		glDrawElements(GL_TRIANGLES, numIndicesVec[index], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[index]));
	}

	if (!answer2) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureVec[numberOfObj + 4]);
		glUniform1i(glGetUniformLocation(textProgram, "ourTexture1"), 0);
	}
	else {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureVec[numberOfObj + 3]);
		glUniform1i(glGetUniformLocation(textProgram, "ourTexture1"), 0);
	}

	textbox* textc = new textbox(str3, mWindowWidth, fontWidths);
	cursory = 1.39f;
	glUniform1fv(cursoryUniformLocation, 1, &cursory);
	for (int i = 0; i < textc->len; i++) {
		index = numberofObjects + textc->shapeIdx[i];
		cursorx = textc->cursor[i];
		glUniform1fv(cursorxUniformLocation, 1, &cursorx);
		glBindVertexArray(VAOIdVec[index]);
		glDrawElements(GL_TRIANGLES, numIndicesVec[index], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[index]));
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureVec[numberOfObj + 5]);
	glUniform1i(glGetUniformLocation(textProgram, "ourTexture1"), 0);

	cursory = 0.0f;
	glUniform1fv(cursoryUniformLocation, 1, &cursory);
	index = numberofObjects + 256;
	cursorx = 0.0f;
	glUniform1fv(cursorxUniformLocation, 1, &cursorx);
	glBindVertexArray(VAOIdVec[index]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[index], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[index]));

	glUseProgram(mProgram);
}

SimpleRenderer::SimpleRenderer(std::vector<ShapeData>& geom, ShapeAnim& playerShape) :
    mWindowWidth(0),
    mWindowHeight(0),
    mDrawCount(0)
{   
	camera = new Camera();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	//explosion only texture:
	//setupTexture();

	//set bone rotation matrices:
	walk = new GenerateWalk;

	//set geometries:
	ShapeDataVec.resize(geom.size());
	for (uint i = 0; i < geom.size(); i++) {
		ShapeDataVec[i] = geom[i];
	}
	
	VAOIdVec.resize(ShapeDataVec.size());
	IndexDataByteOffsetVec.resize(ShapeDataVec.size());
	numIndicesVec.resize(ShapeDataVec.size());

	GLuint ByteOffset = 0;

	GLsizeiptr ShapeDataTotalSize = 0;
	for (GLuint i = 0; i < ShapeDataVec.size(); i++) {
		ShapeDataTotalSize += ShapeDataVec[i].vertexBufferSize() + ShapeDataVec[i].indexBufferSize();
	}
    
	glGenBuffers(1, &theBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glBufferData(GL_ARRAY_BUFFER, ShapeDataTotalSize, 0, GL_DYNAMIC_DRAW);			//all objects
	GLsizeiptr currentOffset = 0;
	for (GLuint i = 0; i < ShapeDataVec.size(); i++) {
		glBufferSubData(GL_ARRAY_BUFFER, currentOffset, ShapeDataVec[i].vertexBufferSize(), ShapeDataVec[i].vertices); //tank0
		currentOffset += ShapeDataVec[i].vertexBufferSize();
		glBufferSubData(GL_ARRAY_BUFFER, currentOffset, ShapeDataVec[i].indexBufferSize(), ShapeDataVec[i].indices);
		currentOffset += ShapeDataVec[i].indexBufferSize();

		numIndicesVec[i] = ShapeDataVec[i].numindices;

		glGenVertexArrays(1, &VAOIdVec[i]); //tank0
		glBindVertexArray(VAOIdVec[i]);
		glEnableVertexAttribArray(0); // 0 steht für die koordinaten der vertices: "location=0" im shadercode
		glEnableVertexAttribArray(1); //  1 steht für die normalen der vertices: "location=1" im shadercode
		glEnableVertexAttribArray(2); // 2 steht für textur der vertices
		//glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(ByteOffset + 0)); // letztes argument x: koordinaten starten nach x bytes
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(ByteOffset + sizeof(float) * 3)); // letztes argument y: farben starten nach y bytes
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(ByteOffset + sizeof(float) * 6));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

		IndexDataByteOffsetVec[i] = ByteOffset + ShapeDataVec[i].vertexBufferSize();
		//all objects before:
		ByteOffset += ShapeDataVec[i].vertexBufferSize() + ShapeDataVec[i].indexBufferSize();
	}
	

	GLsizeiptr ShapSize = 0;
	ShapSize = playerShape.vertexBufferSize() + playerShape.indexBufferSize();
	//bind to new animate buffer:
	glGenBuffers(1, &newBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, newBufferID);
	glBufferData(GL_ARRAY_BUFFER, ShapSize, 0, GL_DYNAMIC_DRAW);

    currentOffset = 0;

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, playerShape.vertexBufferSize(), playerShape.vertices); //tank0
	currentOffset += playerShape.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, playerShape.indexBufferSize(), playerShape.indices);
	currentOffset += playerShape.indexBufferSize();

	glGenVertexArrays(1, &VAOanim); //tank0
	glBindVertexArray(VAOanim);
	glEnableVertexAttribArray(0); // 0 steht für die koordinaten der vertices: "location=0" im shadercode
	glEnableVertexAttribArray(1); //  1 steht für die normalen der vertices: "location=1" im shadercode
	glEnableVertexAttribArray(2); // 2 steht für textur der vertices
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);//glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float) , (void*)(0)); // letztes argument x: koordinaten starten nach x bytes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float) , (void*)(sizeof(float) * 3)); // letztes argument y: farben starten nach y bytes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float) , (void*)(sizeof(float) * 6));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float) , (void*)(sizeof(float) * 8));
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float) , (void*)(sizeof(float) * 11));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newBufferID);

	IndexDataByteOffset = playerShape.vertexBufferSize();
	animIndices = playerShape.numindices;

	// Vertex Shader source
	const std::string vs = ShaderFile::vShader();
	const std::string vsAnim = ShaderFile::vShaderAnim();
	const std::string vsText = ShaderFile::vShaderText();
	const std::string vsBlur = ShaderFile::vShaderBlur();
    // Fragment Shader source
	const std::string fs = ShaderFile::fShader();
	const std::string fsAnim = ShaderFile::fShaderAnim();
	const std::string fsText = ShaderFile::fShaderText();
	const std::string fsBlur = ShaderFile::fShaderBlur();

    // Set up the shader and its uniform/attribute locations.
    mProgram = CompileProgram(vs, fs);
    mModelUniformLocation = glGetUniformLocation(mProgram, "uModelMatrix");
    mViewUniformLocation = glGetUniformLocation(mProgram, "uViewMatrix");
    mProjUniformLocation = glGetUniformLocation(mProgram, "uProjMatrix");
	lightPositionUniformLocation = glGetUniformLocation(mProgram, "lightPosition");
	eyePositionUniformLocation = glGetUniformLocation(mProgram, "eyePosition");
	ambientUniformLocation = glGetUniformLocation(mProgram, "ambient");
	texShiftUniformLocation = glGetUniformLocation(mProgram, "texShift");
	fogColorUniformLocation = glGetUniformLocation(mProgram, "fogColor");

	animProgram =  CompileProgram(vsAnim, fsAnim);
	animModelUniformLocation = glGetUniformLocation(animProgram, "uModelMatrix");
	animViewUniformLocation = glGetUniformLocation(animProgram, "uViewMatrix");
	animProjUniformLocation = glGetUniformLocation(animProgram, "uProjMatrix");
	animlightPositionUniformLocation = glGetUniformLocation(animProgram, "lightPosition");
	animEyePositionUniformLocation = glGetUniformLocation(animProgram, "eyePosition");
	animAmbientUniformLocation = glGetUniformLocation(animProgram, "ambient");

	textProgram = CompileProgram(vsText, fsText);

	blurProgram = CompileProgram(vsBlur, fsBlur);
}

SimpleRenderer::~SimpleRenderer()
{

    if (mProgram != 0)
    {
        glDeleteProgram(mProgram);
        mProgram = 0;
    }

    if (mVertexPositionBuffer != 0)
    {
        glDeleteBuffers(1, &mVertexPositionBuffer);
        mVertexPositionBuffer = 0;
    }

    if (mVertexColorBuffer != 0)
    {
        glDeleteBuffers(1, &mVertexColorBuffer);
        mVertexColorBuffer = 0;
    }

	if (mTextureCoordsBuffer != 0)
	{
		glDeleteBuffers(1, &mTextureCoordsBuffer);
		mTextureCoordsBuffer = 0;
	}

    if (mIndexBuffer != 0)
    {
        glDeleteBuffers(1, &mIndexBuffer);
        mIndexBuffer = 0;
    }
	
}

void SimpleRenderer::Draw(MathHelper::Matrix4& projectionMatrix, glm::mat4& worldToViewMatrix)
{
	pos = camera->playerPosition;
	camera->position = camera->playerPosition - 10.0f*camera->viewDirection + glm::vec3(0.0, 5.0, 0.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mProgram == 0)
        return;

    glUseProgram(mProgram);

	//glActiveTexture(GL_TEXTURE0 + 1);
	//glBindTexture(GL_TEXTURE_2D, textureVec[1]);
	//glUniform1i(glGetUniformLocation(mProgram, "normalMap"), 1);
	
	glm::vec3 lightPosition = vec3(0.0, 3.3, -15.5);
	glUniform3fv(lightPositionUniformLocation, 1, &(lightPosition[0]));

	glm::vec3 eyePosition = camera->position;
	glUniform3fv(eyePositionUniformLocation, 1, &(eyePosition[0]));
	
	fogColor = vec3(0.01,0.01,0.01);
	glUniform3fv(fogColorUniformLocation, 1, &(fogColor[0]));

	float ambient = 1.5f;
	glUniform1fv(ambientUniformLocation, 1, &ambient);

	float texShift = 0.0f;
	glUniform1fv(texShiftUniformLocation, 1, &texShift);

	//draw skybox:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureVec[0]);
	glUniform1i(glGetUniformLocation(mProgram, "OurTexture"), 0);

	mat4 modelToWorldMatrix = translate(mat4(), camera->position)*modelToWorlds[0];
	glUniformMatrix4fv(mModelUniformLocation, 1, GL_FALSE, &(modelToWorldMatrix[0][0]));

	glBindVertexArray(VAOIdVec[0]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[0], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[0]));

	ambient = 0.4f;
	glUniform1fv(ambientUniformLocation, 1, &ambient);
	//draw shuttle:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureVec[1]);
	glUniform1i(glGetUniformLocation(mProgram, "OurTexture"), 0);

	modelToWorldMatrix = modelToWorlds[1];
	glUniformMatrix4fv(mModelUniformLocation, 1, GL_FALSE, &(modelToWorldMatrix[0][0]));

	glBindVertexArray(VAOIdVec[1]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[1], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[1]));

	//draw ground:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureVec[2]);
	glUniform1i(glGetUniformLocation(mProgram, "OurTexture"), 0);

	modelToWorldMatrix = modelToWorlds[2];
	glUniformMatrix4fv(mModelUniformLocation, 1, GL_FALSE, &(modelToWorldMatrix[0][0]));

	glBindVertexArray(VAOIdVec[2]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[2], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[2]));

	//draw mundus:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureVec[3]);
	glUniform1i(glGetUniformLocation(mProgram, "OurTexture"), 0);

	modelToWorldMatrix = modelToWorlds[3];
	glUniformMatrix4fv(mModelUniformLocation, 1, GL_FALSE, &(modelToWorldMatrix[0][0]));

	glBindVertexArray(VAOIdVec[3]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[3], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[3]));

	//crystal:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureVec[4]);
	glUniform1i(glGetUniformLocation(mProgram, "OurTexture"), 0);

	modelToWorldMatrix = modelToWorlds[4];
	glUniformMatrix4fv(mModelUniformLocation, 1, GL_FALSE, &(modelToWorldMatrix[0][0]));

	glBindVertexArray(VAOIdVec[4]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[4], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[4]));

	//portal:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureVec[5]);
	glUniform1i(glGetUniformLocation(mProgram, "OurTexture"), 0);

	modelToWorldMatrix = modelToWorlds[5];
	glUniformMatrix4fv(mModelUniformLocation, 1, GL_FALSE, &(modelToWorldMatrix[0][0]));

	glBindVertexArray(VAOIdVec[5]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[5], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[5]));

	//draw astronaut:
	glUseProgram(animProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureVec[numberOfObj +1]);
	glUniform1i(glGetUniformLocation(animProgram, "OurTexture"), 0);

	//load bone rotation matrices:
	mat4 mBones[14];
	for (int i = 0; i < 14; i++) {
		mBones[i] = walk->step[step % 8].boneRot[i];
		//mBones[i] = translate(mat4(), vec3(0.0, -1.0, 0.0));
	}

	GLint rotMatrixUniformLocation = glGetUniformLocation(animProgram, "mBones[0]");
	//mat4 RotMatrix = rotate(mat4(), 1.5f, vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(rotMatrixUniformLocation, 14, GL_FALSE, &(mBones[0][0][0]));

	modelToWorldMatrix = translate(mat4(), camera->playerPosition)*glm::rotate(mat4(), camera->anglegun, vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(animModelUniformLocation, 1, GL_FALSE, &(modelToWorldMatrix[0][0]));
	glUniformMatrix4fv(animViewUniformLocation, 1, GL_FALSE, &(worldToViewMatrix[0][0]));
	glUniformMatrix4fv(animProjUniformLocation, 1, GL_FALSE, &(projectionMatrix.m[0][0]));
	glUniform3fv(animlightPositionUniformLocation, 1, &(lightPosition[0]));
	glUniform3fv(animEyePositionUniformLocation, 1, &(eyePosition[0]));
	glUniform1fv(animAmbientUniformLocation, 1, &ambient);

	glBindVertexArray(VAOanim);
	glDrawElements(GL_TRIANGLES, animIndices, GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffset));

	if (length(camera->playerPosition-location1) < 2.0) {
		renderText(dialog.str1, dialog.str2, dialog.str3);
	}

	if ((length(camera->playerPosition - location2) < 2.0) && game.knowsCrystal) {
		renderText(dialog.str1, dialog.str2, dialog.str3);
	}

	if (length(camera->playerPosition - location3) < 2.0 && game.knowsPortal) {
		renderText(dialog.str1, dialog.str2, dialog.str3);
	}

	//collision detection with cube bounding box for enemy:
	glm::mat4 cubeModelToWorldMatrix = scale(mat4(), vec3(1.0,4.0,1.0));
	ShapeData cube = ShapeGenerator::makeCube();
	bomb = Collision::test(cube, cubeModelToWorldMatrix, bulletVec, camera->playerPosition);
	
	if (bomb.detonate) {
		//QSound sound = QSound::QSound("Bomb.wav");
		//sound.play();
		//renderText("peng", "peng", "peng");
		explosionTime = getTimeNsec();
		Explosion* explosion = new Explosion(bomb.position, explosionTime);
		explosionVec.push_back(explosion);
	}

	//bullet:
	glUseProgram(mProgram);
	//std::cout << "bulletVec.size = " << bulletVec.size() << std::endl;
	for (int i = 0; i < bulletVec.size(); i++) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureVec[numberOfObj + 5]);
		glUniform1i(glGetUniformLocation(mProgram, "ourTexture1"), 0);

		bulletVec[i]->move(getTimeNsec());

		modelToWorldMatrix = translate(mat4(), bulletVec[i]->position)*scale(mat4(),vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(mModelUniformLocation, 1, GL_FALSE, &(modelToWorldMatrix[0][0]));

		glBindVertexArray(VAOIdVec[2]);
		glDrawElements(GL_TRIANGLES, numIndicesVec[2], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[2]));
	}


	renderGUI();

	if (explosionVec.size() > 0) {
		explosionOnPlane();
	}

    mDrawCount += 1;
}

void SimpleRenderer::paintGL()
{
	glViewport(0, 0, mWindowWidth, mWindowHeight); // rescale shapeangle to fit window

	glUseProgram(mProgram);

	MathHelper::Matrix4 projectionMatrix = MathHelper::SimpleProjectionMatrix(float(mWindowWidth) / float(mWindowHeight));
	glUniformMatrix4fv(mProjUniformLocation, 1, GL_FALSE, &(projectionMatrix.m[0][0]));

	mat4 worldToViewMatrix = camera->getWorldToViewMatrix();
	glUniformMatrix4fv(mViewUniformLocation, 1, GL_FALSE, &(worldToViewMatrix[0][0]));

	//explosion texture generating (invisibly):
	glBindFramebuffer(GL_FRAMEBUFFER, fboIdExplosion);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // depth set to z=1 (=far away)
														//invisibly draw a scene to a texture directly:
	renderExplosion(bomb.position);
	// unbind FBO and switch back to showing window-system
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, textureIdExplosion);
	//glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	Draw(projectionMatrix, worldToViewMatrix);
}

void SimpleRenderer::mouseMoveEvent(int& x) {
	camera->mouseUpdate((float)x);

	paintGL();
}

void SimpleRenderer::runEvent(int & y, int & height)
{
	camera->keyUpdate(y, height);

	if (runCount % 10 == 0) {
		if (y > height / 2) {
			step += 1;
		}
		if (y < height / 2) {
			if (step > 0) {
				step -= 1;
			}
			else {
				step += 7;
			}
		}
	}

	runCount += 1;
	//paintGL();
}

void SimpleRenderer::UpdateWindowSize(GLsizei width, GLsizei height)
{
    glViewport(0, 0, width, height);
    mWindowWidth = width;
    mWindowHeight = height;
}

void SimpleRenderer::sendTextureToOpenGL(unsigned char* pdata, GLuint& textureId, unsigned int& width, unsigned int& height)
{
	//texture0:
	//int width, height;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	//load texture from file:
	//unsigned char* image = SOIL_load_image("tiles3.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pdata);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void SimpleRenderer::sendTextureWithAlpha(unsigned char * pdata, GLuint & textureId, unsigned int & width, unsigned int & height)
{
	//texture0:
	//int width, height;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//load texture from file:
	//unsigned char* image = SOIL_load_image("tiles3.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pdata);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

long SimpleRenderer::getTimeNsec() {
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return (long)now.tv_sec * 1000000000LL + now.tv_nsec;
}

void SimpleRenderer::renderExplosion(glm::vec3 position) {

	glUseProgram(mProgram);

	//explosion particles = triangles:
	mat4 modelToWorldMatrix;
	float particlePhi;
	
	LOGI("explosionVecSize=%i",explosionVec.size());

	std::vector<Particle*>::iterator it;
	//tri:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureVec[6]);
	glUniform1i(glGetUniformLocation(mProgram, "ourTexture1"), 0);

	glBindVertexArray(VAOIdVec[6]);
	float ambient = 0.5f;
	glUniform1fv(ambientUniformLocation, 1, &ambient);
	std::vector<Explosion*>::iterator jt;
	for (jt = explosionVec.begin(); jt != explosionVec.end();) {
		LOGI("elapsedTime=%i", (unsigned int)(getTimeNsec() - explosionTime)/1000000);
		LOGI("particleVecSize=%i", (*jt)->particleVec.size());
		//if ((*jt)->elapsedTime>18) {
		if (getTimeNsec()-explosionTime>1000000000) {
			delete (*jt);
			jt = explosionVec.erase(jt);
		}
		else {
			//spawn and move particles:
			(*jt)->spawn(getTimeNsec());
			//draw fire particles (lifetime=8):
			for (int j = 0; j < (*jt)->particleVec.size(); j++) {
				if ((*jt)->particleVec[j]->lifeTime < 8) {
					//transform and draw particle: 
					(*jt)->particleVec[j]->move(getTimeNsec());
					particlePhi = (float)(std::rand()) / (float)(RAND_MAX / 1.5f);
					modelToWorldMatrix = translate(mat4(), (*jt)->particleVec[j]->position)*rotate(mat4(), particlePhi, camera->viewDirection) * rotate(mat4(), camera->anglegun, vec3(0.0,1.0,0.0)) * scale(mat4(), vec3(0.5,0.5,0.5));// 
					
					float texShift = 0.1f*(*jt)->particleVec[j]->lifeTime;
					glUniform1fv(texShiftUniformLocation, 1, &texShift);
					glUniformMatrix4fv(mModelUniformLocation, 1, GL_FALSE, &(modelToWorldMatrix[0][0]));
					glDrawElements(GL_TRIANGLES, numIndicesVec[6], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[6]));
				}
			}
			++jt;
		}
	}

	float texShift = 0.0f;
	glUniform1fv(texShiftUniformLocation, 1, &texShift);
}

void SimpleRenderer::setupTexture() {
	// create a texture object
	glGenTextures(1, &textureIdExplosion);
	glBindTexture(GL_TEXTURE_2D, textureIdExplosion);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWindowWidth, mWindowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// create a framebuffer object
	glGenFramebuffers(1, &fboIdExplosion);
	glBindFramebuffer(GL_FRAMEBUFFER, fboIdExplosion);

	// attach the texture to FBO color attachment point
	glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER 
		GL_COLOR_ATTACHMENT0,  // 2. attachment point
		GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
		textureIdExplosion,             // 4. tex ID
		0);                    // 5. mipmap level: 0(base)


							 // check FBO status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		LOGI("fbo not used"); 
		LOGW("fbo not used");
	}

	// switch back to window-system-provided framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SimpleRenderer::explosionOnPlane()
{
	glUseProgram(blurProgram);

	//explosion:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureIdExplosion);
	glUniform1i(glGetUniformLocation(blurProgram, "ourTexture1"), 0);


	glBindVertexArray(VAOIdVec[7]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[7], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[7]));
}