#include "EFEWindow.hpp"

EFE::Window::Window(int width, int height, std::string windowName) {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	this->window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}

EFE::Window::~Window() {
	glfwDestroyWindow(this->window);
	glfwTerminate();
}