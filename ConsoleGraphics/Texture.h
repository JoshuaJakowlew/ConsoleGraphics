#ifndef TEXTURE_H
#define TEXTURE_H

#include <fstream>
#include <string_view>
#include <vector>
#include <array>
#include <algorithm>
#include <tuple>
#include <cassert>
#include <optional>

#include "bitmap_image.hpp"
#include "Vec2.h"
#include "Color.h"

namespace cg
{
	class Texture
	{
	public:
		inline Texture(const Palette& palette = palette::defaultPalette);
		inline Texture(std::string_view path, const Palette& palette = palette::defaultPalette);

		[[nodiscard]] inline Vec2u getSize() const noexcept;

		[[nodiscard]] inline CHAR_INFO& getCell(Vec2i coords);
		[[nodiscard]] inline const CHAR_INFO * getLine(int line);
		[[nodiscard]] inline const CHAR_INFO * getBuffer() const;

		inline void setPalette(const Palette& palette);
		[[nodiscard]] bool loadFromBitmap(std::string_view path);	
	private:
		using InternalPalette = std::array<rgb_t, 16>;

		InternalPalette m_palette;
		mutable std::vector<CHAR_INFO> m_data;
		Vec2u m_size;

		[[nodiscard]] static InternalPalette convertToInternalPalette(const Palette& external) noexcept;

		[[nodiscard]] std::vector<CHAR_INFO> convertBitmapToPalette(const bitmap_image& bitmap);
		[[nodiscard]] size_t convertColorToPalette(rgb_t color) const noexcept;
	};

	Texture::Texture(const Palette& palette) :
		m_palette(convertToInternalPalette(palette))
	{}

	Texture::Texture(std::string_view path, const Palette& palette) :
		m_palette(convertToInternalPalette(palette))
	{
		loadFromBitmap(std::move(path));
	}

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

	void Texture::setPalette(const Palette& palette)
	{
		m_palette = convertToInternalPalette(palette);
	}

} // namespace cg

#endif