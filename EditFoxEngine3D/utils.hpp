#ifndef __EDITFOXENGINE_UTILS_H_
#define __EDITFOXENGINE_UTILS_H_

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

std::vector<char> readFileToVector(const std::string& fileName) {
	std::ifstream file{ filepath, std::ios::ate | std::ios::binary };
	
	if (!file.is_open()) throw new std::runtime_error("Nie mo¿na otworzyæ pliku " + fileName);
	
	size_t fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

#endif