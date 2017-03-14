/*
* Copyright (C) 2010 Das Android Open Source-Projekt
 *
 * Lizenziert unter der Apache-Lizenz, Version 2.0 (der "Lizenz").
* Sie dürfen diese Datei nur gemäß den Bedingungen der Lizenz verwenden.
 * Sie können eine Kopie der Lizenz unter
*
*      http://www.apache.org/licenses/LICENSE-2.0 erhalten.
*
* Sofern nicht durch geltendes Recht oder durch schriftliche Zustimmung anders festgelegt, wird
 * die unter der Lizenz vertriebene Software "WIE BESEHEN",
 * OHNE GARANTIEN ODER BEDINGUNGEN GLEICH WELCHER ART, seien sie ausdrücklich oder konkludent, zur Verfügung gestellt.
 * Die unter der Lizenz geltenden Berechtigungen und Einschränkungen finden Sie
* in der Lizenz für die jeweilige Sprache.
*
*/
#define _CRT_SECURE_NO_DEPRECATE
#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc

#include <png.h>
#include <pngpriv.h>
#include <iostream>     // std::cout
#include <fstream> 
#include <istream>
#include <streambuf>
#include <string>
#include <tiny_obj_loader.h>
#include <vector>
#include "Vertex.h"
#include <ShapeData.h>
#include "ShapeGenerator.h"
#include "../OpenGLESApp7.Shared/SimpleRenderer.h"

using namespace std;

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "astroGame.NativeActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "astroGame.NativeActivity", __VA_ARGS__))

AAsset* asset = 0;
GLuint numberOfTextures = 3;
std::vector<ShapeData> shapeExternVec;

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

	std::unique_ptr<SimpleRenderer> mCubeRenderer;
};

struct membuf : std::streambuf
{
	membuf(char* begin, char* end) {
		this->setg(begin, begin, end);
	}
};

void png_asset_read(png_structp png, png_bytep data, png_size_t size) {
	//AAsset_seek(pngAsset_, 0, 0);
	AAsset_read(asset, data, size);
	/*int numBytesRemaining = AAsset_getRemainingLength(asset);
	LOGI("Read size: %d, remaining: %d", size, numBytesRemaining);*/
}


void load_texture(struct engine* engine, unsigned char* pdata, GLuint j) {
	off_t fileLength = AAsset_getLength(asset);
	unsigned char* pBuffer = new unsigned char[fileLength];

	//off_t bufferLength = sizeof(pBuffer);

	memcpy(pBuffer, pdata, fileLength * sizeof(char));

	// new for loading png image data:
	int HEADER_SIZE = 8;
	png_byte* buffer = new png_byte[HEADER_SIZE];
	int numBytesRead = AAsset_read(asset, buffer, HEADER_SIZE);
	//int numBytesRemaining = AAsset_getRemainingLength(asset);

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

	//engine->mCubeRenderer->sendTextureToOpenGL(pBuffer);
	engine->mCubeRenderer->sendTextureToOpenGL((unsigned char*)image_data, engine->mCubeRenderer->textureVec[j], twidth, theight);
}

void load_data(struct engine* engine) {
	
	    AAssetManager* manager = engine->app->activity->assetManager;

		asset = AAssetManager_open(manager, "Astronaut.png", AASSET_MODE_STREAMING);
		unsigned char* pdata = (unsigned char*)(AAsset_getBuffer(asset));
		load_texture(engine, pdata, 0);
		AAsset_close(asset);

		asset = AAssetManager_open(manager, "sky2.png", AASSET_MODE_STREAMING);
		pdata = (unsigned char*)(AAsset_getBuffer(asset));
		load_texture(engine, pdata, 1);
		AAsset_close(asset);

		asset = AAssetManager_open(manager, "ground.png", AASSET_MODE_STREAMING);
		pdata = (unsigned char*)(AAsset_getBuffer(asset));
		load_texture(engine, pdata, 2);
		AAsset_close(asset);

}

