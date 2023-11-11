#ifndef __EditFoxEngine3D_WINDOW_H_
#define __EditFoxEngine3D_WINDOW_H_

#include "macros.h"
#define GLFW_INCLUDE_VULKAN
#include "../glfw/include/GLFW/glfw3.h"
#include <string>
#include <vulkan/vulkan.h>


NS(EFE) {
	class Window {
	public:
		Window(int width, int height, std::string windowName);
		~Window();

		Window(const Window&) = delete;
		void operator=(const Window&) = delete;

		inline bool shouldClose() const {
			return glfwWindowShouldClose(this->window);
		}

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
	private:
		GLFWwindow* window;
	};
}

#endif