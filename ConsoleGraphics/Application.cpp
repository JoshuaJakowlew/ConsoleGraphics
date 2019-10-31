#include <thread>

#include "Application.h"

namespace cg
{
	Application::Application(Vec2u resolution, Vec2u fontSize) :
		m_console{ std::move(resolution), std::move(fontSize) },
		m_surface{ std::move(resolution) },
		m_eventManager{ m_console.getInputHandle() }
	{
	}

	void Application::start()
	{
		setup();
		startDrawThread();
		runMainLoop();
	}

	void Application::startDrawThread()
	{
		auto draw_thread = std::thread{
			[this]() {
				while (m_console.isOpen()) {
					if (m_surfaceReady == RenderState::Displayed)
					{
						m_surfaceReady = RenderState::DrawStart;
						m_surface.fill(CHAR_INFO{ L' ', 0x00 });
						draw();
						m_surfaceReady = RenderState::DrawEnd;
					}
				}
			}
		};

		draw_thread.detach();
	}

	void Application::runMainLoop()
	{
		Clock clock;
		while (m_console.isOpen())
		{
			Event e;
			while (m_eventManager.pollEvent(e))
				processEvent(e);

			update(clock.restart());

			if (m_surfaceReady == RenderState::DrawEnd)
			{
				if(!m_console.display(m_surface.getBuffer()))
					m_console.close();
				m_surfaceReady = RenderState::Displayed;
			}
		}
	}

} // namespace cg
