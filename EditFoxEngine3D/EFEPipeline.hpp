#ifndef __EDITFOXENGINE_PIPELINE_H_
#define __EDITFOXENGINE_PIPELINE_H_

#include "macros.h"
#include <string>
#include "EFEDevice.hpp"
#include "EFEModel.hpp"

NS(EFE) {
	NS(render) {
		struct PipelineConfigInfo {
			PipelineConfigInfo() = default;
			PipelineConfigInfo(const PipelineConfigInfo&) = delete;
			PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

			VkViewport viewport;
			VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
			VkRect2D scissor;
			VkPipelineRasterizationStateCreateInfo rasterizationInfo;
			VkPipelineMultisampleStateCreateInfo multisampleInfo;
			VkPipelineColorBlendAttachmentState colorBlendAttachment;
			VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
			VkPipelineLayout pipelineLayout = nullptr;
			VkRenderPass renderPass = nullptr;
			uint32_t subpass = 0;
		};
		class Pipeline {
		public:
			Pipeline(Device& device, const std::string& vertShaderFilename, const std::string& fragShaderFilename, const PipelineConfigInfo& info);
			~Pipeline();

			Pipeline(const Pipeline&) = delete;
			void operator=(const Pipeline&) = delete;

			inline void bind(VkCommandBuffer cmdBuffer) { vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->graphicsPipeline); }

			static void defaultPipelineConfigInfo(PipelineConfigInfo& info, uint32_t width, uint32_t height);

		private:
			void createGraphicsPipeline(const std::string& vertShaderFilename, const std::string& fragShaderFilename, const PipelineConfigInfo& info);

			void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

			Device& device; // A Pipeline can't exist without a device!
			VkPipeline graphicsPipeline;
			VkShaderModule vertShaderModule;
			VkShaderModule fragShaderModule;
		};
	}
}

#endif