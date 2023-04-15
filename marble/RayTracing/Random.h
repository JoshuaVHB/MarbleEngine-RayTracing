#pragma once


#include <random>
#include <chrono>

namespace Random {

	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	static float Float()
	{
		return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}
	static uint32_t UInt()
	{
		return s_Distribution(s_RandomEngine);
	}

}