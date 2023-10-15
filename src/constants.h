#pragma once

#include <imgui.h>

static constexpr int kWindowSize[] = { 1280, 720 }; // Width x height
static constexpr ImVec4 kClearColor{ 25 / 255.0f, 65 / 255.0f, 105 / 255.0f, 1.00f };

static constexpr unsigned int kNumParticles{ 1000 };
static constexpr float kParticleRadius{ 0.005 };
static constexpr unsigned int kSimUpdatePeriodMs{ 10 };

// Primitives
static constexpr int kVerticesPerParticle{ 3 }; // Triangle
static constexpr int kFloatsInVertexPosition{ 2 }; // 2D position
static constexpr int kFloatsInCentroid{ 2 }; // 2D centroid
static constexpr int kFloatsInColor{ 3 }; // 3D color

static constexpr int kGridWidth{ 50 };
static constexpr int kGridHeight{ 20 };

static constexpr int kGridStepSize{ 20 };
static constexpr int kGridBottomLeftX{ 100 };
static constexpr int kGridBottomLeftY{ 100 };