ShapeData load_geometry(struct engine* engine, const char* filename) {
	AAssetManager* manager = engine->app->activity->assetManager;
	asset = AAssetManager_open(manager, filename, AASSET_MODE_STREAMING);
	unsigned char* pdata = (unsigned char*)(AAsset_getBuffer(asset));

	size_t len = strlen((char*)pdata);
	//char tempData[929670];
	char tempData[len];
	strncpy(tempData, (char*)pdata, len);
	membuf sbuf(tempData, tempData + sizeof(tempData));
	std::istream in(&sbuf);

	ShapeData shape = ShapeGenerator::makeObject(&in);

	AAsset_close(asset);

	return shape;
}
/**
* Initialisieren eines EGL-Kontexts für die aktuelle Anzeige.
*/
static int engine_init_display(struct engine* engine) {
	// OpenGL-ES und -EGL initialisieren
	// texture:

	
	/*
	* Geben Sie hier die Attribute der gewünschten Konfiguration an.
	* Unten wählen wir eine EGLConfig mit mindestens 8 Bits pro
	* Farbkomponente aus, die mit den Fenstern auf dem Bildschirm kompatibel ist.
	*/
	const EGLint attribs[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 8,
        EGL_STENCIL_SIZE, 8,
        EGL_NONE
	};
	EGLint w, h, format;
	EGLint numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, 0, 0);

	/* Hier wählt die Anwendung die von ihr gewünschte Konfiguration aus. In diesem
	* Beispiel handelt es sich um einen stark vereinfachten Auswahlvorgang, in dem wir die 
	* erste "EGLConfig" auswählen, die unseren Kriterien entspricht. */
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	/* "EGL_NATIVE_VISUAL_ID" ist ein Attribut der "EGLConfig", dessen
	* Akzeptierung durch "ANativeWindow_setBuffersGeometry()" garantiert ist.
	* Sobald wir eine "EGLConfig" ausgewählt haben, können wir die ANativeWindow-Puffer
	* gefahrlos mithilfe von  "EGL_NATIVE_VISUAL_ID" entsprechend neu konfigurieren. */
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);

	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	context = eglCreateContext(display, config, NULL, contextAttribs);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		LOGW("Unable to eglMakeCurrent");
		return -1;
	}

	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	engine->display = display;
	engine->context = context;
	engine->surface = surface;
	engine->width = w;
	engine->height = h;
	engine->state.angle = 0;
	engine->state.x = 1000;
	engine->state.y = 1000;

	ShapeData shape = load_geometry(engine, "astron.txt.txt");
	shapeExternVec.push_back(shape);
	shape = load_geometry(engine, "sky2.txt");
	shapeExternVec.push_back(shape);

	if (!engine->mCubeRenderer)
	{
		engine->mCubeRenderer.reset(new SimpleRenderer(shapeExternVec));
		engine->mCubeRenderer->UpdateWindowSize(w, h);
	}

	engine->mCubeRenderer->textureVec.resize(numberOfTextures);
	//load textures from file:
	load_data(engine);



	return 0;
}

/**
* Nur der aktuelle Frame in der Anzeige.
*/
static void engine_draw_frame(struct engine* engine) {
	if (engine->display == NULL) {
		// Keine Anzeige.
		return;
	}

	//moving light:
	engine->mCubeRenderer->lightX = (float)engine->state.x/1000.0 -1.0;
	
	engine->mCubeRenderer->lightZ = (float)engine->state.y/1000.0 -1.0;

	engine->mCubeRenderer->Draw();

	eglSwapBuffers(engine->display, engine->surface);
}

/**
* Den aktuell der Anzeige zugeordneten EGL-Kontext entfernen.
*/
static void engine_term_display(struct engine* engine) {
	if (engine->display != EGL_NO_DISPLAY) {
		eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (engine->context != EGL_NO_CONTEXT) {
			eglDestroyContext(engine->display, engine->context);
		}
		if (engine->surface != EGL_NO_SURFACE) {
			eglDestroySurface(engine->display, engine->surface);
		}
		eglTerminate(engine->display);
	}
	engine->animating = 0;
	engine->display = EGL_NO_DISPLAY;
	engine->context = EGL_NO_CONTEXT;
	engine->surface = EGL_NO_SURFACE;
}

/**
* Das nächste Eingabeereignis verarbeiten.
*/
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
	struct engine* engine = (struct engine*)app->userData;
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
		engine->state.x = AMotionEvent_getX(event, 0);
		engine->state.y = AMotionEvent_getY(event, 0);
		return 1;
	}
	return 0;
}

