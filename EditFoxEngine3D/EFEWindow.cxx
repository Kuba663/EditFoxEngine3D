#include "EFEWindow.hpp"

#include <stdexcept>

#define CLASS Window

using EFE::CLASS;

CLASS::Window(int width, int height, std::string windowName) {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	this->window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}

CLASS::~Window() {
	glfwDestroyWindow(this->window);
	glfwTerminate();
}

void CLASS::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
	if (glfwCreateWindowSurface(instance, this->window, nullptr, surface) != VK_SUCCESS) throw std::runtime_error("Nie uda³o siê utworzyæ przestrzeni okna");
}