#pragma once
#include <string>

class ShaderTools {
public:
	static unsigned int compileShader(unsigned int type, const std::string& shaderCode);
	static unsigned int createGeometryShader(const std::string& vertexShader, const std::string& fragmentShader);
	static unsigned int createGeometryShaderFromFile(const std::string& filepath);

	static unsigned int createComputeShader(const std::string& computeShader);
	static unsigned int createComputeShaderFromFile(const std::string& filepath);
};