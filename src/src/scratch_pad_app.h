#pragma once

#include "sys.h"

class ScratchPadApp : public IApp {
public:

	virtual void init(Window* win);

	virtual void render(Window* win);

	virtual void update(Window* win);

	virtual void release(Window* win);
};