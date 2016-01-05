#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <android/log.h>

/*#include "Matrix/Matrix.h"

using namespace MatrixEngine;
using namespace MatrixEngine::Core;
using namespace MatrixEngine::Graphics;*/


#ifdef VSCODEGEN
/**
* Dati sullo stato salvato.
*/

struct saved_state{
float angle;
int32_t x;
int32_t y;
};

/**
* Stato condiviso dell'app.
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
};

/**
* Inizializza un contesto EGL per la visualizzazione corrente.
*/
static int engine_init_display(struct engine* engine) {
	// Inizializza OpenGL ES e EGL

	/*
	* In questo punto vengono specificati gli attributi della configurazione desiderata.
	* Di seguito viene selezionato un oggetto EGLConfig con almeno 8 bit per componente
	* colore compatibile con le finestre su schermo
	*/
	const EGLint attribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
	};
	EGLint w, h, format;
	EGLint numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, 0, 0);

	/* Qui l'applicazione sceglie la configurazione desiderata. In questo
	* esempio il processo di selezione è molto semplificato e viene selezionato
	* il primo oggetto EGLConfig corrispondente ai criteri */
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID è un attributo dell'oggetto EGLConfig che viene
	* sicuramente accettato da ANativeWindow_setBuffersGeometry().
	* Non appena si seleziona un oggetto EGLConfig, si può riconfigurare in tutta sicurezza
	* i buffer ANativeWindow da abbinare usando EGL_NATIVE_VISUAL_ID. */
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
	context = eglCreateContext(display, config, NULL, NULL);

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

	// Inizializza lo stato GL.
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glEnable(GL_CULL_FACE);
	//glShadeModel(GL_SMOOTH);
	glDisable(GL_DEPTH_TEST);

	return 0;
}

/**
* Solo il frame corrente della visualizzazione.
*/
static void engine_draw_frame(struct engine* engine) {
	if (engine->display == NULL) {
		// Nessuna visualizzazione.
		return;
	}

	// Applica solo un colore di riempimento alla schermata.
	//glClearColor(((float)engine->state.x) / engine->width, engine->state.angle,
	//	((float)engine->state.y) / engine->height, 1);
	//glClear(GL_COLOR_BUFFER_BIT);

	MatrixEngine::Graphics::RenderPipeline::ClearScreen(glm::vec4((rand() % 10) / 10.0, (rand() % 10) / 10.0, (rand() % 10) / 10.0, 1.0));
	eglSwapBuffers(engine->display, engine->surface);
}

/**
* Disinstalla il contesto EGL attualmente associato alla visualizzazione.
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
* Elabora l'evento di input successivo.
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
* Elabora il comando principale successivo.
*/
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
	struct engine* engine = (struct engine*)app->userData;
	switch (cmd) {
	case APP_CMD_SAVE_STATE:
		// Salva lo stato corrente come richiesto dal sistema.
		engine->app->savedState = malloc(sizeof(struct saved_state));
		*((struct saved_state*)engine->app->savedState) = engine->state;
		engine->app->savedStateSize = sizeof(struct saved_state);
		break;
	case APP_CMD_INIT_WINDOW:
		// Prepara la finestra che verrà visualizzata.
		if (engine->app->window != NULL) {
			engine_init_display(engine);
			engine_draw_frame(engine);
		}
		break;
	case APP_CMD_TERM_WINDOW:
		// Pulisce o chiude la finestra che verrà nascosta o chiusa.
		engine_term_display(engine);
		break;
	case APP_CMD_GAINED_FOCUS:
		// Quando l'app ottiene lo stato attivo, avvia il monitoraggio dell'accelerometro.
		if (engine->accelerometerSensor != NULL) {
			ASensorEventQueue_enableSensor(engine->sensorEventQueue,
				engine->accelerometerSensor);
			// Imposta 60 eventi al secondo.
			ASensorEventQueue_setEventRate(engine->sensorEventQueue,
				engine->accelerometerSensor, (1000L / 60) * 1000);
		}
		break;
	case APP_CMD_LOST_FOCUS:
		// Quando l'app perde lo stato attivo, interrompe il monitoraggio dell'accelerometro.
		// In tal modo si evita di consumare la batteria quando non è in uso.
		if (engine->accelerometerSensor != NULL) {
			ASensorEventQueue_disableSensor(engine->sensorEventQueue,
				engine->accelerometerSensor);
		}
		// Viene interrotta anche l'animazione.
		engine->animating = 0;
		engine_draw_frame(engine);
		break;
	}
}

