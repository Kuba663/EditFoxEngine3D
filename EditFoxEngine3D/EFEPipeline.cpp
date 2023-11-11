#include "EFEPipeline.hpp"

#include "utils.hpp"

#define CLASS Pipeline

using EFE::render::CLASS;

CLASS::Pipeline(const std::string& vertShaderFilename, const std::string& fragShaderFilename) {
	this->createGraphicsPipeline(vertShaderFilename, fragShaderFilename);
}

void CLASS::createGraphicsPipeline(const std::string& vertShaderFilename, const std::string& fragShaderFilename) {
	auto vertShader = readFileToVector(vertShaderFilename);
	auto fragShader = readFileToVector(fragShaderFilename);
}