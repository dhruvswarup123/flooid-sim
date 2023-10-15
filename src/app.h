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

// Customizable
static constexpr int kNumParticles{ 100000 }; // == num primitives
static constexpr float kParticleRadius{ 0.005 };

// Primitives
static constexpr int kVerticesPerParticle{ 3 }; // Triangle
static constexpr int kFloatsInVertexPosition{ 2 }; // 2D position
static constexpr int kFloatsInCentroid{ 2 }; // 2D centroid
static constexpr int kFloatsInColor{ 3 }; // 3D color

// Total number of floats
static constexpr int kFloatsPerVertex{ kFloatsInVertexPosition + kFloatsInCentroid + kFloatsInColor}; 
static constexpr int kTotalFloats{ kNumParticles * kVerticesPerParticle * kFloatsPerVertex };
static constexpr int kFloatsPerParticle{ kVerticesPerParticle * kFloatsPerVertex };

// x, y, x_centroid, y_centroid, r, g, b
static float vertex_buffer[kTotalFloats];
std::mutex vb_mutex{};

static float velocities[2 * kNumParticles];


class App {
public:
	void setup() {
		populatePositions();

		vb.init(vertex_buffer, kTotalFloats * sizeof(float));
		shader.init("shaders/shader.vert", "shaders/shader.frag");

		shader.setUniform1f("u_radius", kParticleRadius);

		// 2D position
		GL_CALL(glEnableVertexAttribArray(0));
		GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, kFloatsPerVertex * sizeof(float), (void*)0));

		// Centroid
		GL_CALL(glEnableVertexAttribArray(2));
		GL_CALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, kFloatsPerVertex * sizeof(float), (void*)8));

		// Color
		GL_CALL(glEnableVertexAttribArray(4));
		GL_CALL(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, kFloatsPerVertex * sizeof(float), (void*)16));
	}

	void loop() {
		auto time = std::chrono::high_resolution_clock::now();
		auto duration = duration_cast<std::chrono::microseconds>(time - start_time);
		if (duration >= std::chrono::milliseconds(1)) {
			updatePositions();
			vb.subData(vertex_buffer, kTotalFloats * sizeof(float));
			start_time = time;
		}
		GL_CALL(glDrawArrays(GL_TRIANGLES, 0, kTotalFloats));
	}

private:
	static float random(float min, float max)  {
		return min + ((float)rand() / (float)RAND_MAX) * (max - min);
	}

	static void calculateTriangleFromCentroid(const float &x, const float& y, float& x1, float &y1, float& x2, float& y2, float& x3, float& y3) {
		static constexpr float kOneByTan30{ 1.7320508075688772935274463415058723669428052538103806280558069794 };
		static constexpr float kC1{ kOneByTan30 * kParticleRadius };

		x1 = x;
		y1 = y + 2 * kParticleRadius;
		x2 = x + kC1;
		y2 = y - kParticleRadius;
		x3 = x - kC1;
		y3 = y - kParticleRadius;
	}

	static void assignVertexToVb(int triangle_num, int vertex_num, const float& x, const float& y, const float& cx, const float& cy, const float& r, const float& g, const float& b) {
		vertex_buffer[triangle_num * kFloatsPerParticle + vertex_num * kFloatsPerVertex + 0] = x;
		vertex_buffer[triangle_num * kFloatsPerParticle + vertex_num * kFloatsPerVertex + 1] = y;
		vertex_buffer[triangle_num * kFloatsPerParticle + vertex_num * kFloatsPerVertex + 2] = cx;
		vertex_buffer[triangle_num * kFloatsPerParticle + vertex_num * kFloatsPerVertex + 3] = cy;
		vertex_buffer[triangle_num * kFloatsPerParticle + vertex_num * kFloatsPerVertex + 4] = r;
		vertex_buffer[triangle_num * kFloatsPerParticle + vertex_num * kFloatsPerVertex + 5] = g;
		vertex_buffer[triangle_num * kFloatsPerParticle + vertex_num * kFloatsPerVertex + 6] = b;
	}

	void populatePositions() {
		for (int i = 0; i < kNumParticles; i++) {
			velocities[i * 2 + 0] = random(-0.001, 0.001);
			velocities[i * 2 + 1] = random(-0.01, 0.01);

			float x = random(-1, 1);
			float y = random(-1, 1);

			float x1{}, y1{}, x2{}, y2{}, x3{}, y3{};
			calculateTriangleFromCentroid(x, y, x1, y1, x2, y2, x3, y3);

			float r = random(0, 1);
			float g = random(0, 1);
			float b = random(0, 1);

			assignVertexToVb(i, 0, x1, y1, x, y, r, g, b);
			assignVertexToVb(i, 1, x2, y2, x, y, r, g, b);
			assignVertexToVb(i, 2, x3, y3, x, y, r, g, b);
		}
	}

	void updatePositions() {
		for (int i = 0; i < kNumParticles; i++) {
			velocities[i * 2 + 1] += -9.8 * 1 / 200 * 1 / 100;
			float x = vertex_buffer[i * 21 + 2] + velocities[i * 2];
			float y = vertex_buffer[i * 21 + 3] + velocities[i * 2 + 1];

			if ((x < -1) || (x > 1)) {
				velocities[2 * i] *= -0.8;
			}
			if ((y < -1) || (y > 1)) {
				y = -1;
				velocities[2 * i + 1] *= -0.8;
				velocities[2 * i] *= 0.9;
			}

			float r = vertex_buffer[i * kFloatsPerParticle + 4];
			float g = vertex_buffer[i * kFloatsPerParticle + 5];
			float b = vertex_buffer[i * kFloatsPerParticle + 6];

			float x1{}, y1{}, x2{}, y2{}, x3{}, y3{};
			calculateTriangleFromCentroid(x, y, x1, y1, x2, y2, x3, y3);
			assignVertexToVb(i, 0, x1, y1, x, y, r, g, b);
			assignVertexToVb(i, 1, x2, y2, x, y, r, g, b);
			assignVertexToVb(i, 2, x3, y3, x, y, r, g, b);
		}
	}

	std::chrono::high_resolution_clock::time_point start_time{ std::chrono::high_resolution_clock::now() };
	VertexBuffer vb{ kTotalFloats * sizeof(float) };
	Shader shader{};
};
