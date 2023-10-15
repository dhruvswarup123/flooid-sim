#include "simulator.h"
#include <cassert>

Particles Simulator::particles = Particles{};

Simulator::Simulator() {
	assert(kGridWidth * kGridHeight == kNumParticles);

	// Create a uniform grid for predictability
	{
		const std::lock_guard<Particles> lg{ particles };

		for (int i = 0; i < kGridHeight; i++) {
			for (int j = 0; j < kGridWidth; j++) {
				const int index{ i * kGridWidth + j };
				particles[index].position[0] = kGridBottomLeftX + j * kGridStepSize;
				particles[index].position[1] = kGridBottomLeftY + i * kGridStepSize;
				particles[index].velocity[0] = 0;
				particles[index].velocity[1] = 0;
				particles[index].color[0] = 52;
				particles[index].color[1] = 163;
				particles[index].color[2] = 203; 
			}
		}
	}
}


void Simulator::loop() {
	static constexpr double kDeltaT{ kSimUpdatePeriodMs / 1000.0 };
	static constexpr double kDeltaT2{ kDeltaT  * kDeltaT };
	// Create a uniform grid for predictability
	{
		const std::lock_guard<Particles> lg{ particles };

		for (int i = 0; i < kGridHeight; i++) {
			for (int j = 0; j < kGridWidth; j++) {
				const int index{ i * kGridWidth + j };
				particles[index].position[0] += particles[index].velocity[0] * kDeltaT;
				particles[index].position[1] += particles[index].velocity[1] * kDeltaT + 0.5 * (-3000) * kDeltaT2;

				//particles[index].velocity[0] += (-30000) * kDeltaT;
				particles[index].velocity[1] += (-3000) * kDeltaT;

				if (particles[index].position[1] < 0) {
					particles[index].position[1] = 0;
					particles[index].velocity[1] *= -0.8;
				}
			}
		}
	}
}

void Simulator::main() {
	while (!stop_loop_) {
		// Get time and duation
		auto curr_time = hrclock::now();
		auto duration = duration_cast<std::chrono::milliseconds>(curr_time - prev_time_);

		// Exec main loop every kSimUpdatePeriodMs
		if (duration >= std::chrono::milliseconds(kSimUpdatePeriodMs)) {
			loop();
			prev_time_ = curr_time;
		}
	}
}
