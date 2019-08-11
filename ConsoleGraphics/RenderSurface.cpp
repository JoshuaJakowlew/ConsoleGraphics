#include "RenderSurface.h"

namespace cg
{
	RenderSurface::RenderSurface(Vec2u size) :
		RenderSurface(size.x, size.y)
	{}

	RenderSurface::RenderSurface(unsigned width, unsigned height) :
		m_surfaceSize{ width, height },
		m_surface{ width * height, makeCharInfo(' ', Color::Black, Color::Gray) }
	{
		m_surface.shrink_to_fit();
	}

	auto RenderSurface::putCellUnsafe(int x, int y, CHAR_INFO color) -> void
	{
		const auto pos = y * m_surfaceSize.x + x;
		m_surface[pos] = color;
	}

	auto RenderSurface::putCell(int x, int y, CHAR_INFO color) -> void
	{
		[[unlikely]]
		if (!on_surface(x, y))
			return;

		putCellUnsafe(x, y, color);
	}

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

	auto RenderSurface::drawSprite(const Sprite & sprite) -> void
	{
		auto left = sprite.getPos();
		auto right = sprite.getPos() + sprite.getSize();
		
		clamp(left.x, right.y);
		clamp(right.x, right.y);

		for (auto y = left.y; y <= right.y; ++y)
		{
			const std::uintptr_t offset = y * sprite.getSize().x;
			const auto srcLine = sprite.m_data + offset;
			const auto dstLine = &m_surface[0] + offset + left.x;
			const auto size = (right.x - left.x + 1) * sizeof(CHAR_INFO);

			std::memcpy(dstLine, srcLine, size);
		}
	}

	auto RenderSurface::fill(CHAR_INFO color) -> void
	{
		std::fill(m_surface.begin(), m_surface.end(), color);
	}

} // namespace cg