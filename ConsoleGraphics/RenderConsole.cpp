#include "RenderConsole.h"

namespace cg
{
	bool RenderConsole::display() noexcept
	{
		assert(m_resolution.x <= SHRT_MAX - 1 && m_resolution.y <= SHRT_MAX - 1);
		assert(m_handles.out != INVALID_HANDLE_VALUE);

		static const COORD screenSize = {
			static_cast<SHORT>(m_resolution.x),
			static_cast<SHORT>(m_resolution.y)
		};

		static const SMALL_RECT writeRegion = {
			0,
			0,
			static_cast<SHORT>(m_resolution.x - 1),
			static_cast<SHORT>(m_resolution.y - 1)
		};

		if (!m_isOpen)
			return false;

		const auto buffer = getBuffer();
		assert(buffer);

		auto writtenRegion = writeRegion;

		const auto writeResult = ::WriteConsoleOutputW(m_handles.out, buffer, screenSize, { 0, 0 }, &writtenRegion);

		return
			(writtenRegion.Left == writeRegion.Left) &
			(writtenRegion.Right == writeRegion.Right) &
			(writtenRegion.Bottom == writeRegion.Bottom) &
			(writtenRegion.Top == writeRegion.Top) &
			writeResult;
	}

} // namespace cg
