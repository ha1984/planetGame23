//
// This file is used by the template to render a basic scene using GL.
//
#include "pch.h"

#include "SimpleRenderer.h"
#include "../OpenGLESApp7.Android.NativeActivity/include/MathHelper.h"
#include "../OpenGLESApp7.Android.NativeActivity/ShapeGenerator.h"
// These are used by the shader compilation methods.
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
using namespace glm;
GLuint VERTEX_BYTE_SIZE = 8*sizeof(float);

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

SimpleRenderer::SimpleRenderer(std::vector<ShapeData>& geom) :
    mWindowWidth(0),
    mWindowHeight(0),
    mDrawCount(0)
{
	for (uint i = 0; i < geom.size(); i++) {
		ShapeDataVec.push_back(geom[i]);
	}
	
	ShapeData data = ShapeGenerator::makePlane(5);
	ShapeDataVec.push_back(data);

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
		glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(ByteOffset + 0)); // letztes argument x: koordinaten starten nach x bytes
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(ByteOffset + sizeof(float) * 3)); // letztes argument y: farben starten nach y bytes
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(ByteOffset + sizeof(float) * 6));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

		IndexDataByteOffsetVec[i] = ByteOffset + ShapeDataVec[i].vertexBufferSize();
		//all objects before:
		ByteOffset += ShapeDataVec[i].vertexBufferSize() + ShapeDataVec[i].indexBufferSize();
	}
	
	// Vertex Shader source
	const std::string vs = R"(
        uniform mat4 uModelMatrix;
        uniform mat4 uViewMatrix;
        uniform mat4 uProjMatrix;
        attribute vec4 aPosition;
        attribute vec4 aColor;
		attribute vec2 aTexCoord;
		varying vec2 vTexCoord;
        varying vec4 vColor;
		varying vec3 positionWorld;
        varying vec3 normalWorld;
        void main()
        {
            gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * aPosition;
			vec4 normal = vec4(aColor.x, aColor.y, aColor.z, 0.0);
			normalWorld = normalize(vec3(uModelMatrix * normal)); //normal = normalModel
			positionWorld = vec3(uModelMatrix * aPosition); //aPositon = positionModel
            vColor = aColor;
			vTexCoord = aTexCoord;
        }
    )";

    // Fragment Shader source
	const std::string fs = R"(
        precision mediump float;
		uniform sampler2D OurTexture;
		uniform sampler2D normalMap;
        uniform vec3 lightPosition;
		uniform vec3 eyePosition;
		uniform float ambient;
        varying vec4 vColor;
		varying vec2 vTexCoord;
		varying vec3 positionWorld;
        varying vec3 normalWorld;
        void main()
        {
			vec4 normalColor = texture2D(normalMap, vTexCoord);
			//vec3 normal = vec3(normalColor.r*2.0-1.0,0.0,normalColor.g*2.0-1.0);

			vec3 r = lightPosition - positionWorld;
			vec3 lightVector = normalize(r);

			//specular:
			vec3 reflectedLightVector = -reflect(lightVector,normalize(normalWorld));
			vec3 eyeDirection = normalize(eyePosition - positionWorld);
			float product = dot(reflectedLightVector, eyeDirection);
			float s = clamp(product,0.0,1.0);
			float len = length(r);
			float ss = pow(s,20.0)/len;
			vec3 light = vec3(ss, ss, ss) + vec3(ambient,ambient,ambient);
			//
            //gl_FragColor = vColor;
            gl_FragColor = texture2D(OurTexture, vTexCoord) * vec4(light.x,light.y,light.z, 1.0);
        }
    )";

    // Set up the shader and its uniform/attribute locations.
    mProgram = CompileProgram(vs, fs);
    mPositionAttribLocation = glGetAttribLocation(mProgram, "aPosition");
    mColorAttribLocation = glGetAttribLocation(mProgram, "aColor");
	mTextureAttribLocation = glGetAttribLocation(mProgram, "aTexCoord");
    mModelUniformLocation = glGetUniformLocation(mProgram, "uModelMatrix");
    mViewUniformLocation = glGetUniformLocation(mProgram, "uViewMatrix");
    mProjUniformLocation = glGetUniformLocation(mProgram, "uProjMatrix");
	lightPositionUniformLocation = glGetUniformLocation(mProgram, "lightPosition");
	eyePositionUniformLocation = glGetUniformLocation(mProgram, "eyePosition");
	ambientUniformLocation = glGetUniformLocation(mProgram, "ambient");

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

void SimpleRenderer::Draw()
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mProgram == 0)
        return;

    glUseProgram(mProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureVec[0]);
	glUniform1i(glGetUniformLocation(mProgram, "OurTexture"), 0);

	//glActiveTexture(GL_TEXTURE0 + 1);
	//glBindTexture(GL_TEXTURE_2D, textureVec[1]);
	//glUniform1i(glGetUniformLocation(mProgram, "normalMap"), 1);
	
    MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrix((float)mDrawCount/50.0);
    glUniformMatrix4fv(mModelUniformLocation, 1, GL_FALSE, &(modelMatrix.m[0][0]));

    MathHelper::Matrix4 viewMatrix = MathHelper::SimpleViewMatrix();
    glUniformMatrix4fv(mViewUniformLocation, 1, GL_FALSE, &(viewMatrix.m[0][0]));

    MathHelper::Matrix4 projectionMatrix = MathHelper::SimpleProjectionMatrix(float(mWindowWidth) / float(mWindowHeight));
    glUniformMatrix4fv(mProjUniformLocation, 1, GL_FALSE, &(projectionMatrix.m[0][0]));

	MathHelper::vec3 lightPosition = MathHelper::vector3(lightX, 2.0, lightZ);
	glUniform3fv(lightPositionUniformLocation, 1, &(lightPosition.m[0]));

	MathHelper::vec3 eyePosition = MathHelper::vector3(0.0f, 4.33f, 2.5f);
	glUniform3fv(eyePositionUniformLocation, 1, &(eyePosition.m[0]));

	GLint ambientUniformLocation = glGetUniformLocation(mProgram, "ambient");
	float ambient = 0.3f;
	glUniform1fv(ambientUniformLocation, 1, &ambient);

    // Draw astronaut: 

	glBindVertexArray(VAOIdVec[0]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[0], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[0]));

	//draw skybox:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureVec[1]);
	glUniform1i(glGetUniformLocation(mProgram, "OurTexture"), 0);

	mat4 modelToWorldMatrix = scale(mat4(), vec3(30.0f, 30.0f, 30.0f));
	glUniformMatrix4fv(mModelUniformLocation, 1, GL_FALSE, &(modelToWorldMatrix[0][0]));

	glBindVertexArray(VAOIdVec[1]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[1], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[1]));

	//draw ground:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureVec[2]);
	glUniform1i(glGetUniformLocation(mProgram, "OurTexture"), 0);

	modelToWorldMatrix = scale(mat4(), vec3(10.0f, 10.0f, 10.0f));
	glUniformMatrix4fv(mModelUniformLocation, 1, GL_FALSE, &(modelToWorldMatrix[0][0]));

	glBindVertexArray(VAOIdVec[2]);
	glDrawElements(GL_TRIANGLES, numIndicesVec[2], GL_UNSIGNED_SHORT, (void*)(IndexDataByteOffsetVec[2]));

    mDrawCount += 1;
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//load texture from file:
	//unsigned char* image = SOIL_load_image("tiles3.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pdata);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


}


