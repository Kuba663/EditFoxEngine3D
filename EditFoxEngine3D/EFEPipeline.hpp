#ifndef __EDITFOXENGINE_PIPELINE_H_
#define __EDITFOXENGINE_PIPELINE_H_

#include "macros.h"
#include <string>

NS(EFE) {
	NS(render) {
		class Pipeline {
		public:
			Pipeline(const std::string& vertShaderFilename, const std::string& fragShaderFilename);
		private:
			void createGraphicsPipeline(const std::string& vertShaderFilename, const std::string& fragShaderFilename);
		};
	}
}

#endif