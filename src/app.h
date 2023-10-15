#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <string>
#include <iostream>
#include <chrono>

#include <GL/glew.h>

#include "renderer.h"

#include "vertex_buffer.h"
#include "shader.h"

// Sim size
static constexpr int kNumParticles{ 100000 }; // == num primitives

// Primitives
static constexpr int kVerticesPerParticle{ 3 }; // Triangle

// One per vertex
static constexpr int kFloatsInVertexPosition{ 2 }; // 2D
static constexpr int kFloatsInCentroid{ 2 }; // 2D
static constexpr int kFloatsInColor{ 3 }; // 3 nums to a color

// Total number of floats
static constexpr int kFloatsPerVertex{ 
	kFloatsInVertexPosition + kFloatsInCentroid + kFloatsInColor
}; 

static constexpr int kTotalFloats{ kNumParticles * kVerticesPerParticle * kFloatsPerVertex };

// x, y, x_centroid, y_centroid, r, g, b
static float vertex_buffer[kTotalFloats];
static float vel[2 * kNumParticles];

static constexpr float one_by_tan_30{ 1.7320508075688772935274463415058723669428052538103806280558069794 };

auto start_time = std::chrono::high_resolution_clock::now();

VertexBuffer vb{kTotalFloats * sizeof(float)};
Shader shader{};

struct Vertex {
	float pos[2];
	float centroid[2];
	float col[3];
};

static Vertex buffer[kNumParticles * 3]{};

namespace App {
	static constexpr float kRadius{ 0.005 };

	// Horribly inefficient algorithm
// Convert to equilateral triangle ideally
	void populatePositions() {
		for (int i = 0; i < kNumParticles; i++) {
			vel[i * 2 + 0] = (rand() % 100 / 100.0 - 0.5)/1000.0;
			vel[i * 2 + 1] = -(rand() % 100 / 100.0)/100.0;

			float x = rand() % 200 / 100.0 - 1;
			float y = rand() % 200 / 100.0 - 1;

			float A_x{ x };
			float A_y{ y + 2 * kRadius };

			float B_x{ x + one_by_tan_30 * kRadius };
			float B_y{ y - kRadius };

			float C_x{ x - one_by_tan_30 * kRadius };
			float C_y{ y - kRadius };

			float r = rand() % 100 / 100.0;
			float g = rand() % 100 / 100.0;
			float b = rand() % 100 / 100.0;

			vertex_buffer[i * 21 + 0] = A_x;
			vertex_buffer[i * 21 + 1] = A_y;
			vertex_buffer[i * 21 + 2] = x;
			vertex_buffer[i * 21 + 3] = y;
			vertex_buffer[i * 21 + 4] = r;
			vertex_buffer[i * 21 + 5] = g;
			vertex_buffer[i * 21 + 6] = b;

			vertex_buffer[i * 21 + 7] = B_x;
			vertex_buffer[i * 21 + 8] = B_y;
			vertex_buffer[i * 21 + 9] = x;
			vertex_buffer[i * 21 + 10] = y;
			vertex_buffer[i * 21 + 11] = r;
			vertex_buffer[i * 21 + 12] = g;
			vertex_buffer[i * 21 + 13] = b;

			vertex_buffer[i * 21 + 14] = C_x;
			vertex_buffer[i * 21 + 15] = C_y;
			vertex_buffer[i * 21 + 16] = x;
			vertex_buffer[i * 21 + 17] = y;
			vertex_buffer[i * 21 + 18] = r;
			vertex_buffer[i * 21 + 19] = g;
			vertex_buffer[i * 21 + 20] = b;
		}
	}

	void populatePositionsProper() {
		for (int i = 0; i < kNumParticles; i++) {
			vel[i * 2 + 0] = (rand() % 100 / 100.0 - 0.5) / 1000.0;
			vel[i * 2 + 1] = -(rand() % 100 / 100.0) / 100.0;

			float x = rand() % 200 / 100.0 - 1;
			float y = rand() % 200 / 100.0 - 1;
			
			buffer[i * 3 + 0].pos[0] = x;
			buffer[i * 3 + 0].pos[1] = y;
			buffer[i * 3 + 1].pos[0] = x + kRadius;
			buffer[i * 3 + 1].pos[1] = y;
			buffer[i * 3 + 2].pos[0] = x;
			buffer[i * 3 + 2].pos[1] = y + kRadius;

			buffer[i * 3 + 0].centroid[0] = x;
			buffer[i * 3 + 0].centroid[1] = y;
			buffer[i * 3 + 1].centroid[0] = x;
			buffer[i * 3 + 1].centroid[1] = y;
			buffer[i * 3 + 2].centroid[0] = x;
			buffer[i * 3 + 2].centroid[1] = y;

			float r = rand() % 100 / 100.0;
			float g = rand() % 100 / 100.0;
			float b = rand() % 100 / 100.0;

			buffer[i * 3 + 0].col[0] = r;
			buffer[i * 3 + 0].col[1] = g;
			buffer[i * 3 + 0].col[2] = b;
			buffer[i * 3 + 1].col[0] = r;
			buffer[i * 3 + 1].col[1] = g;
			buffer[i * 3 + 1].col[2] = b;
			buffer[i * 3 + 2].col[0] = r;
			buffer[i * 3 + 2].col[1] = g;
			buffer[i * 3 + 2].col[2] = b;
		}
	}
	
