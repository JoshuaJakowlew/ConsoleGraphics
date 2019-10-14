#include "RenderSurface.h"

namespace cg
{
	auto RenderSurface::drawLine(int x0, int y0, int x1, int y1, CHAR_INFO color) -> void
	{
		bool steep = false;
		if (std::abs(x0 - x1) < std::abs(y0 - y1))
		{
			std::swap(x0, y0);
			std::swap(x1, y1);
			steep = true;
		}
		if (x0 > x1) {
			std::swap(x0, x1);
			std::swap(y0, y1);
		}

		int dx = x1 - x0;
		int dy = y1 - y0;

		int derror2 = std::abs(dy) * 2;
		int error2 = 0;

		int y = y0;
		for (int x = x0; x <= x1; x++)
		{
			if (steep) putCell(y, x, color);
			else putCell(x, y, color);

			error2 += derror2;

			if (error2 > dx)
			{
				y += ((y1 > y0) ? 1 : -1);
				error2 -= dx * 2;
			}
		}
	}

	auto RenderSurface::drawRect(int x, int y, int width, int height, CHAR_INFO color) -> void
	{
		if (width > 0) width = width - 1;
		if (height > 0) height = height - 1;

		if (width < 0) width = width + 1;
		if (height < 0) height = height + 1;

		drawLine(x, y, x + width, y, color);
		drawLine(x + width, y, x + width, y + height, color);
		drawLine(x + width, y + height, x, y + height, color);
		drawLine(x, y + height, x, y, color);
	}

	auto RenderSurface::fillRect(int x, int y, int width, int height, CHAR_INFO color) -> void
	{
		if (width < 0)
		{
			x = x + width + 1;
			width = std::abs(width) + 1;
		}

		if (height < 0)
		{
			y = y + height + 1;
			height = std::abs(height) + 1;
		}

		for (int ypos = y; ypos < height + y; ++ypos)
		{
			for (int xpos = x; xpos < width + x; ++xpos)
			{
				putCell(xpos, ypos, color);
			}
		}
	}

	auto RenderSurface::drawString(int x, int y, std::wstring_view str, CHAR_INFO color) -> void
	{
		int endX = x + str.length();
		clamp(x, y, endX, y);

		for (int i = x; i <= endX; ++i)
		{
			color.Char.UnicodeChar = str[i - x];
			putCellUnsafe(i, y, color);
		}
	}

	auto RenderSurface::drawStringAlpha(int x, int y, std::wstring_view str, CHAR_INFO color) -> void
	{
		const size_t offset = y * m_surfaceSize.x + x;

		int endX = x + str.length();
		clamp(x, y, endX, y);

		for (int i = 0; i <= endX - x; ++i)
		{
			auto bgColor = getBgColor(m_surface[offset + i].Attributes);
			auto fgColor = getFgColor(color.Attributes);

			m_surface[offset + i] = makeCharInfo(str[i], bgColor, fgColor);
		}
	}

	auto RenderSurface::drawSprite(const Sprite& sprite) -> void
	{
		for (unsigned y = 0; y < sprite.getSize().y; ++y)
		{
			const auto line_offset = sprite.getData() + y * sprite.getSize().x;
			for (unsigned x = 0; x < sprite.getSize().x; ++x)
			{
				const auto pos = sprite.getActualPos() + Vec2i{static_cast<int>(x), static_cast<int>(y)};
				const auto cellptr = line_offset + x;

				if (sprite.isTransparent() &&
					cellptr->Attributes == sprite.getTransparentColor().Attributes &&
					cellptr->Char.UnicodeChar == sprite.getTransparentColor().Char.UnicodeChar)
					continue;
				putCell(pos.x, pos.y, *cellptr);
			}
		}
	}

} // namespace cg