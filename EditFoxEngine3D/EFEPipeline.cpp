#include "EFEPipeline.hpp"

#include "utils.hpp"

#include <cassert>

#define CLASS Pipeline

using EFE::render::CLASS;

CLASS::Pipeline(Device& device, const std::string& vertShaderFilename, const std::string& fragShaderFilename, const PipelineConfigInfo& info)
	: device{device} {
	this->createGraphicsPipeline(vertShaderFilename, fragShaderFilename, info);
}

CLASS::~Pipeline() {
	vkDestroyShaderModule(this->device.device(), this->vertShaderModule, nullptr);
	vkDestroyShaderModule(this->device.device(), this->fragShaderModule, nullptr);
	vkDestroyPipeline(this->device.device(), this->graphicsPipeline, nullptr);
}

void CLASS::createGraphicsPipeline(const std::string& vertShaderFilename, const std::string& fragShaderFilename, const PipelineConfigInfo& info) {

	assert(
		info.pipelineLayout != nullptr &&
		"Cannot create graphics pipeline: no pipelineLayout provided in config info");
	assert(
		info.renderPass != nullptr &&
		"Cannot create graphics pipeline: no renderPass provided in config info");

	auto vertShader = readFileToVector(vertShaderFilename);
	auto fragShader = readFileToVector(fragShaderFilename);

	this->createShaderModule(vertShader, &vertShaderModule);
	this->createShaderModule(fragShader, &fragShaderModule);

	VkPipelineViewportStateCreateInfo viewportInfo{};
	viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportInfo.viewportCount = 1;
	viewportInfo.pViewports = &info.viewport;
	viewportInfo.scissorCount = 1;
	viewportInfo.pScissors = &info.scissor;

	VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
	colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendInfo.logicOpEnable = VK_FALSE;
	colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
	colorBlendInfo.attachmentCount = 1;
	colorBlendInfo.pAttachments = &info.colorBlendAttachment;
	colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
	colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
	colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
	colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

	VkPipelineShaderStageCreateInfo shaderStages[2];
	shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	shaderStages[0].module = vertShaderModule;
	shaderStages[0].pName = "main";
	shaderStages[0].flags = 0;
	shaderStages[0].pNext = nullptr;
	shaderStages[0].pSpecializationInfo = nullptr;
	shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shaderStages[1].module = fragShaderModule;
	shaderStages[1].pName = "main";
	shaderStages[1].flags = 0;
	shaderStages[1].pNext = nullptr;
	shaderStages[1].pSpecializationInfo = nullptr;

	auto bindings = Model::Vertex::getBindingDescriptions();
	auto attributes = Model::Vertex::getAttributeDescriptions();
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributes.size());
	vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindings.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributes.data();
	vertexInputInfo.pVertexBindingDescriptions = bindings.data();

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &info.inputAssemblyInfo;
	pipelineInfo.pViewportState = &viewportInfo;
	pipelineInfo.pRasterizationState = &info.rasterizationInfo;
	pipelineInfo.pColorBlendState = &colorBlendInfo;
	pipelineInfo.pDepthStencilState = &info.depthStencilInfo;
	pipelineInfo.pDynamicState = nullptr;

	pipelineInfo.layout = info.pipelineLayout;
	pipelineInfo.renderPass = info.renderPass;
	pipelineInfo.subpass = info.subpass;

	pipelineInfo.basePipelineIndex = -1;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(this->device.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) 
		throw new std::runtime_error("Nie mo¿na utworzyæ ci¹gu graficznego");
}

void CLASS::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) {
	VkShaderModuleCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	info.codeSize = code.size();
	info.pCode = reinterpret_cast<const uint32_t*>(code.data());
	if (vkCreateShaderModule(device.device(), &info, nullptr, shaderModule) != VK_SUCCESS) throw new std::runtime_error("Nie uda³o sie utworzyæ modu³u programu cieniuj¹cego");
}

void CLASS::defaultPipelineConfigInfo(PipelineConfigInfo& info, uint32_t width, uint32_t height) {
	info.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	info.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	info.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	info.viewport.x = 0.0f;
	info.viewport.y = 0.0f;
	info.viewport.width = static_cast<float>(width);
	info.viewport.height = static_cast<float>(height);
	info.viewport.minDepth = 0.0f;
	info.viewport.maxDepth = 1.0f;

	info.scissor.offset = { 0, 0 };
	info.scissor.extent = { width, height };

	info.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	info.rasterizationInfo.depthClampEnable = VK_FALSE;
	info.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
	info.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
	info.rasterizationInfo.lineWidth = 1.0f;
	info.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
	info.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	info.rasterizationInfo.depthBiasEnable = VK_FALSE;
	info.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
	info.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
	info.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

	info.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	info.multisampleInfo.sampleShadingEnable = VK_FALSE;
	info.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	info.multisampleInfo.minSampleShading = 1.0f;           // Optional
	info.multisampleInfo.pSampleMask = nullptr;             // Optional
	info.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
	info.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

	info.colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;
	info.colorBlendAttachment.blendEnable = VK_FALSE;
	info.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
	info.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
	info.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
	info.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
	info.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
	info.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

	info.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	info.depthStencilInfo.depthTestEnable = VK_TRUE;
	info.depthStencilInfo.depthWriteEnable = VK_TRUE;
	info.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	info.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	info.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
	info.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
	info.depthStencilInfo.stencilTestEnable = VK_FALSE;
	info.depthStencilInfo.front = {};  // Optional
	info.depthStencilInfo.back = {};   // Optional
}