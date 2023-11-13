// EditFoxEngine3D.cpp: definiuje punkt wejścia dla aplikacji.
//

#include "EditFoxEngine3D.h"

using namespace std;

using EFE::Engine;

int main()
{
	cout << "Hello CMake." << endl;
	return 0;
}

Engine::Engine(int width, int height, std::string title)
	:width{ width }, height{ height }, title{ title }
{
	createPipelineLayout();
	createPipeline();
	createCommandBuffers();
	drawFrame();
}

void Engine::run() {
	while (!window.shouldClose()) {
		glfwPollEvents();
		drawFrame();
	}
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
	auto pipelineConfig = Pipeline::defaultPipelineConfigInfo(swapChain.width(), swapChain.height());
	pipelineConfig.renderPass = swapChain.getRenderPass();
	pipelineConfig.pipelineLayout = pipelineLayout;
	this->pipeline = std::make_unique<Pipeline>(device, "", "", pipelineConfig);
}

void Engine::createCommandBuffers() {
	commandBuffers.resize(swapChain.MAX_FRAMES_IN_FLIGHT);

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
		vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

		vkCmdEndRenderPass(commandBuffers[i]);
		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) throw new std::runtime_error("Nie udało się zapisać bufora poleceń!");
	}
}

void Engine::drawFrame() {
	uint32_t imageIndex;
	auto result = swapChain.acquireNextImage(&imageIndex);

	if (result != VK_SUCCESS || result != VK_SUBOPTIMAL_KHR) throw new std::runtime_error("TMP ERROR");

	result = swapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

	if (result != VK_SUCCESS) throw new std::runtime_error("");
}