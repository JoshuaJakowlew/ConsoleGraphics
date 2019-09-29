#include "Texture.h"

namespace cg
{
	void Texture::setPalette(const Palette& palette)
	{
		m_palette = convertToInternalPalette(palette);
	}
	
	bool Texture::loadFromBitmap(std::string_view path)
	{
		bitmap_image bitmap(path.data());
		
		[[unlikely]]
		if (!bitmap.data())
			return false;
		
		m_size = { bitmap.width(), bitmap.height() };

		std::vector<CHAR_INFO> local_data;
		if (bool success = convertBitmapToPalette(bitmap, local_data);
			success)
		{
			m_data = std::move(local_data);
		}
	}

	std::array<rgb_t, 16> Texture::convertToInternalPalette(Palette external)
	{
		auto internal = std::array<rgb_t, 16>{};
		for (int i = 0; i < 16; ++i)
		{
			internal[i] = rgb_t{ GetRValue(external[i]), GetGValue(external[i]), GetBValue(external[i]) };
		}
		return internal;
	}

	bool Texture::convertBitmapToPalette(const bitmap_image& bitmap, std::vector<CHAR_INFO>& converted_data)
	{
		if(converted_data.capacity() < bitmap.pixel_count())
			converted_data.reserve(bitmap.pixel_count());

		for (size_t y = 0; y < bitmap.height(); ++y)
		{
			for (size_t x = 0; x < bitmap.width(); ++x)
			{
				if (auto[index, success] = convertColorToPalette(bitmap.get_pixel(x, y));
					success)
				{
					converted_data.emplace_back(makeCharInfo(L' ', index, 0));
				}
				else return false;
			}
		}

		return true;
	}

	std::tuple<size_t, bool> Texture::convertColorToPalette(rgb_t color)
	{
		color = find_nearest_color(color, m_palette.begin(), m_palette.end());
		if (auto it = std::find(m_palette.begin(), m_palette.end(), color);
			it != m_palette.end())
		{
			return { it - m_palette.begin(), true };
		}

		return { 0, false };
	}

} // namespace cg