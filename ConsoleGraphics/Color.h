#ifndef COLOR_H
#define COLOR_H

#include <cstdint>

#include <Windows.h>

namespace cg
{
	struct Color
	{
		enum Palette : uint32_t
		{
			Black = 0x0000,
			DarkBlue = 0x0001,
			DarkGreen = 0x0002,
			DarkCyan = 0x0003,
			DarkRed = 0x0004,
			DarkMagenta = 0x0005,
			DarkYellow = 0x0006,
			Gray = 0x0007, // Thanks, MS
			DarkGrey = 0x0008,
			Blue = 0x0009,
			Green = 0x000A,
			Cyan = 0x000B,
			Red = 0x000C,
			Magenta = 0x000D,
			Yellow = 0x000E,
			White = 0x000F
		};

		Color() noexcept;
		Color(CHAR_INFO color) noexcept;
		Color(wchar_t glyph, uint16_t color) noexcept;
		Color(wchar_t glyph, uint16_t bgColor, uint16_t fgColor) noexcept;

		inline CHAR_INFO toCharInfo() const noexcept;

		inline static CHAR_INFO makeCharInfo(wchar_t glyph, uint16_t bgColor, uint16_t fgColor) noexcept;
		inline static uint16_t combine(uint16_t bgColor, uint16_t fgColor) noexcept;
		inline static uint16_t getBgColor(uint16_t color) noexcept;
		inline static uint16_t getFgColor(uint16_t color) noexcept;

		wchar_t glyph;
		uint16_t bgColor;
		uint16_t fgColor;
	};

	inline CHAR_INFO Color::toCharInfo() const noexcept
	{
		CHAR_INFO ci;
		ci.Char.UnicodeChar = glyph;
		ci.Attributes = fgColor | bgColor;
		return ci;
	}

	inline CHAR_INFO Color::makeCharInfo(wchar_t glyph, uint16_t bgColor, uint16_t fgColor) noexcept
	{
		CHAR_INFO ci;
		ci.Char.UnicodeChar = glyph;
		ci.Attributes = fgColor | bgColor << 4;
		return ci;
	}

	inline uint16_t Color::combine(uint16_t bgColor, uint16_t fgColor) noexcept
	{
		return (bgColor << 4) | fgColor;
	}

	inline uint16_t Color::getBgColor(uint16_t color) noexcept
	{
		return color >> 4;
	}

	inline uint16_t Color::getFgColor(uint16_t color) noexcept
	{
		return color & 0xF;
	}
} // namespace cg

#endif
