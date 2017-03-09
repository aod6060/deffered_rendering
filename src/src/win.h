#pragma once


class Window;

class IApp {
public:
	virtual void init(Window* win) = 0;

	virtual void render(Window* win) = 0;

	virtual void update(Window* win) = 0;

	virtual void release(Window* win) = 0;
};

class Window {
private:


	Uint32 width;

	Uint32 height;

	SDL_Window* window;

	SDL_GLContext context;

	IApp* app;

	bool isRunning;

	// Timer API
	Timer timer;
	// Input API
	Input input;
public:

	Window();

	void init(std::string cap, Uint32 width, Uint32 height, IApp* app = 0);

	void update();

	void release();

	Uint32 getWidth();

	Uint32 getHeight();

	void exit();

	// Get Timer
	Timer* getTimer();
	// Get Input
	Input* getInput();
};