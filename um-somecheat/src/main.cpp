#include <iostream>
#include <format>
#include <thread>
#include <algorithm>
#include <Windows.h>
#define WIN32_LEAN_AND_MEAN
#include "features/features.h"
#include "mem/memory.h"
#include "utils/xor.h"

using namespace std;


void init()
{
	std::thread{ features::legitbot::aimbot }.detach();
	std::thread{ features::legitbot::triggerbot }.detach();

	std::thread{ features::visuals::fovchanger }.detach();
	std::thread{ features::visuals::noflash }.detach();
	std::thread{ features::visuals::glow }.detach();

	std::thread{ features::misc::bhop }.detach();
	std::thread{ features::misc::autostrafe }.detach();
	std::thread{ features::misc::fakelag }.detach();
}

const int dialog(const std::string& text) noexcept {
	MessageBeep(MB_ICONERROR);
	return MessageBox(
		nullptr ? nullptr : NULL,
		text.c_str(),
		"somecheat",
		MB_ICONERROR | MB_OK
	);
}

int main()
{
	memory::process(xorstr_("csgo.exe"));

	if (memory::id == 0) {
		dialog(xorstr_("Please open cs:go!"));
		return 1;
	}

	// wait for csgo to load modules
	while (!memory::module_address(xorstr_("serverbrowser.dll"))) {
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	memory::client = memory::module_address("client.dll");
	memory::engine = memory::module_address("engine.dll");

	if (memory::client == 0 || memory::engine == 0) {
		cout << (xorstr_("failed to read module address!\n"));
	}

	if (!memory::open_handle()) {
		dialog(xorstr_("Failed to open a handle to the game."));
		return 1;
	}

	init();

	while (vars::run) {
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	memory::close_handle();
	return 0;
}