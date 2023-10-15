#pragma once

#include <GL/glew.h>
#include <iostream>

#define ASSERT(x, error) if (!(x)) {std::cout << error; exit(0);};
#define GL_CALL(x)  \
	_glClearError();\
	x;              \
	ASSERT(_glLogCall(#x, __FILE__, __LINE__), "GL_CALL failed")

void _glClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool _glLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL error]: (" << error << "): " << file << "::" << function << " : Line " << line;
		return false;
	}

	return true;
}