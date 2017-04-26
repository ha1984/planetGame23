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
#include <iterator>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <tiny_obj_loader.h>
#include <vector>
#include "Vertex.h"
#include "VertexAnimate.h"
#include <ShapeData.h>
#include <ShapeAnim.h>
#include "ShapeGenerator.h"
#include "Engine.h"
#include "bone.h"
#include "Game.h"
#include "Region.h"
#include "Region1.h"
#include "Region2.h"
#include "Check.h"
#include "Bullet.h"

using namespace std;

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "astroGame.NativeActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "astroGame.NativeActivity", __VA_ARGS__))

bone* Bone = new bone[14];
ShapeAnim playerShape;
bool run = false;
int stepCount;
Game game;
Region region;
AAsset* asset;



static std::istream &safeGetline(std::istream &is, std::string &t) {
	t.clear();

	// The characters in the stream are read one-by-one using a std::streambuf.
	// That is faster than reading them one-by-one using the std::istream.
	// Code that uses streambuf this way must be guarded by a sentry object.
	// The sentry object performs various tasks,
	// such as thread synchronization and updating the stream state.

	std::istream::sentry se(is, true);
	std::streambuf *sb = is.rdbuf();

	for (;;) {
		int c = sb->sbumpc();
		switch (c) {
		case '\n':
			return is;
		case '\r':
			if (sb->sgetc() == '\n') sb->sbumpc();
			return is;
		case EOF:
			// Also handle the case when the last line has no line ending
			if (t.empty()) is.setstate(std::ios::eofbit);
			return is;
		default:
			t += static_cast<char>(c);
		}
	}
}

vector<float> char_to_vector(unsigned char* pdata) {
	//int N = sizeof(pdata);
	std::istringstream in(reinterpret_cast<char*>(pdata));
	std::istream *inStream = &in;
	vector<float> v;

	std::string linebuf;
	while (inStream->peek() != -1) {
		safeGetline(*inStream, linebuf);

		// Trim newline '\r\n' or '\n'
		if (linebuf.size() > 0) {
			if (linebuf[linebuf.size() - 1] == '\n')
				linebuf.erase(linebuf.size() - 1);
		}
		if (linebuf.size() > 0) {
			if (linebuf[linebuf.size() - 1] == '\r')
				linebuf.erase(linebuf.size() - 1);
		}

		// Skip if empty line.
		if (linebuf.empty()) {
			continue;
		}

		// Skip leading space.
		const char *token = linebuf.c_str();

		token += strspn(token, " \t");

		assert(token);
		if (token[0] == '\0') continue;  // empty line

		if (token[0] == '#') continue;  // comment line

		std::string fs(token);
		float f = (float)std::atof(fs.c_str());
		v.push_back(f);
	}
	return v;
}

ShapeAnim& load_player(struct engine* engine, const char* filename, bone* Bone) {
	//AAsset* asset = 0;
	AAssetManager* manager = engine->app->activity->assetManager;
	asset = AAssetManager_open(manager, filename, AASSET_MODE_STREAMING);
	unsigned char* pdata = (unsigned char*)(AAsset_getBuffer(asset));

	std::istringstream in((char*)pdata);

	ShapeAnim shape = ShapeGenerator::makePlayer(&in, Bone);

	AAsset_close(asset);

	return shape;
}

vector<float> load_bone(struct engine* engine, const char* filename) {
	//AAsset* asset = 0;
	AAssetManager* manager = engine->app->activity->assetManager;
	asset = AAssetManager_open(manager, filename, AASSET_MODE_STREAMING);
	unsigned char* pdata = (unsigned char*)(AAsset_getBuffer(asset));
	vector<float> v = char_to_vector(pdata);
	AAsset_close(asset);

	return v;
}

vector<int> load_bone_idx(struct engine* engine, const char* filename) {
	//AAsset* asset = 0;
	AAssetManager* manager = engine->app->activity->assetManager;
	asset = AAssetManager_open(manager, filename, AASSET_MODE_STREAMING);
	unsigned char* pdata = (unsigned char*)(AAsset_getBuffer(asset));

	vector<float> vf = char_to_vector(pdata);
	vector<int> v;

	for (int i = 0; i < vf.size(); i++) {
		v.push_back((int)vf[i]);
	}

	AAsset_close(asset);

	return v;
}

