#include <lua/Api.h>

namespace cg::lua::detail
{
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

	void RenderSurface(const std::string& name, sol::state& lua)
	{
		auto type = lua.new_usertype<cg::RenderSurface>(
			name,
			sol::constructors<
			cg::RenderSurface(cg::Vec2u),
			cg::RenderSurface(size_t, size_t)
			>());

		type["getSize"] = &cg::RenderSurface::getSize;

		type["putCell"] = &cg::RenderSurface::putCell;
		type["drawLine"] = &cg::RenderSurface::drawLine;
		type["drawRect"] = &cg::RenderSurface::drawRect;
		type["fill"] = &cg::RenderSurface::fill;
		type["fillRect"] = &cg::RenderSurface::fillRect;
		type["drawString"] = &cg::RenderSurface::drawString;
		type["drawStringAlpha"] = &cg::RenderSurface::drawStringAlpha;
		type["drawSprite"] = &cg::RenderSurface::drawSprite;
	}

} // namespace cg::lua::detail
