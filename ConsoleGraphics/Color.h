#ifndef COLOR_H
#define COLOR_H

#include <cstdint>

#include <Windows.h>

namespace cg
{
	struct Color
	{
		enum Palette : uint16_t
		{
			Black = 0x0000,
			DarkBlue = 0x0001,
			DarkGreen = 0x0002,
			DarkCyan = 0x0003,
			DarkRed = 0x0004,
			DarkMagenta = 0x0005,
			DarkYellow = 0x0006,
			Gray = 0x0007, // Thanks, MS
			DarkGray = 0x0008,
			Blue = 0x0009,
			Green = 0x000A,
			Cyan = 0x000B,
			Red = 0x000C,
			Magenta = 0x000D,
			Yellow = 0x000E,
			White = 0x000F
		};

		constexpr Color() noexcept;
		constexpr Color(CHAR_INFO color) noexcept;
		constexpr Color(wchar_t glyph, uint16_t color) noexcept;
		constexpr Color(wchar_t glyph, uint16_t bgColor, uint16_t fgColor) noexcept;

		[[nodiscard]] constexpr auto toCharInfo() const noexcept -> CHAR_INFO;

		wchar_t glyph;
		uint16_t bgColor;
		uint16_t fgColor;
	};

	[[nodiscard]] constexpr auto makeCharInfo(wchar_t glyph, uint16_t bgColor, uint16_t fgColor) noexcept -> CHAR_INFO
	{
		return { glyph, static_cast<WORD>(fgColor | bgColor << 4) };
	}

	[[nodiscard]] constexpr auto combine(uint16_t bgColor, uint16_t fgColor) noexcept -> uint16_t
	{
		return (bgColor << 4) | fgColor;
	}

	[[nodiscard]] constexpr auto getBgColor(uint16_t color) noexcept -> uint16_t
	{
		return color >> 4;
	}

	[[nodiscard]] constexpr auto getFgColor(uint16_t color) noexcept -> uint16_t
	{
		return color & 0xF;
	}

	constexpr Color::Color() noexcept :
		Color(L' ', 0, 0)
	{}	
	
	constexpr Color::Color(CHAR_INFO color) noexcept :
		Color(
			color.Char.UnicodeChar,
			getBgColor(color.Attributes),
			getFgColor(color.Attributes)
		)
	{}	
	
	constexpr Color::Color(wchar_t glyph, uint16_t color) noexcept :
		Color(
			glyph,
			getBgColor(color),
			getFgColor(color)
		)
	{}	
	
	constexpr Color::Color(wchar_t glyph, uint16_t bgColor, uint16_t fgColor) noexcept :
		glyph(glyph),
		bgColor(bgColor << 4),
		fgColor(fgColor)
	{}

	constexpr auto Color::toCharInfo() const noexcept -> CHAR_INFO
	{
		return { glyph, static_cast<WORD>(fgColor | bgColor) };
	}

} // namespace cg

#endif
