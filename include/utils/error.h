#pragma once

#include <iostream>
#include <string_view>

namespace park18::utils::error
{
	static void error_handling(std::string_view message)
	{
		std::cerr << message << std::endl;
		::exit(1);
	}
}
