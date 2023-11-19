#ifndef __EDITFOXENGINE_MODEL_H_
#define __EDITFOXENGINE_MODEL_H_

#include "macros.h"
#include "EFEDevice.hpp"
#include <vulkan/vulkan.h>

NS(EFE) {
	NS(render) {
		class Model {
		public:

			struct Vertex {
				float position[3];

				static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
				static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
			};

			Model(Device& device, const std::vector<Vertex>& vertices);
			~Model();

			Model(const Model&) = delete;
			void operator=(const Model&) = delete;

			void bind(VkCommandBuffer cmdBuffer);
			void draw(VkCommandBuffer cmdBuffer);

		private:
			void createVertexBuffers(const std::vector<Vertex> &vertices);
			Device& device;
			VkBuffer vertexBuffer;
			VkDeviceMemory vertexMemory;
			uint32_t vertexCount;
		};
	}
}

#endif