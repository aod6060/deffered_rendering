#include "scratch_pad_app.h"


int main(int argc, char** argv) {

	ScratchPadApp app;

	Window window;

	window.init("Deffered Renderer", 800, 600, &app);

	window.update();

	window.release();

	return 0;
}