void load_bone_weights(struct engine* engine) {
	vector<float> v = load_bone(engine, "hand_L_weights.txt");
	vector<int> idx = load_bone_idx(engine, "hand_L_indices.txt");
	Bone[0].weights.resize(v.size());
	Bone[0].weights = v;
	Bone[0].indices.resize(v.size());
	Bone[0].indices = idx;

	v = load_bone(engine, "bottom_arm_L_weights.txt");
	idx = load_bone_idx(engine, "bottom_arm_L_indices.txt");
	Bone[1].weights.resize(v.size());
	Bone[1].weights = v;
	Bone[1].indices.resize(v.size());
	Bone[1].indices = idx;

	v = load_bone(engine, "upper_arm_L_weights.txt");
	idx = load_bone_idx(engine, "upper_arm_L_indices.txt");
	Bone[2].weights.resize(v.size());
	Bone[2].weights = v;
	Bone[2].indices.resize(v.size());
	Bone[2].indices = idx;

	v = load_bone(engine, "hand_R_weights.txt");
	idx = load_bone_idx(engine, "hand_R_indices.txt");
	Bone[3].weights.resize(v.size());
	Bone[3].weights = v;
	Bone[3].indices.resize(v.size());
	Bone[3].indices = idx;

	v = load_bone(engine, "bottom_arm_R_weights.txt");
	idx = load_bone_idx(engine, "bottom_arm_R_indices.txt");
	Bone[4].weights.resize(v.size());
	Bone[4].weights = v;
	Bone[4].indices.resize(v.size());
	Bone[4].indices = idx;

	v = load_bone(engine, "upper_arm_R_weights.txt");
	idx = load_bone_idx(engine, "upper_arm_R_indices.txt");
	Bone[5].weights.resize(v.size());
	Bone[5].weights = v;
	Bone[5].indices.resize(v.size());
	Bone[5].indices = idx;

	v = load_bone(engine, "foot_front_L_weights.txt");
	idx = load_bone_idx(engine, "foot_front_L_indices.txt");
	Bone[6].weights.resize(v.size());
	Bone[6].weights = v;
	Bone[6].indices.resize(v.size());
	Bone[6].indices = idx;

	v = load_bone(engine, "foot_back_L_weights.txt");
	idx = load_bone_idx(engine, "foot_back_L_indices.txt");
	Bone[7].weights.resize(v.size());
	Bone[7].weights = v;
	Bone[7].indices.resize(v.size());
	Bone[7].indices = idx;

	v = load_bone(engine, "bottom_leg_L_weights.txt");
	idx = load_bone_idx(engine, "bottom_leg_L_indices.txt");
	Bone[8].weights.resize(v.size());
	Bone[8].weights = v;
	Bone[8].indices.resize(v.size());
	Bone[8].indices = idx;

	v = load_bone(engine, "upper_leg_L_weights.txt");
	idx = load_bone_idx(engine, "upper_leg_L_indices.txt");
	Bone[9].weights.resize(v.size());
	Bone[9].weights = v;
	Bone[9].indices.resize(v.size());
	Bone[9].indices = idx;

	v = load_bone(engine, "foot_front_R_weights.txt");
	idx = load_bone_idx(engine, "foot_front_R_indices.txt");
	Bone[10].weights.resize(v.size());
	Bone[10].weights = v;
	Bone[10].indices.resize(v.size());
	Bone[10].indices = idx;

	v = load_bone(engine, "foot_back_R_weights.txt");
	idx = load_bone_idx(engine, "foot_back_R_indices.txt");
	Bone[11].weights.resize(v.size());
	Bone[11].weights = v;
	Bone[11].indices.resize(v.size());
	Bone[11].indices = idx;

	v = load_bone(engine, "bottom_leg_R_weights.txt");
	idx = load_bone_idx(engine, "bottom_leg_R_indices.txt");
	Bone[12].weights.resize(v.size());
	Bone[12].weights = v;
	Bone[12].indices.resize(v.size());
	Bone[12].indices = idx;

	v = load_bone(engine, "upper_leg_R_weights.txt");
	idx = load_bone_idx(engine, "upper_leg_R_indices.txt");
	Bone[13].weights.resize(v.size());
	Bone[13].weights = v;
	Bone[13].indices.resize(v.size());
	Bone[13].indices = idx;
}

void stepchange(struct engine* engine) {
	engine->renderer->runEvent(engine->state.y, engine->height);

	stepCount = engine->renderer->step;
}

static int loadRegion2(struct engine* engine) {
	
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
	engine->state.x = 100;
	engine->state.y = h / 2 + 1;

	Region2 region2;

	std::vector<int> csvWidths = load_bone_idx(engine, "FontData.txt");
	std::vector<ShapeData> shapeExternVec = region2.shape_loader(engine, csvWidths);

	game.knowsMundus = false;
	game.knowsCrystal = false;
	game.hasCrystal = false;
	game.knowsPortal = false;

	if (!engine->renderer)
	{
		engine->renderer.reset(new SimpleRenderer(shapeExternVec, playerShape));
		engine->renderer->UpdateWindowSize(w, h);
		engine->renderer->setupTexture();
		engine->renderer->step = stepCount;
	}

	//load modelToWorldMatrices:
	region2.setModelToWorlds(engine);

	//load textures from file:
	region2.load_Tex(engine);

	//load text dialogues:
	region = region2.load_dialog();
	engine->renderer->dialog = region.dialog1;
	engine->renderer->fontWidths = csvWidths;
	engine->renderer->location1 = region.location1;
	engine->renderer->location2 = region.location2;
	engine->renderer->location3 = region.location3;

	return 0;
}

