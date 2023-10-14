/**
* @brief Contains utils to parse the shader, and return shader objects
*/

#include <fstream>
#include <iostream>
#include <sstream>

#include <GL/gl.h>

static void parseShader(const std::string& filepath, std::string& code) {
	std::ifstream stream(filepath);
	std::stringstream ss;
	ss << stream.rdbuf();

	code = ss.str();
}

static unsigned int compileShader(unsigned int type, const std::string& code) {
	unsigned int id{ glCreateShader(type) };
	const char* src{ code.c_str() };

	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);

	int result{};
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length{};
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

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

		glGetShaderInfoLog(id, length, &length, message);
		std::cout << message << std::endl;
		return 0;
	}

	return id;
}

static unsigned int createShader(const std::string& vertex_shader, const std::string& fragment_shader) {
	unsigned int program{ glCreateProgram() };

	unsigned int vs{ compileShader(GL_VERTEX_SHADER, vertex_shader) };
	unsigned int fs{ compileShader(GL_FRAGMENT_SHADER, fragment_shader) };

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDetachShader(program, vs);
	glDetachShader(program, fs);

	return program;
}
