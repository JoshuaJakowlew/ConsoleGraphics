#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>

namespace cg
{
	class Clock
	{
	public:
		inline Clock() noexcept;

		[[nodiscard]] inline auto getElapsed() const noexcept -> float;
		[[nodiscard]] inline auto restart() noexcept -> float;

	private:
		using clock_t = std::chrono::high_resolution_clock;
		using time_t = std::chrono::time_point<clock_t>;
		clock_t m_clock;
		time_t m_time;
	};

	inline Clock::Clock() noexcept
	{
		m_time = m_clock.now();
	}

	[[nodiscard]] inline auto Clock::getElapsed() const noexcept -> float
	{
		const std::chrono::duration<float> elapsed = m_clock.now() - m_time;
		return elapsed.count();
	}

	[[nodiscard]] inline auto Clock::restart() noexcept -> float
	{
		const auto elapsed = getElapsed();
		m_time = m_time = m_clock.now();
		return elapsed;
	}

} // namespace cg

#endif
