#include <lua/LuaApplication.h>

namespace cg
{
	LuaApplication::LuaApplication(Vec2u resolution, Vec2u fontSize) :
		cg::Application{ resolution, fontSize }
	{
		loadApi();
		runLua();
		setHandlers();
	}

	void LuaApplication::setup() noexcept
	{
		std::lock_guard<std::mutex> lock{ m_lock };
		m_setupHandler();
	}

	void LuaApplication::processEvent(const cg::Event& e) noexcept
	{
		std::lock_guard<std::mutex> lock{ m_lock };
		m_processEventHandler(e);
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

} // namespace cg
