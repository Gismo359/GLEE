//
// Created by braynstorm on 2/10/18.
//

#ifndef GLEE_WINDOW_HPP
#define GLEE_WINDOW_HPP

#include <string>

class Window {
  public:
	Window(int width, int height);

	int getWidth();
	int getHeight();

	void setTitle(const std::string& title);
	void setSize(int width, int height);


	void close();


};


#endif //GLEE_WINDOW_HPP
