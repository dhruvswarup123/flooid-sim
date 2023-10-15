#pragma once

#include <array>
#include <stdint.h>
#include <mutex>

#include "constants.h"

struct Particle {
	double position[2];
	double velocity[2];
	uint8_t color[3];    // 0-255 RGB
};

class Particles {
public:
	void lock() {
		mutex_.lock();
	}

	void unlock() {
		mutex_.unlock();
	}

	Particle operator [] (int i) const { return data_[i]; }
	Particle& operator [] (int i) { return data_[i]; }

private:
	std::array<Particle, kNumParticles> data_{};
	std::mutex mutex_{};
};
