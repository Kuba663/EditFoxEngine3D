// EditFoxEngine3D.h: plik dołączany dla standardowych systemowych plików dołączanych,
// lub pliki dołączane specyficzne dla projektu.

#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "EFEDevice.hpp"
#include "EFEWindow.hpp"
#include "EFESwapChain.hpp"
#include "EFEPipeline.hpp"
#include "macros.h"
#include "EFEModel.hpp"

using NS(EFE::render);

NS(EFE) {
	class Engine {
	public:
		Engine(int width, int height, std::string title);
		~Engine();

		Engine(const Engine&) = delete;
		void operator=(const Engine&) = delete;

		void run();

		const int width, height;
		const std::string title;
	private:
		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();

		Window window{ width, height, title };
		Device device{ window };
		SwapChain swapChain{ device, {static_cast<uint32_t>(width), static_cast<uint32_t>(height)} };
		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::unique_ptr<Model> model;
	};
}