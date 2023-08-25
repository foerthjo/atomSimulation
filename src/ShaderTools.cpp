#include "ShaderTools.h"

#include <GLEW/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

unsigned int ShaderTools::compileShader(unsigned int type, const std::string& shaderCode) {
	unsigned int shaderID = glCreateShader(type);
	const char* source = shaderCode.c_str();
	glShaderSource(shaderID, 1, &source, nullptr);
	glCompileShader(shaderID);

	int compilationResult;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compilationResult);
	if (compilationResult == GL_FALSE) {
		int length;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)_alloca(length * sizeof(char));
		glGetShaderInfoLog(shaderID, length, &length, message);
		std::string shaderTypeName;
		switch (type) {
			case GL_VERTEX_SHADER:		shaderTypeName = "vertex";		break;
			case GL_FRAGMENT_SHADER:	shaderTypeName = "fragment";	break;
			case GL_COMPUTE_SHADER:		shaderTypeName = "compute";		break;
			default:					shaderTypeName = "undefined";	break;
		}
		std::cout << "[ERROR] Shader compile error for " << shaderTypeName << " shader: " << message << std::endl;
		glDeleteShader(shaderID);
		return 0;
	}

	return shaderID;
}

unsigned int ShaderTools::createGeometryShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int programID = glCreateProgram();
	unsigned int vertexShaderID = ShaderTools::compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fragmentShaderID = ShaderTools::compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
	glValidateProgram(programID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}

unsigned int ShaderTools::createGeometryShaderFromFile(const std::string& filepath) {
	std::ifstream stream(filepath);

	int type = 1;
	std::stringstream streams[2];
	std::string line;
	while (getline(stream, line)) {
		if (line.find("#shader") == 0) {
			if (line.find("vertex") != std::string::npos)
				type = 0;
			if (line.find("fragment") != std::string::npos)
				type = 1;
		} else {
			streams[type] << line << "\n";
		}
	}

	return ShaderTools::createGeometryShader(streams[0].str(), streams[1].str());
}

unsigned int ShaderTools::createComputeShader(const std::string& computeShader) {
	unsigned int programID = glCreateProgram();
	unsigned int computeShaderID = ShaderTools::compileShader(GL_COMPUTE_SHADER, computeShader);

	glAttachShader(programID, computeShaderID);
	glLinkProgram(programID);
	glValidateProgram(programID);

	int linkStatus;
	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_FALSE) {
		char message[255];
		int length = 254;
		glGetProgramInfoLog(programID, length, &length, message);
		std::cout << "[ERROR] Shader program linking error: " << length << ", " << message << std::endl;
		glDeleteProgram(programID);
		return 0;
	}

	glDeleteShader(computeShaderID);

	return programID;
}

unsigned int ShaderTools::createComputeShaderFromFile(const std::string& filepath) {
	std::ifstream stream(filepath);

	std::stringstream stringStream;
	std::string line;
	while (getline(stream, line)) {
		if (line.find("#shader") != 0) {
			stringStream << line << "\n";
		}
	}

	std::string code = stringStream.str();
	// std::cout << code << std::endl;
	return createComputeShader(code);
}