#pragma once


class Timer {
private:

	Uint32 preTime;

	Uint32 currTime;

	float delta;

public:

	Timer();

	void init();

	void update();

	float getDelta();

};