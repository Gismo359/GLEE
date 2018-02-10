#include "../src/GLEE.hpp"

using glee::Window;

int main() {
	Window window{ "GLEE Test Window", 10, 10, 800, 600 };
	window.loop();
};