#include "Texture.h"

namespace cg
{
	bool Texture::loadFromBitmap(std::string_view path)
	{
		bitmap_image bitmap{ path.data() };
		
		[[unlikely]]
		if (!bitmap.data())
			return false;
		
		m_size = { bitmap.width(), bitmap.height() };
		m_data = convertBitmapToPalette(bitmap);
		
		return true;
	}

	Texture::InternalPalette Texture::convertToInternalPalette(const Palette& external) noexcept
	{
		auto internal = InternalPalette{};

		std::transform(std::begin(external), std::end(external), std::begin(internal), [](auto e) {
			return rgb_t{ GetRValue(e), GetGValue(e), GetBValue(e)};
		});

		return internal;
	}

	std::vector<CHAR_INFO> Texture::convertBitmapToPalette(const bitmap_image& bitmap)
	{
		std::vector<CHAR_INFO> converted_data;

		if(converted_data.capacity() < bitmap.pixel_count())
			converted_data.reserve(bitmap.pixel_count());

		for (size_t y = 0; y < bitmap.height(); ++y)
		{
			for (size_t x = 0; x < bitmap.width(); ++x)
			{
				auto index = convertColorToPalette(bitmap.get_pixel(x, y));
				converted_data.emplace_back(makeCharInfo(L' ', index, 0));
			}
		}

		return converted_data;
	}

	size_t Texture::convertColorToPalette(rgb_t color) const noexcept
	{
		color = find_nearest_color(color, m_palette.begin(), m_palette.end());

		auto it = std::find(m_palette.begin(), m_palette.end(), color);
		return it - m_palette.begin();
	}

} // namespace cg