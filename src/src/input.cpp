#include "sys.h"

// Input Events
void Input::onKeyboardEvent(SDL_KeyboardEvent& e) {
	Uint32 s = e.keysym.scancode;
	bool pressed = (e.state == SDL_PRESSED) ? true : false;
	if (pressed) {
		if (this->keyStates[s] == IS_RELEASE) {
			this->keyStates[s] = IS_ONCE;
		}
	}
	else {
		if (this->keyStates[s] != IS_RELEASE) {
			this->keyStates[s] = IS_RELEASE;
		}
	}
}

void Input::onMouseButtonEvent(SDL_MouseButtonEvent& e) {
	Uint32 s = e.button - 1;
	bool pressed = (e.state == SDL_PRESSED) ? true : false;
	if (pressed) {
		if (mouseStates[s] == IS_RELEASE) {
			this->mouseStates[s] = IS_ONCE;
		}
	}
	else {
		if (this->mouseStates[s] != IS_RELEASE) {
			this->mouseStates[s] = IS_RELEASE;
		}
	}
}

void Input::onMouseMotionEvent(SDL_MouseMotionEvent& e) {
	if (this->isGrab()) {
		this->mx = e.xrel;
		this->my = e.yrel;
	}
	else {
		this->mx = e.x;
		this->my = e.y;
	}
}

Input::Input() {
	this->mx = 0;
	this->my = 0;
}

// Key Methods
void Input::init() {
	for (int i = 0; i < Input::KB_SIZE; i++) {
		this->keyStates[i] = IS_RELEASE;
	}

	for (int i = 0; i < Input::MB_SIZE; i++) {
		this->mouseStates[i] = IS_RELEASE;
	}
}

void Input::update() {

	for (int i = 0; i < Input::KB_SIZE; i++) {
		
		if (this->keyStates[i] == IS_ONCE) {
			this->keyStates[i] = IS_PRESSED;
		}
	}

	for (int i = 0; i < Input::MB_SIZE; i++) {
		if (this->mouseStates[i] == IS_ONCE) {
			this->mouseStates[i] = IS_PRESSED;
		}
	}

	if (this->isGrab()) {
		this->mx = 0;
		this->my = 0;
	}
}

void Input::doEvent(SDL_Event& e) {
	switch (e.type) {
	case SDL_KEYDOWN:
	case SDL_KEYUP:
		this->onKeyboardEvent(e.key);
		break;
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		this->onMouseButtonEvent(e.button);
		break;
	case SDL_MOUSEMOTION:
		this->onMouseMotionEvent(e.motion);
		break;
	}
}

// Keyboard 
bool Input::isKeyHit(Keyboard k) {
	return this->keyStates[k] == IS_ONCE;
}

bool Input::isKeyPress(Keyboard k) {
	return this->keyStates[k] == IS_ONCE || this->keyStates[k] == IS_PRESSED;
}

// Mouse
bool Input::isMouseButtonHit(MouseButton mb) {
	return this->mouseStates[mb] == IS_ONCE;
}

bool Input::isMouseButtonPress(MouseButton mb) {
	return this->mouseStates[mb] == IS_ONCE || this->mouseStates[mb] == IS_PRESSED;
}

void Input::getMousePosition(glm::ivec2& mc) {
	mc.x = mx;
	mc.y = my;
}

// Handling Grab
void Input::setGrab(bool b) {
	if (b) {
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	else {
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}

bool Input::isGrab() {
	return (SDL_GetRelativeMouseMode() == SDL_TRUE) ? true : false;
}

void Input::toggleGrab() {
	this->setGrab(!this->isGrab());
}