/**
* Questo è il punto di ingresso principale di un'applicazione nativa che usa
* android_native_app_glue. Viene eseguito nel proprio thread, con il proprio
* ciclo di eventi per ricevere eventi di input ed eseguire altre operazioni.
*/
void android_main(struct android_app* state) {
	SDL_Window *window = 0;
	SDL_GLContext gl = 0;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Unable to init SDL %s\n", SDL_GetError());
		return;
	}


	/*SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_DisplayMode mode;
	SDL_GetDisplayMode(0, 0, &mode);
	int width = mode.w;
	int height = mode.h;

	SDL_Log("Width = %d, Height = %d.\n", width, height);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	window = SDL_CreateWindow(NULL, 0, 0, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE);

	if (window == 0)
	{
		SDL_Log("Failed to create window");
		SDL_Quit();
		return;
	}

	gl = SDL_GL_CreateContext(window);

	Uint8 done = 0;
	SDL_Event event;
	int count = 0;

	while (!done)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN || event.type == SDL_FINGERDOWN)
				done = 1;
		}

		glClearColor((rand() % 256) / 256.0f, (rand() % 256) / 256.0f, (rand() % 256) / 256.0f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		SDL_Log("%d\n", count++);
		SDL_GL_SwapWindow(window);
		SDL_Delay(0);
	}*/

	exit(0);
	/*
	struct engine engine;

	memset(&engine, 0, sizeof(engine));
	state->userData = &engine;
	state->onAppCmd = engine_handle_cmd;
	state->onInputEvent = engine_handle_input;
	engine.app = state;

	// Prepara il monitoraggio dell'accelerometro
	engine.sensorManager = ASensorManager_getInstance();
	engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
		ASENSOR_TYPE_ACCELEROMETER);
	engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
		state->looper, LOOPER_ID_USER, NULL, NULL);

	if (state->savedState != NULL) {
		// Esegue il ripristino da uno stato salvato precedente da cui si inizierà.
		engine.state = *(struct saved_state*)state->savedState;
	}

	engine.animating = 1;

	// Ciclo in attesa delle operazioni da eseguire.

	while (1) {
		// Legge tutti gli eventi in sospeso.
		int ident;
		int events;
		struct android_poll_source* source;

		// In assenza di animazione, blocca per sempre l'attesa di eventi.
		// In presenza di animazione, esegue il ciclo fino alla lettura di tutti gli eventi,
		// quindi passa al frame di animazione successivo.
		while ((ident = ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
			(void**)&source)) >= 0) {

			// Elabora questo evento.
			if (source != NULL) {
				source->process(state, source);
			}

			// Elabora i dati disponibili per un sensore.
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

			// Verifica la chiusura.
			if (state->destroyRequested != 0) {
				engine_term_display(&engine);
				return;
			}
		}

		if (engine.animating) {
			// Elaborazione degli eventi completata. Disegna il frame di animazione successivo.
			engine.state.angle += .01f;
			if (engine.state.angle > 1) {
				engine.state.angle = 0;
			}

			// Il disegno viene limitato alla velocità di aggiornamento della schermata, quindi
			// non è necessario eseguire qui la temporizzazione.
			engine_draw_frame(&engine);
		}
	}
	*/
}
#endif // VSCODEGEN



int main(int argc, char **argv) {
	__android_log_write(ANDROID_LOG_INFO, "MatrixEngine_main", "THIS IS THE MAIN");
	__android_log_print(ANDROID_LOG_INFO, "MatrixEngine_main", "THIS IS THE MAIN");

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Log("THIS IS THE MAIN");
	

	exit(0);
}