static int engine_init_display(struct engine* engine) {
	// Initialisieren eines EGL-Kontexts für die aktuelle Anzeige.
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
	engine->state.x = 100;
	engine->state.y = h/2 + 1;

	Region1 region1;

	std::vector<int> csvWidths = load_bone_idx(engine, "FontData.txt");
	std::vector<ShapeData> shapeExternVec = region1.shape_loader(engine, csvWidths);


	
	if (!engine->renderer)
	{
		engine->renderer.reset(new SimpleRenderer(shapeExternVec, playerShape));
		engine->renderer->UpdateWindowSize(w, h);
		engine->renderer->setupTexture();
		engine->renderer->step = stepCount;
	}
	//load modelToWorldMatrices:
	region1.setModelToWorlds(engine);

	//load textures from file:
	region1.load_Tex(engine);
	
	//load text dialogues:
	region = region1.load_dialog();
	engine->renderer->dialog = region.dialog1;
	engine->renderer->fontWidths = csvWidths;
	engine->renderer->location1 = region.location1;
	engine->renderer->location2 = region.location2;
	engine->renderer->location3 = region.location3;

	return 0;
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
* Nur der aktuelle Frame in der Anzeige.
*/
static void engine_draw_frame(struct engine* engine) {

	if (engine->display == NULL) {
		// Keine Anzeige.
		return;
	}

	if (run) {
		stepchange(engine);
	}

	if (length(engine->renderer->pos - glm::vec3(0.0, 0.0, -15.0)) < 2.0 && game.knowsCrystal) {
		game.hasCrystal = true;
		engine->renderer->dialog = region.dialog3;
	}

	if (length(engine->renderer->pos) < 2.0 && game.hasCrystal) {
		game.knowsPortal = true;
		engine->renderer->dialog = region.dialog4;
	}

	if (length(engine->renderer->pos - glm::vec3(0.0, 0.0, 15.0)) < 2.0 && game.knowsPortal) {
		engine->renderer->dialog = region.dialog5;
		game.portal1 = true;
	}
	
	engine->renderer->paintGL();

	eglSwapBuffers(engine->display, engine->surface);
}



/**
* Das nächste Eingabeereignis verarbeiten.
*/
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
	struct engine* engine = (struct engine*)app->userData;
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
		int action = AKeyEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
		int h = engine->height;
		int w = engine->width;
		//Check check(event, w, h);
		if ((action == AMOTION_EVENT_ACTION_MOVE) && AMotionEvent_getY(event, 0) < 14 * h / 20) {
			engine->state.x = AMotionEvent_getX(event, 0);
			//engine->state.y = AMotionEvent_getY(event, 0);
			engine->renderer->mouseMoveEvent(engine->state.x);
		}
		if ((action == AMOTION_EVENT_ACTION_DOWN) && AMotionEvent_getY(event, 0) < 14 * h / 20) {
			run = true;
			engine->state.y = AMotionEvent_getY(event, 0);
		}
		if (action == AMOTION_EVENT_ACTION_UP) {
			run = false;
		}
		//arrow button:
		if ((action == AMOTION_EVENT_ACTION_DOWN) && (AMotionEvent_getY(event, 0) > 14 * h / 20) && (AMotionEvent_getY(event, 0) < 18 * h / 20) && (AMotionEvent_getX(event, 0) < 3 * w / 20)) {
			engine->renderer->answer2 = !engine->renderer->answer2;
		}
		//select button:
		if ((action == AMOTION_EVENT_ACTION_DOWN) && engine->renderer->answer2 && (AMotionEvent_getY(event, 0) > 18 * h / 20) && (AMotionEvent_getX(event, 0) < 3 * w / 20)) {
			game.knowsCrystal = true;
			engine->renderer->dialog = region.dialog2;
		}
		// gun button:
		if ((action == AMOTION_EVENT_ACTION_DOWN) && (AMotionEvent_getY(event, 0) > 14 * h / 20) && (AMotionEvent_getY(event, 0) < 18 * h / 20) && (AMotionEvent_getX(event, 0) > 17 * w / 20)) {
			Bullet* bullet = new Bullet(engine->renderer->pos, engine->renderer->camera->viewDirection, engine->renderer->getTimeNsec());
			engine->renderer->bulletVec.push_back(bullet);
			//check.button3 = false;
		}

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
			//game.portal1 = true;
			if (!game.portal1) {
				engine_init_display(engine);
			}
			else {
				loadRegion2(engine);
			}
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

	//load bone rotation matrices:
	load_bone_weights(&engine);
	playerShape = load_player(&engine, "astronew2.txt", Bone);

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

