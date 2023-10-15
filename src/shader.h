#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <unordered_map>

#include "renderer.h"

class Shader {
public:
	~Shader() {
		GL_CALL(glDeleteProgram(id_));
	}

	void init(const char* vs_filepath, const char* fs_filepath) {
		const std::string vs_str{ readFile(vs_filepath) };
		const std::string fs_str{ readFile(fs_filepath) };

		GL_CALL(id_ = glCreateProgram());

		unsigned int vs{ compileShader(GL_VERTEX_SHADER, vs_str) };
		unsigned int fs{ compileShader(GL_FRAGMENT_SHADER, fs_str) };
		
		GL_CALL(glAttachShader(id_, vs));
		GL_CALL(glAttachShader(id_, fs));

		GL_CALL(glLinkProgram(id_));
		GL_CALL(glValidateProgram(id_));

		GL_CALL(glDetachShader(id_, vs));
		GL_CALL(glDetachShader(id_, fs));

		GL_CALL(glUseProgram(id_));
	}

	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
		GL_CALL(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
	}

	void setUniform2f(const std::string& name, float v0, float v1) {
		GL_CALL(glUniform2f(getUniformLocation(name), v0, v1));
	}

	void setUniform1f(const std::string& name, float v0) {
		GL_CALL(glUniform1f(getUniformLocation(name), v0));
	}

private:
	static std::string readFile(const std::string& filepath) {
		std::ifstream stream(filepath);
		std::stringstream ss;
		ss << stream.rdbuf();

		return ss.str();
	}

	static unsigned int compileShader(unsigned int type, const std::string& code) {
		unsigned int id{ glCreateShader(type) };
		const char* src{ code.c_str() };

		GL_CALL(glShaderSource(id, 1, &src, NULL));
		GL_CALL(glCompileShader(id));

		int result{};
		GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE) {
			int length{};
			GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

			char* message = (char*)malloc(length * sizeof(char));
			if (message == nullptr) {
				std::cout << "gg. malloc error in shader\n";
				exit(1);
			}

			std::cout << "gg. failed to compile shader\n";
			switch (type) {
			case GL_VERTEX_SHADER:
				std::cout << "in vertex shader\n";
				break;
			case GL_FRAGMENT_SHADER:
				std::cout << "in fragment shader\n";
				break;
			default:
				std::cout << "in... where the hell am i?\n";
			}

			GL_CALL(glGetShaderInfoLog(id, length, &length, message));
			std::cout << message << std::endl;
			return 0;
		}

		return id;
	}

	int getUniformLocation(const std::string& name) {
		if (uniform_loc_cache_.find(name) != uniform_loc_cache_.end()) {
			return uniform_loc_cache_[name];
		}

		int location;
		GL_CALL((location = glGetUniformLocation(id_, name.c_str())));
		ASSERT(location != -1, "Invalid uniform location / var may be unused");
		uniform_loc_cache_[name] = location;

		return location;
	}

	unsigned int id_{};
	std::unordered_map<std::string, unsigned int> uniform_loc_cache_{};
};