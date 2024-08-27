#pragma once
#include "basic_safe_data.h"

namespace park18::safe_data
{
	struct default_closesocket
	{
		constexpr default_closesocket() noexcept = default;

		void operator()(SOCKET sock)
		{
			::closesocket(sock);
		}
	};

	template<typename Deleter = default_closesocket>
	class safe_socket : public basic_safe_data<SOCKET, Deleter>
	{
	public:
		safe_socket(SOCKET handle) : basic_safe_data<SOCKET, Deleter>(handle) {}
		~safe_socket() = default;

		safe_socket(const safe_socket&) = delete;
		safe_socket& operator=(const safe_socket) = delete;
	};
}