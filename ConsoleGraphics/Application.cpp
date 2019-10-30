#include "Application.h"
#include <iostream>
namespace cg
{
	Application::Application(Vec2u resolution, Vec2u fontSize) :
		m_console{ std::move(resolution), std::move(fontSize) },
		m_surface{ std::move(resolution) },
		m_eventManager{ m_console.getInputHandle() }
	{
		/*if (m_console.isOpen())
		{
			m_eventManager.setHandle(m_console.getInputHandle());
		}*/
	}

	void Application::start()
	{
		//using namespace std::chrono;

		setup();

		Clock clock;
		//static auto frames = 0;
		//auto ts = std::chrono::high_resolution_clock::now();
		
		auto draw_thread = std::thread{
			[this]() {
				while (m_console.isOpen()) {
					if (m_surfaceReady == 2)
					{
						m_surfaceReady = 0;
						draw();
						m_surfaceReady = 1;
					}
				}
			}
		};

		while (m_console.isOpen())
		{
			Event e;
			while (m_eventManager.pollEvent(e))
				processEvent(e);
			
			update(clock.restart());
	
			//draw();
			
			if (m_surfaceReady == 1)
			{
				bool _ = m_console.display(m_surface.getBuffer());
				m_surfaceReady = 2;
			}

			//auto end = high_resolution_clock::now();
			//auto secs = duration_cast<milliseconds>(end - ts);
			//frames++;
			//if (secs > 10s)
			//{
			//	std::cout << frames << " frames rendered\n";
			//	m_stopAll = true;
			//	m_console.close();
			//	draw_thread.join();
			//	//display_thread.join();
			//	return;
			//}
		}
	}

} // namespace cg
