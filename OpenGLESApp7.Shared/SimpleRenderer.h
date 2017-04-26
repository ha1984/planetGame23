#pragma once

#include <string.h>
#include <vector>
#include "../OpenGLESApp7.Android.NativeActivity/include/ShapeData.h"
#include "../OpenGLESApp7.Android.NativeActivity/include/ShapeAnim.h"
#include "../OpenGLESApp7.Android.NativeActivity/Camera.h"
#include "../../../glm/glm/glm.hpp"
#include "../../../glm/glm/gtc/matrix_transform.hpp"
#include "../../../glm/glm/gtc/type_ptr.hpp"
#include "../OpenGLESApp7.Android.NativeActivity/GenerateWalk.h"
#include "../OpenGLESApp7.Android.NativeActivity/Dialogue.h"
#include "../OpenGLESApp7.Android.NativeActivity/Bullet.h"
#include "../OpenGLESApp7.Android.NativeActivity/Collision.h"
#include "../OpenGLESApp7.Android.NativeActivity/Bomb.h"
#include "../OpenGLESApp7.Android.NativeActivity/Explosion.h"
#include "../OpenGLESApp7.Android.NativeActivity/Particle.h"

#ifdef __APPLE__
#include <unistd.h>
#include <sys/resource.h>

#include <OpenGLES/ES2/gl.h>
#else // __ANDROID__ or _WIN32
//#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
//for sounds:
//#include <SLES/OpenSLES.h>
//#include <SLES/OpenSLES_Android.h>
#endif

#include <string>


class SimpleRenderer
{
public:
	long getTimeNsec();
	std::vector<Explosion*> explosionVec;
	std::vector<Bullet*> bulletVec;
	Camera* camera;
	GLuint numberOfObj;
	std::vector<mat4> modelToWorlds;
	dialogue dialog;
	glm::vec3 pos;
	void renderGUI();
	void renderExplosion(glm::vec3 position);
	void renderText(const char* str1, const char* str2, const char* str3);
	std::vector<int> fontWidths;
	SimpleRenderer(std::vector<ShapeData>& geom, ShapeAnim& playerShape);
    ~SimpleRenderer();
    void Draw(MathHelper::Matrix4& projectionMatrix, glm::mat4& worldToViewMatrix);
	void paintGL();
    void UpdateWindowSize(GLsizei width, GLsizei height);
	void setupTexture();
	void sendTextureToOpenGL(unsigned char* pdata, GLuint& textureId, unsigned int& width, unsigned int& height);
	void sendTextureWithAlpha(unsigned char* pdata, GLuint& textureId, unsigned int& width, unsigned int& height);
	std::vector<GLuint> textureVec;
	std::vector<GLuint> numIndicesVec;
	GLuint animIndices;
	std::vector<GLuint> VAOIdVec;
	GLuint VAOanim;
	std::vector<GLuint> IndexDataByteOffsetVec;
	GLuint IndexDataByteOffset;
	std::vector<ShapeData> ShapeDataVec;
	void mouseMoveEvent(int& x);
	void runEvent(int& y, int& height);
	int step;
	int mDrawCount;
	int runCount;
	GenerateWalk* walk;
	bool answer2;
	Bomb bomb;
	void explosionOnPlane();
	glm::vec3 location1;
	glm::vec3 location2;
	glm::vec3 location3;
	glm::vec3 fogColor;
	long explosionTime;
	


private:
    GLuint mProgram;
	GLsizei mWindowWidth;
    GLsizei mWindowHeight;

    GLint mModelUniformLocation;
    GLint mViewUniformLocation;
    GLint mProjUniformLocation;
	GLint lightPositionUniformLocation;
	GLint eyePositionUniformLocation;
	GLint ambientUniformLocation;
	GLint texShiftUniformLocation;
	GLint fogColorUniformLocation;

    GLuint mVertexPositionBuffer;
    GLuint mVertexColorBuffer;
	GLuint mTextureCoordsBuffer;
    GLuint mIndexBuffer;

	GLuint theBufferID;
	GLuint newBufferID;

	GLuint	animProgram;
	GLuint	animModelUniformLocation;
	GLuint	animViewUniformLocation;
	GLuint	animProjUniformLocation;

	GLuint	animlightPositionUniformLocation;
	GLuint	animEyePositionUniformLocation;
	GLuint	animAmbientUniformLocation;

	GLuint	textProgram;
	GLuint	blurProgram;

	GLuint textureIdExplosion;
	GLuint fboIdExplosion;

};
