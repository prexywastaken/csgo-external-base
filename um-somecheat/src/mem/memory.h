#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cstdint>
#include <string>

namespace memory {
	inline int id = 0;
	inline void* handle = nullptr;

	inline std::uintptr_t client = 0;
	inline std::uintptr_t engine = 0;

	void process(const std::string name) noexcept;

	std::uintptr_t module_address(const std::string name) noexcept;

	bool open_handle() noexcept;
	void close_handle() noexcept;

	template <typename T>
	constexpr T read(const std::uintptr_t& addr) noexcept {
		T value;
		ReadProcessMemory(handle, reinterpret_cast<const void*>(addr), &value, sizeof(T), 0);
		return value;
	}

	template <typename T>
	constexpr void write(const std::uintptr_t& addr, const T& value) noexcept {
		WriteProcessMemory(handle, reinterpret_cast<void*>(addr), &value, sizeof(T), 0);
	}
}
