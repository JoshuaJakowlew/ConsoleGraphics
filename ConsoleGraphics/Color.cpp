#include "Color.h"

namespace cg
{
	Color::Color() noexcept :
		Color(L' ', 0, 0)
	{}

	Color::Color(CHAR_INFO color) noexcept :
		Color(
			color.Char.UnicodeChar,
			Color::getBgColor(color.Attributes),
			Color::getFgColor(color.Attributes)
		)
	{}

	Color::Color(wchar_t glyph, uint16_t color) noexcept :
		Color(
			glyph,
			Color::getBgColor(color),
			Color::getFgColor(color)
		)
	{}

	Color::Color(wchar_t glyph, uint16_t bgColor, uint16_t fgColor) noexcept :
		glyph(glyph),
		bgColor(bgColor << 4),
		fgColor(fgColor)
	{}
} // namespace cg