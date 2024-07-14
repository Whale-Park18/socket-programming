#pragma once
#include "pch.h"
#include "types/basic_types.h"
#include "types/smart_handle.h"

namespace park18::types
{
	constexpr uint BufSize = 1024;

	using SmartSocket = SmartHandle < SOCKET, int(*)(SOCKET)>;
}