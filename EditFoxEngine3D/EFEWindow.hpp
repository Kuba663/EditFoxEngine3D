#ifndef __EditFoxEngine3D_WINDOW_H_
#define __EditFoxEngine3D_WINDOW_H_

#include "macros.h"
#include "../glfw/include/GLFW/glfw3.h"
#include <string>

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
	private:
		GLFWwindow* window;
	};
}

#endif