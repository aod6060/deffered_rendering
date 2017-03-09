#include "sys.h"

Window::Window() {
	this->window = 0;
	this->context = 0;
	this->app = 0;
	this->isRunning = true;
}

void Window::init(std::string cap, Uint32 width, Uint32 height, IApp* app) {

	this->width = width;
	this->height = height;
	this->app = app;

	SDL_Init(SDL_INIT_EVERYTHING);


	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);


	this->window = SDL_CreateWindow(
		cap.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		this->width,
		this->height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	);

	this->context = SDL_GL_CreateContext(this->window);

	glewInit();

	timer.init();

	input.init();

	if (app != 0) {
		app->init(this);
	}
}

void Window::update() {
	SDL_Event e;

	while (this->isRunning) {

		timer.update();

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				this->exit();
			}

			input.doEvent(e);
		}

		

		if (app != 0) {

			app->update(this);
			app->render(this);
		}

		input.update();

		SDL_GL_SwapWindow(this->window);
	}
}

void Window::release() {

	if (app != 0) {
		app->release(this);
	}

	SDL_GL_DeleteContext(this->context);

	SDL_DestroyWindow(this->window);

	SDL_Quit();
}

Uint32 Window::getWidth() {
	return this->width;
}

Uint32 Window::getHeight() {
	return this->height;
}

void Window::exit() {
	this->isRunning = false;
}

Timer* Window::getTimer() {
	return &this->timer;
}

// Get Input
Input* Window::getInput() {
	return &this->input;
}