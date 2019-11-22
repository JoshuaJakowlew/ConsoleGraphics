#ifndef LUA_APPLICATION_H
#define LUA_APPLICATION_H

#include <lua/Api.h>

namespace cg
{
	class LuaApplication : public cg::Application
	{
	public:
		using SetupHandler = std::function<void()>;
		using ProcessEventHandler = std::function<void(const sol::table&)>;
		using UpdateHandler = std::function<void(float)>;
		using DrawHandler = std::function<void(cg::RenderSurface&)>;

		LuaApplication(Vec2u resolution, Vec2u fontSize, std::wstring_view title);

		void setup() noexcept override;

		void processEvent(const cg::Event& e) noexcept override;

		void update(float dt) noexcept override;

		void draw() noexcept override;
	private:
		std::mutex m_lock;

		sol::state m_lua;

		SetupHandler m_setupHandler;
		ProcessEventHandler m_processEventHandler;
		UpdateHandler m_updateHandler;
		DrawHandler m_drawHandler;

		void loadApi();
		void setHandlers();
		void runLua();

		sol::table translateEvent(const Event& e);
	};

} // namespace cg

#endif
