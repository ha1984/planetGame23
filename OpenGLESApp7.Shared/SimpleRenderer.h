#pragma once

#include <string.h>
#include <vector>
#include "../OpenGLESApp7.Android.NativeActivity/include/ShapeData.h"
#include "../../../glm/glm/glm.hpp"
#include "../../../glm/glm/gtc/matrix_transform.hpp"
#include "../../../glm/glm/gtc/type_ptr.hpp"

#ifdef __APPLE__
#include <unistd.h>
#include <sys/resource.h>

#include <OpenGLES/ES2/gl.h>
#else // __ANDROID__ or _WIN32
//#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
#endif

class SimpleRenderer
{
public:
    SimpleRenderer(std::vector<ShapeData>& geom);
    ~SimpleRenderer();
    void Draw();
    void UpdateWindowSize(GLsizei width, GLsizei height);
	void sendTextureToOpenGL(unsigned char* pdata, GLuint& textureId, unsigned int& width, unsigned int& height);
	std::vector<GLuint> textureVec;
	std::vector<GLuint> numIndicesVec;
	std::vector<GLuint> VAOIdVec;
	std::vector<GLuint> IndexDataByteOffsetVec;
	std::vector<ShapeData> ShapeDataVec;
	float lightZ;
	float lightX;
private:
	
    GLuint mProgram;
    GLsizei mWindowWidth;
    GLsizei mWindowHeight;

    GLint mPositionAttribLocation;
    GLint mColorAttribLocation;
	GLint mTextureAttribLocation;

    GLint mModelUniformLocation;
    GLint mViewUniformLocation;
    GLint mProjUniformLocation;
	GLint lightPositionUniformLocation;
	GLint eyePositionUniformLocation;
	GLint ambientUniformLocation;

    GLuint mVertexPositionBuffer;
    GLuint mVertexColorBuffer;
	GLuint mTextureCoordsBuffer;
    GLuint mIndexBuffer;

	GLuint theBufferID;


    int mDrawCount;

	
};