/**
* Den nächsten Hauptbefehl verarbeiten.
*/
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
	struct engine* engine = (struct engine*)app->userData;
	switch (cmd) {
	case APP_CMD_SAVE_STATE:
		// Das System hat uns aufgefordert, unseren aktuellen Status zu speichern. Speichern Sie ihn.
		engine->app->savedState = malloc(sizeof(struct saved_state));
		*((struct saved_state*)engine->app->savedState) = engine->state;
		engine->app->savedStateSize = sizeof(struct saved_state);
		break;
	case APP_CMD_INIT_WINDOW:
		// Das Fenster wird angezeigt, stellen Sie es fertig.
		if (engine->app->window != NULL) {
			engine_init_display(engine);
			engine_draw_frame(engine);
		}
		break;
	case APP_CMD_TERM_WINDOW:
		// Das Fenster wird ausgeblendet oder geschlossen, bereinigen Sie es.
		engine_term_display(engine);
		break;
	case APP_CMD_GAINED_FOCUS:
		// Wenn unsere App den Fokus erhält, beginnen wir mit der Überwachung des Beschleunigungsmessers.
		if (engine->accelerometerSensor != NULL) {
			ASensorEventQueue_enableSensor(engine->sensorEventQueue,
				engine->accelerometerSensor);
			// Wir möchten 60 Ereignisse pro Sekunde abrufen.
			ASensorEventQueue_setEventRate(engine->sensorEventQueue,
				engine->accelerometerSensor, (1000L / 60) * 1000);
		}
		break;
	case APP_CMD_LOST_FOCUS:
		// Wenn unsere App den Fokus verliert, beenden wir die Überwachung des Beschleunigungsmessers.
		// Dies dient dazu, den Akku zu schonen, während die App nicht verwendet wird.
		if (engine->accelerometerSensor != NULL) {
			ASensorEventQueue_disableSensor(engine->sensorEventQueue,
				engine->accelerometerSensor);
		}
		// Animationen ebenfalls beenden.
		engine->animating = 0;
		engine_draw_frame(engine);
		break;
	}
}




/**
* Dies ist der Haupteinstiegspunkt einer systemeigenen Anwendung, die 
* "android_native_app_glue" verwendet. Sie wird in ihrem eigenen Thread mit ihrer eigenen
* Ereignisschleife zum Empfangen von Eingabeereignissen sowie zum Ausführen anderer Aktionen ausgeführt.
*/
void android_main(struct android_app* state) {

	struct engine engine;

	memset(&engine, 0, sizeof(engine));
	state->userData = &engine;
	state->onAppCmd = engine_handle_cmd;
	state->onInputEvent = engine_handle_input;
	engine.app = state;

	// Auf Überwachung des Beschleunigungsmessers vorbereiten
	engine.sensorManager = ASensorManager_getInstance();
	engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
		ASENSOR_TYPE_ACCELEROMETER);
	engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
		state->looper, LOOPER_ID_USER, NULL, NULL);

	if (state->savedState != NULL) {
		// Wir beginnen mit einem zuvor gespeicherten Status. Stellen Sie die App aus diesem wieder her.
		engine.state = *(struct saved_state*)state->savedState;
	}

	engine.animating = 1;

	// Schleife, die auf auszuführende Aktionen wartet.

	while (1) {
		// Alle ausstehenden Ereignisse lesen.
		int ident;
		int events;
		struct android_poll_source* source;

		// Wenn keine Animationen ausgeführt werden, blockieren wir das Warten auf Ereignisse dauerhaft.
		// Wenn Animationen ausgeführt werden, wird die Schleife ausgeführt, bis alle Ereignisse gelesen wurden, dann fahren
		// wir fort, den nächsten Frame der Animation zu zeichnen.
		while ((ident = ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
			(void**)&source)) >= 0) {

			// Dieses Ereignis verarbeiten.
			if (source != NULL) {
				source->process(state, source);
			}

			// Wenn ein Sensor über Daten verfügt, diese jetzt verarbeiten.
			if (ident == LOOPER_ID_USER) {
				if (engine.accelerometerSensor != NULL) {
					ASensorEvent event;
					while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
						&event, 1) > 0) {
						LOGI("accelerometer: x=%f y=%f z=%f",
							event.acceleration.x, event.acceleration.y,
							event.acceleration.z);
					}
				}
			}

			// Überprüfen, ob der Vorgang beendet wird.
			if (state->destroyRequested != 0) {
				engine_term_display(&engine);
				return;
			}
		}

		if (engine.animating) {
			// Ereignisse sind abgeschlossen, nächsten Animationsframe zeichnen.
			engine.state.angle += .01f;
			if (engine.state.angle > 1) {
				engine.state.angle = 0;
			}

			// Das Zeichnen wird auf die Aktualisierungsrate des Bildschirms gedrosselt,
			// daher muss hier keine zeitliche Steuerung erfolgen.
			engine_draw_frame(&engine);
		}
	}

	

}

