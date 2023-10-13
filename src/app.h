#pragma once

#include <GL/gl.h>  // GLUT, includes glu.h and gl.h


class App final {
public:

	void setup() {
		static constexpr int kNumVertices{ 3 };
		static constexpr int kNumPointsPerVertex{ 2 };
		float positions[kNumVertices * kNumPointsPerVertex] = {
			-0.5f, -0.5f,
			0.0f, 0.5f,
			0.5f, -0.5f
		};

		glGenBuffers(1, &circle_buffer_);
		glBindBuffer(GL_ARRAY_BUFFER, circle_buffer_);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	}

	void loop() {
		glDrawArrays(GL_DRAW_BUFFER, 0, 3);
	}

private:
	// GL buffer
	GLuint circle_buffer_{};
};

