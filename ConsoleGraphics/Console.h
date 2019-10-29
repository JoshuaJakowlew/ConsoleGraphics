#ifndef CONSOLE_H
#define CONSOLE_H

#include <array>
#include <vector>
#include <queue>
#include <type_traits>
#include <cassert>

#include "Vec2.h"
#include "Color.h"

namespace cg
{
	class Console
	{
	public:
		Console(Vec2u resolution, Vec2u font_size) noexcept;

		Console(const Console&) = delete;
		Console& operator=(const Console&) = delete;

		[[nodiscard]] bool display(const CHAR_INFO* buffer) noexcept;
		[[nodiscard]] bool close() noexcept;

		[[nodiscard]] bool isOpen() const noexcept;

		[[nodiscard]] inline const HANDLE getInputHandle() const noexcept;
		[[nodiscard]] inline const HANDLE getOutputHandle() const noexcept;
		[[nodiscard]] inline Vec2u getResolution() const noexcept;
		[[nodiscard]] inline Vec2u getMaxResolution() const noexcept;
		
		[[nodiscard]] bool setPalette(const cg::Palette& palette) noexcept;

	protected:
		struct Handles
		{
			HANDLE original;
			HANDLE out;
			HANDLE in;
		};

		Handles m_handles;
		bool m_isOpen = false;

		Vec2u m_resolution;
		Vec2u m_maxResolution;
		Vec2u m_fontSize;

		Palette m_palette;

		[[nodiscard]] bool create() noexcept;
		[[nodiscard]] Vec2u getMaxScreenBufferSize() noexcept;
		[[nodiscard]] bool getStdHandles() noexcept;
		[[nodiscard]] bool createScreenBuffer() noexcept;
		[[nodiscard]] bool configureInput() noexcept;
		[[nodiscard]] bool configureOutput() noexcept;
		[[nodiscard]] bool assignScreenBuffer() noexcept;
		[[nodiscard]] bool disableCursor() noexcept;
		[[nodiscard]] bool setFontSize(Vec2u fontSize) noexcept;
	};

	inline bool Console::isOpen() const noexcept
	{
		return m_isOpen;
	}

	inline const HANDLE Console::getInputHandle() const noexcept
	{
		return m_handles.in;
	}

	inline const HANDLE Console::getOutputHandle() const noexcept
	{
		return m_handles.out;
	}

	inline Vec2u Console::getResolution() const noexcept
	{
		return m_resolution;
	}

	inline Vec2u Console::getMaxResolution() const noexcept
	{
		return m_maxResolution;
	}

} // namespace cg

#endif