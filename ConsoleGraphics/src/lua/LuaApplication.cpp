#include <lua/LuaApplication.h>

namespace cg
{
	LuaApplication::LuaApplication(Vec2u resolution, Vec2u fontSize, std::wstring_view title) :
		cg::Application{ resolution, fontSize, title }
	{
		loadApi();
		runLua();
		setHandlers();
	}

	void LuaApplication::setup() noexcept
	{
		auto table = m_lua.create_table();
		table["console"] = std::ref(m_console);
		table["render"] = std::ref(m_surface);
		table["gcMaxMem"] = m_GC_MAX_MEM;

		std::lock_guard<std::mutex> lock{ m_lock };
		m_setupHandler(table);
	}

	void LuaApplication::processEvent(const cg::Event& e) noexcept
	{
		auto lua_event = translateEvent(e);

		std::lock_guard<std::mutex> lock{ m_lock };
		m_processEventHandler(lua_event);
	}

	inline void LuaApplication::update(float dt) noexcept
	{
		std::lock_guard<std::mutex> lock{ m_lock };
		m_updateHandler(dt);
	}

	inline void LuaApplication::draw() noexcept
	{
		std::lock_guard<std::mutex> lock{ m_lock };
		m_drawHandler(m_surface);
	}

	void LuaApplication::loadApi()
	{
		lua::Vec2(m_lua);
		lua::Clock(m_lua);
		lua::Texture(m_lua);
		lua::Color(m_lua);
		lua::Sprite(m_lua);
		lua::RenderSurface(m_lua);
		lua::TextureHolder(m_lua);
		lua::SpriteHolder(m_lua);
		lua::FrameAnimation(m_lua);
		lua::FrameAnimator(m_lua);
		lua::Palette(m_lua);
		lua::Console(m_lua);
	}

	void LuaApplication::setHandlers()
	{
		m_setupHandler = m_lua["setup"];
		m_processEventHandler = m_lua["processEvents"];
		m_updateHandler = m_lua["update"];
		m_drawHandler = m_lua["draw"];
	}

	void LuaApplication::runLua()
	{
		m_lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::string);

		m_lua.script_file("assets/scripts/main.lua");
	}

	sol::table LuaApplication::translateEvent(const Event& e)
	{
		auto lua_event = m_lua.create_table();;
		if (EventType::KeyPressed == e.type)
		{
			lua_event["type"] = "KeyPressed";
			lua_event["controlKeyState"] = e.key.controlKeyState;
			lua_event["scanCode"] = e.key.scanCode;
			lua_event["key"] = std::wstring{ e.key.key };
		}
		else if (EventType::KeyReleased == e.type)
		{
			lua_event["type"] = "KeyReleased";
			lua_event["controlKeyState"] = e.key.controlKeyState;
			lua_event["scanCode"] = e.key.scanCode;
			lua_event["key"] = std::wstring{ e.key.key };
		}
		else if (EventType::MouseMove == e.type)
		{
			lua_event["type"] = "MouseMoved";

			const Vec2i position = { e.mouseMove.position.X, e.mouseMove.position.Y };
			lua_event["position"] = position;
		}
		else if (EventType::MouseClick == e.type)
		{
			lua_event["type"] = "MouseMoved";

			const Vec2i position = { e.mouseMove.position.X, e.mouseMove.position.Y };
			lua_event["position"] = position;

			lua_event["mouseButton"] =
				(e.mouseClick.button == MouseButton::Left) ? "Left" : "Right";

			lua_event["doubleClick"] = e.mouseClick.doubleClick;
		}
		else
		{
			lua_event["type"] = std::string{ "Raw" };
			lua_event["data"] = e.raw.data;
		}

		return lua_event;
	}

} // namespace cg
