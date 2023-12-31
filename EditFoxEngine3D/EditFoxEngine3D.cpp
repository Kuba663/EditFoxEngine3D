﻿// EditFoxEngine3D.cpp: definiuje punkt wejścia dla aplikacji.
//
#define NDEBUG
#include "EditFoxEngine3D.h"

using namespace std;

using EFE::Engine;

int main()
{
	try {
		Engine engine{ 1600, 800, "Test" };
		engine.run();
	}
	catch (const std::exception& e) {
		cerr << e.what() << endl;
	}
	return 0;
}

Engine::Engine(int width, int height, std::string title)
	:width{ width }, height{ height }, title{ title }
{
	loadModels();
	createPipelineLayout();
	createPipeline();
	createCommandBuffers();
}

Engine::~Engine() { vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr); }

void Engine::run() {
	while (!window.shouldClose()) {
		glfwPollEvents();
		drawFrame();
	}

	vkDeviceWaitIdle(device.device());
}

void EditFoxEngine::Engine::loadModels()
{
	std::vector<Model::Vertex> vertices{
		{{0.0f, -0.5f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}},
		{{-0.5f, 0.5f, 0.0f}}
	};
	model = std::make_unique<Model>(device, vertices);
}

void Engine::createPipelineLayout() {
	VkPipelineLayoutCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	info.setLayoutCount = 0;
	info.pSetLayouts = nullptr;
	info.pushConstantRangeCount = 0;
	info.pPushConstantRanges = nullptr;
	if (vkCreatePipelineLayout(device.device(), &info, nullptr, &pipelineLayout) != VK_SUCCESS) throw new std::runtime_error("Nie udało się utworzyć układu ciągu graficznego");
}

void Engine::createPipeline()
{
	PipelineConfigInfo pipelineConfig{};
	Pipeline::defaultPipelineConfigInfo(pipelineConfig, swapChain.width(), swapChain.height());
	pipelineConfig.renderPass = swapChain.getRenderPass();
	pipelineConfig.pipelineLayout = pipelineLayout;
	this->pipeline = std::make_unique<Pipeline>(device, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);
}

void Engine::createCommandBuffers() {

	commandBuffers.resize(swapChain.imageCount());

	VkCommandBufferAllocateInfo	allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = device.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
	if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) throw new std::runtime_error("Alokacja buforów poleceń uległa niepowodzeniu");

	for (size_t i = 0; i < commandBuffers.size(); i++) {
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) throw new std::runtime_error("Nie udało się zapoczątkować bufora komend");

		VkRenderPassBeginInfo passInfo{};
		passInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		passInfo.renderPass = swapChain.getRenderPass();
		passInfo.framebuffer = swapChain.getFrameBuffer(i);

		passInfo.renderArea = { {0,0}, swapChain.getSwapChainExtent() };

		VkClearValue clearValues[2] = { {{0.1f, 0.1f, 0.1f, 1.0f}}, {} };
		clearValues[1].depthStencil = { 1.0f, 0 };
		passInfo.clearValueCount = 2;
		passInfo.pClearValues = clearValues;
		
		vkCmdBeginRenderPass(commandBuffers[i], &passInfo, VK_SUBPASS_CONTENTS_INLINE);

		pipeline->bind(commandBuffers[i]);
		model->bind(commandBuffers[i]);
		model->draw(commandBuffers[i]);

		vkCmdEndRenderPass(commandBuffers[i]);
		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) throw new std::runtime_error("Nie udało się zapisać bufora poleceń!");
	}
}

void Engine::drawFrame() {
	uint32_t imageIndex;
	auto result = swapChain.acquireNextImage(&imageIndex);

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) throw new std::runtime_error("TMP ERROR");

	result = swapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

	if (result != VK_SUCCESS) throw new std::runtime_error("");
}