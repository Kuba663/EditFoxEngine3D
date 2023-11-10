#include "EFEWindow.hpp"

EFE::EFEWindow::EFEWindow(int width, int height, std::string windowName) {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	this->window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}

EFE::EFEWindow::~EFEWindow() {
	glfwDestroyWindow(this->window);
	glfwTerminate();
}