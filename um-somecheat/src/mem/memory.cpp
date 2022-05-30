#include "memory.h"
#include <TlHelp32.h>

void memory::process(const std::string name) noexcept {
	auto entry = PROCESSENTRY32{ };
	entry.dwSize = sizeof(PROCESSENTRY32);

	const auto snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	// loop through processes
	do {
		if (!name.compare(entry.szExeFile)) {
			memory::id = entry.th32ProcessID;
			break;
		}
	} while (Process32Next(snap, &entry));

	if (snap) {
		CloseHandle(snap);
	}
}

std::uintptr_t memory::module_address(const std::string name) noexcept {
	auto entry = MODULEENTRY32{ };
	entry.dwSize = sizeof(MODULEENTRY32);

	const auto snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, id);

	std::uintptr_t addr = 0;

	// loop through modules
	do {
		if (!name.compare(entry.szModule)) {
			addr = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
			break;
		}
	} while (Module32Next(snap, &entry));

	if (snap) {
		CloseHandle(snap);
	}

	return addr;
}

bool memory::open_handle() noexcept {
	return memory::handle = OpenProcess(PROCESS_ALL_ACCESS, 0, memory::id);
}

void memory::close_handle() noexcept {
	if (memory::handle) {
		CloseHandle(memory::handle);
	}
}
