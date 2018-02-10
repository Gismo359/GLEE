#include "../src/GLEE.hpp"

using glee::Window;

int main() {
	Window window{ "GLEE Test Window", 0, 0, 800, 600, };
	window.loop();
};