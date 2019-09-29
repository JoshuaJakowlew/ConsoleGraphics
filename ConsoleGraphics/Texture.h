#ifndef TEXTURE_H
#define TEXTURE_H

#include <fstream>
#include <string_view>
#include <vector>
#include <array>
#include <algorithm>
#include <tuple>
#include <cassert>

#include <Windows.h>

#include "bitmap_image.hpp"
#include "Vec2.h"
#include "Color.h"

namespace cg
{
	class Texture
	{
	public:
		Texture(std::array<COLORREF, 16> palette) :
			m_palette(convertToInternalPalette(palette))
		{}

		[[nodiscard]] inline Vec2u getSize() const noexcept;

		[[nodiscard]] inline CHAR_INFO& getCell(Vec2i coords);
		[[nodiscard]] inline const CHAR_INFO * getLine(int line);
		[[nodiscard]] inline const CHAR_INFO * getBuffer() const;

		void setPalette(const std::array<COLORREF, 16>& palette);
		[[nodiscard]] bool loadFromBitmap(std::string_view path);	
	private:
		std::array<rgb_t, 16> m_palette;
		mutable std::vector<CHAR_INFO> m_data;
		Vec2u m_size;

		[[nodiscard]] std::array<rgb_t, 16> convertToInternalPalette(std::array<COLORREF, 16> external);

		[[nodiscard]] bool convertBitmapToPalette(const bitmap_image& bitmap, std::vector<CHAR_INFO>& converted_data);
		[[nodiscard]] std::tuple<size_t, bool> convertColorToPalette(rgb_t color);
	};

	inline Vec2u Texture::getSize() const noexcept
	{
		return m_size;
	}

	inline CHAR_INFO& Texture::getCell(Vec2i coords)
	{
		assert(
			coords.x >= 0 &&
			coords.y >= 0 &&
			coords.x < static_cast<long long>(m_size.x) &&
			coords.y < static_cast<long long>(m_size.y)
		);
		return m_data[m_size.x * coords.y + coords.x];
	}

	const CHAR_INFO * Texture::getLine(int line)
	{
		assert(
			line >= 0 &&
			line < static_cast<long long>(m_size.y)
		);
		assert(&m_data[m_size.x * line] != nullptr);
		return &m_data[m_size.x * line];
	}

	const CHAR_INFO * Texture::getBuffer() const
	{
		assert(&m_data[0] != nullptr);
		return &m_data[0];
	}

} // namespace cg

#endif