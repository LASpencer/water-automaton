#include "DemoApp.h"

int main() {
	DemoApp* app = new DemoApp();
	app->run("Water Physics Demo", 1200, 600, false);
	delete app;
	return 0;
}