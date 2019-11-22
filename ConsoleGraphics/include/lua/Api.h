#ifndef API_H
#define API_H

#include <thirdparty/sol.hpp>

#include <utils/Clock.h>
#include <rendering/Sprite.h>

namespace cg::lua::detail
{
	template <typename T>
	void Vec2(const std::string& name, sol::state& lua)
	{
		#pragma region TypeDefinition
		auto type = lua.new_usertype<cg::Vec2<T>>(
			name,
			sol::constructors<cg::Vec2<T>(), cg::Vec2<T>(T, T)>(),
			
			sol::meta_function::unary_minus,
			[](const cg::Vec2<T>& right)
			{
				return -right;
			},

			sol::meta_function::addition,
			[](const cg::Vec2<T>& left, const cg::Vec2<T>& right)
			{
				return left + right;
			},

			sol::meta_function::subtraction,
			[](const cg::Vec2<T>& left, const cg::Vec2<T>& right)
			{
				return left - right;
			},

			sol::meta_function::multiplication,
			[](const cg::Vec2<T>& left, T right)
			{
				return left * right;
			},

			sol::meta_function::division,
			[](const cg::Vec2<T>& left, T right)
			{
				return left / right;
			});
		#pragma endregion
		
		type["x"] = sol::property(
			[](const cg::Vec2<T>& vec) { return vec.x; },
			[](cg::Vec2<T>& vec, T val) { vec.x = val; });

		type["y"] = sol::property(
			[](const cg::Vec2<T>& vec) { return vec.y; },
			[](cg::Vec2<T>& vec, T val) { vec.y = val; });
	}

	void Clock(const std::string& name, sol::state& lua)
	{
		auto type = lua.new_usertype<cg::Clock>(
			name,
			sol::constructors<cg::Clock()>());

		type["getElapsed"] = &cg::Clock::getElapsed;

		type["restart"] = &cg::Clock::restart;
	}

	void Texture(const std::string& name, sol::state& lua)
	{
		auto type = lua.new_usertype<cg::Texture>(
			name,
			sol::constructors<cg::Texture(), cg::Texture(std::string_view)>());

		type["getSize"] = &cg::Texture::getSize;

		type["loadFromBitmap"] = &cg::Texture::loadFromBitmap;
	}

	void Color(const std::string& name, sol::state& lua)
	{
		lua["makeColor"] = sol::overload(
			[]()
			{
				return cg::Color{};
			},
			[](CHAR_INFO color)
			{
				return cg::Color{ color };
			},
			[](uint16_t glyph, uint16_t color)
			{
				return cg::Color{
					glyph,
					color
				};
			},
			[](uint16_t glyph, uint16_t bgColor, uint16_t fgColor)
			{
				return cg::Color{
					static_cast<wchar_t>(glyph),
					bgColor,
					fgColor
				};
			});

		auto type = lua.new_usertype<cg::Color>(
			name,
			sol::constructors<cg::Color()>());

		type["glyph"] = sol::property(
			[](const cg::Color& color)
			{
				return color.glyph;
			},
			[](cg::Color& color, uint16_t glyph)
			{
				color.glyph = static_cast<wchar_t>(glyph);
			});

		type["bgColor"] = sol::property(
			[](const cg::Color& color)
			{
				return color.bgColor;
			},
			[](cg::Color& color, uint16_t bgColor)
			{
				color.bgColor = bgColor;
			});

		type["fgColor"] = sol::property(
			[](const cg::Color& color)
			{
				return color.fgColor;
			},
			[](cg::Color& color, uint16_t fgColor)
			{
				color.fgColor = fgColor;
			});

		type["toCharInfo"] = &cg::Color::toCharInfo;

		lua["makeCharInfo"] = [](uint16_t glyph, uint16_t bgColor, uint16_t fgColor)
		{
			return cg::makeCharInfo(
				static_cast<wchar_t>(glyph),
				bgColor,
				fgColor);
		};

		lua["combine"] = &cg::combine;

		lua["getBgColor"] = &cg::getBgColor;

		lua["getFgColor"] = &cg::getFgColor;
	}

	void Sprite(const std::string& name, sol::state& lua)
	{
		auto type = lua.new_usertype<cg::Sprite>(
			name,
			sol::constructors<
				cg::Sprite(const cg::Texture&),
				cg::Sprite(const cg::Texture&, Vec2u, Vec2u)
			>(),
			
			sol::meta_function::index,
			&cg::Sprite::operator[]);

		type["getPos"] = &cg::Sprite::getPos;
		type["getActualPos"] = &cg::Sprite::getActualPos;
		type["getSize"] = &cg::Sprite::getSize;
		type["getOrigin"] = &cg::Sprite::getOrigin;

		type["at"] = sol::overload(
			[](const cg::Sprite& sprite, Vec2u pos)
			{
				return sprite.at(pos);
			},
			[](const cg::Sprite& sprite, unsigned x, unsigned y)
			{
				return sprite.at(x, y);
			});

		type["setTexture"] = sol::overload(
			[](cg::Sprite& sprite,
				const cg::Texture* texture, Vec2u left, Vec2u right)
			{
				sprite.setTexture(texture, left, right);
			},
			[](cg::Sprite& sprite, const cg::Texture* texture)
			{
				sprite.setTexture(texture);
			});

		type["setTextureRect"] = &cg::Sprite::setTextureRect;

		type["setPos"] = &cg::Sprite::setPos;

		type["setOrigin"] = &cg::Sprite::setOrigin;
		
		type["move"] = sol::overload(
			[](cg::Sprite& sprite, Vec2f offset)
			{
				sprite.move(offset);
			},
			[](cg::Sprite& sprite, Vec2i offset)
			{
				sprite.move(offset);
			});

		type["isTransparent"] = &cg::Sprite::isTransparent;

		type["setTransparent"] = sol::overload(
			[](cg::Sprite& sprite, bool transparent)
			{
				sprite.setTransparent(transparent);
			},
			[](cg::Sprite& sprite, bool transparent, CHAR_INFO color)
			{
				sprite.setTransparent(transparent, color);
			});

		type["getTransparentColor"] = &cg::Sprite::getTransparentColor;
	}
}

namespace cg::lua::type
{
	template <typename T = void>
	constexpr auto name = {};

	template <>
	constexpr auto name<cg::Vec2i> = "Vec2i";

	template <>
	constexpr auto name<cg::Vec2u> = "Vec2u";

	template <>
	constexpr auto name<cg::Vec2f> = "Vec2f";

	template <>
	constexpr auto name<cg::Clock> = "Clock";

	template <>
	constexpr auto name<cg::Texture> = "Texture";

	template <>
	constexpr auto name<cg::Color> = "Color";

	template <>
	constexpr auto name<cg::Sprite> = "Sprite";
}

namespace cg::lua
{
	void Vec2(sol::state& lua)
	{
		detail::Vec2<int>(type::name<cg::Vec2<int>>, lua);
		detail::Vec2<unsigned>(type::name<cg::Vec2<unsigned>>, lua);
		detail::Vec2<float>(type::name<cg::Vec2<float>>, lua);
	}

	void Clock(sol::state& lua)
	{
		detail::Clock(type::name<cg::Clock>, lua);
	}

	void Texture(sol::state& lua)
	{
		detail::Texture(type::name<cg::Texture>, lua);
	}

	void Color(sol::state& lua)
	{
		detail::Color(type::name<cg::Color>, lua);
	}

	void Sprite(sol::state& lua)
	{
		detail::Sprite(type::name<cg::Sprite>, lua);
	}

} // namespace cg::lua

#endif
