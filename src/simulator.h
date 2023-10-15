#pragma once

#include <chrono>
#include <mutex>

#include "constants.h"
#include "particles.h"


// This must be constructed before the first access to particles
class Simulator {
using hrclock = std::chrono::high_resolution_clock;
public:
	Simulator();

	void main();

	void loop();

	static Particles particles;

private:
	hrclock::time_point prev_time_{};
	bool stop_loop_{};
};
