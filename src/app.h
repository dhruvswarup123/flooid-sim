#pragma once

#include <string>
#include <iostream>

#include <GL/gl.h>

#include "shader_utils.h"

class App final {
public:
	void setup() {
		glGenBuffers(1, &circle_buffer_);
		glBindBuffer(GL_ARRAY_BUFFER, circle_buffer_);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

		std::string vs_str{}, fs_str{};
		parseShader("shaders/shader.vert", vs_str);
		parseShader("shaders/shader.frag", fs_str);

		shader_program_ = createShader(vs_str, fs_str);
		glUseProgram(shader_program_);
	}

	void loop() {
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void deinit() {
		glDeleteProgram(shader_program_);
	}

private:
	// GL buffer
	unsigned int circle_buffer_{};
	unsigned int shader_program_{};

	float positions[6] = {
			-0.5f, -0.5f,
			0.0f, 0.5f,
			0.5f, -0.5f
	};
};

