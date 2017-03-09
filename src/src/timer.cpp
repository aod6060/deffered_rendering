#include "sys.h"

Timer::Timer() {
	this->currTime = 0;
	this->preTime = 0;
	this->delta = 0.0f;
}

void Timer::init() {
	this->preTime = SDL_GetTicks();
}

void Timer::update() {
	this->currTime = SDL_GetTicks();

	this->delta = ((float)(this->currTime - this->preTime)) / 1000.0f;

	this->preTime = this->currTime;
}

float Timer::getDelta() {
	return this->delta;
}