#pragma once
#include "../OpenGLESApp7.Shared/SimpleRenderer.h"


/**
* Unsere gespeicherten Statusdaten.
*/
struct saved_state {
	float angle;
	int32_t x;
	int32_t y;
};

/**
* Freigegebener Status für unsere App.
*/

struct engine {
	struct android_app* app;

	ASensorManager* sensorManager;
	const ASensor* accelerometerSensor;
	ASensorEventQueue* sensorEventQueue;

	int animating;
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	int32_t width;
	int32_t height;
	struct saved_state state;

	std::unique_ptr<SimpleRenderer> renderer;
};
