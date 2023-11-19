#include "EFEModel.hpp"

#include <cassert>

using EFE::render::Model;

Model::Model(Device& device, const std::vector<Vertex>& vertices) : device{ device } {
	this->createVertexBuffers(vertices);
}

Model::~Model() {
	vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
	vkFreeMemory(device.device(), vertexMemory, nullptr);
}

void Model::createVertexBuffers(const std::vector<Vertex>& vertices) {
	vertexCount = static_cast<uint32_t>(vertices.size());
	assert(vertexCount >= 3 && "Iloœæ punktów, musi byæ wiêksza lub równa 3");
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
	device.createBuffer(
		bufferSize, 
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
		vertexBuffer, 
		vertexMemory
	);
	void* data;
	vkMapMemory(device.device(), vertexMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), bufferSize);
	vkUnmapMemory(device.device(), vertexMemory);
}

void Model::bind(VkCommandBuffer commandBuffer) {
	VkBuffer buffers[1] = { vertexBuffer };
	VkDeviceSize offsets[1] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

void Model::draw(VkCommandBuffer commandBuffer) {
	vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
}

std::vector<VkVertexInputBindingDescription> Model::Vertex::getBindingDescriptions() {
	std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
	bindingDescriptions[0].binding = 0;
	bindingDescriptions[0].stride = sizeof(Vertex);
	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> Model::Vertex::getAttributeDescriptions() {
	return { {0,0,VK_FORMAT_R32G32B32_SFLOAT, 0} };
}