	void updatePositions() {
		for (int i = 0; i < kNumParticles; i++) {
			vel[i * 2 + 1] += -9.8 * 1/200 * 1/100;
			float x = vertex_buffer[i * 21 + 2] + vel[i * 2];
			float y = vertex_buffer[i * 21 + 3] + vel[i * 2 + 1];

			if ((x < -1) || (x > 1)) {
				vel[2 * i] *= -0.8;
			}
			if ((y < -1) || (y > 1)) {
				y = -1;
				vel[2 * i + 1] *= -0.8;
				vel[2 * i] *= 0.9;
			}

			float A_x{ x };
			float A_y{ y + 2 * kRadius };

			float B_x{ x + one_by_tan_30 * kRadius };
			float B_y{ y - kRadius };

			float C_x{ x - one_by_tan_30 * kRadius };
			float C_y{ y - kRadius };

			float r = rand() % 100 / 100.0;
			float g = rand() % 100 / 100.0;
			float b = rand() % 100 / 100.0;

			vertex_buffer[i * 21 + 0] = A_x;
			vertex_buffer[i * 21 + 1] = A_y;
			vertex_buffer[i * 21 + 2] = x;
			vertex_buffer[i * 21 + 3] = y;
			//vertex_buffer[i * 21 + 4] = r;
			//vertex_buffer[i * 21 + 5] = g;
			//vertex_buffer[i * 21 + 6] = b;

			vertex_buffer[i * 21 + 7] = B_x;
			vertex_buffer[i * 21 + 8] = B_y;
			vertex_buffer[i * 21 + 9] = x;
			vertex_buffer[i * 21 + 10] = y;
			//vertex_buffer[i * 21 + 11] = r;
			//vertex_buffer[i * 21 + 12] = g;
			//vertex_buffer[i * 21 + 13] = b;

			vertex_buffer[i * 21 + 14] = C_x;
			vertex_buffer[i * 21 + 15] = C_y;
			vertex_buffer[i * 21 + 16] = x;
			vertex_buffer[i * 21 + 17] = y;
			//vertex_buffer[i * 21 + 18] = r;
			//vertex_buffer[i * 21 + 19] = g;
			//vertex_buffer[i * 21 + 20] = b;
		}
	}	

	void setup() {
		populatePositions();

		vb.init(vertex_buffer, kTotalFloats * sizeof(float));
		shader.init("shaders/shader.vert", "shaders/shader.frag");

		shader.setUniform1f("u_radius", kRadius);

		// 2D position
		GL_CALL(glEnableVertexAttribArray(0));
		GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, kFloatsPerVertex * sizeof(float), (void*)0));

		// Centroid
		GL_CALL(glEnableVertexAttribArray(2));
		GL_CALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, kFloatsPerVertex * sizeof(float), (void*)8));

		// Color
		GL_CALL(glEnableVertexAttribArray(4));
		GL_CALL(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, kFloatsPerVertex * sizeof(float), (void*)16));

		//// 2D position
		//GL_CALL(glEnableVertexAttribArray(0));
		//GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos)));

		//// Centroid
		//GL_CALL(glEnableVertexAttribArray(2));
		//GL_CALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, centroid)));

		//// Color
		//GL_CALL(glEnableVertexAttribArray(4));
		//GL_CALL(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col)));
	}

	void loop() {
		auto time = std::chrono::high_resolution_clock::now();
		auto duration = duration_cast<std::chrono::microseconds>(time - start_time);
		if (duration >= std::chrono::milliseconds(10)) {
			updatePositions();
			vb.subData(vertex_buffer, kTotalFloats * sizeof(float));
			start_time = time;
		}
		GL_CALL(glDrawArrays(GL_TRIANGLES, 0, kTotalFloats));
	}

	void deinit() {
	}
};

