#ifndef __EditFoxEngine3D_WINDOW_H_
#define __EditFoxEngine3D_WINDOW_H_

#include "macros.h"
#include "../glfw/include/GLFW/glfw3.h"
#include <string>

NS(EFE) {
	class EFEWindow {
	public:
		EFEWindow(int width, int height, std::string windowName);
		~EFEWindow();

		EFEWindow(const EFEWindow&) = delete;
		void operator=(const EFEWindow&) = delete;

		inline bool shouldClose() const {
			return glfwWindowShouldClose(this->window);
		}
	private:
		GLFWwindow* window;
	};
}

#endif