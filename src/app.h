#pragma once

#include <string>
#include <iostream>

#include <GL/gl.h>

#include "shader_utils.h"

static constexpr int kPointsPerTriangle{ 6 }; // aka the number of vertex points
static constexpr int kNumParticles{ 1000 }; // aka the number of triangles
float positions[kPointsPerTriangle * kNumParticles];

class App final {
public:
	void setup() {
		populatePositions();

		glGenBuffers(1, &circle_buffer_);
		glBindBuffer(GL_ARRAY_BUFFER, circle_buffer_);
		glBufferData(GL_ARRAY_BUFFER, kPointsPerTriangle * kNumParticles * sizeof(float), positions, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

		std::string vs_str{}, fs_str{};
		parseShader("shaders/shader.vert", vs_str);
		parseShader("shaders/shader.frag", fs_str);

		shader_program_ = createShader(vs_str, fs_str);
		glUseProgram(shader_program_);
	}

	void loop() {
		glDrawArrays(GL_TRIANGLES, 0, kNumParticles * 3);
	}

	void deinit() {
		glDeleteProgram(shader_program_);
	}

private:
	static constexpr float kRadius{ 0.005 };

	// GL buffer
	unsigned int circle_buffer_{};
	unsigned int shader_program_{};


	// Horribly inefficient algorithm
	// Convert to equilateral triangle ideally
	void populatePositions() {
		for (int i = 0; i < kNumParticles; i++) {
			float x = rand() % 200 / 100.0 - 1;
			float y = rand() % 200 / 100.0 - 1;
			float A_x{ x };
			float A_y{ y };
			float B_x{ x + kRadius };
			float B_y{A_y};
			float C_x{ A_x };
			float C_y{ A_y + kRadius };

			positions[i * 6 + 0] = A_x;
			positions[i * 6 + 1] = A_y;
			positions[i * 6 + 2] = B_x;
			positions[i * 6 + 3] = B_y;
			positions[i * 6 + 4] = C_x;
			positions[i * 6 + 5] = C_y;
		}
	}
};

