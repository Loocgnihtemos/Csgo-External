#include "gui.h"
#include "hacks.h"
#include "globals.h"

#include <thread>



LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


int __stdcall wWinMain(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	PWSTR arguments,
	int commandShow)
{
	Memory mem{ "csgo.exe" };

	globals::clientaddress = mem.GetModuleAddress("client.dll");
	globals::engineaddress = mem.GetModuleAddress("engine.dll");

	std::thread(hacks::VisualsThread, mem).detach();
	std::thread(hacks::MiscThread, mem).detach();
	
	// create gui
	gui::CreateHWindow("Cheat Menu");
	gui::CreateDevice();
	gui::CreateImGui();

	while (gui::isRunning)
	{
		gui::BeginRender();
		gui::Render();
		gui::EndRender();

		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	// destroy gui
	gui::DestroyImGui();
	gui::DestroyDevice();
	gui::DestroyHWindow();

	return EXIT_SUCCESS;
}
