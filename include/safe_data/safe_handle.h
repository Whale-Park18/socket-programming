#pragma once
#include "basic_safe_data.h"

namespace park18::safe_data
{
	struct default_closehandle
	{
		constexpr default_closehandle() noexcept = default;

		void operator()(HANDLE handle)
		{
			::CloseHandle(handle);
		}
	};

	template<typename Deleter = default_closehandle>
	class safe_handle : public basic_safe_data<HANDLE, Deleter>
	{
	public:
		safe_handle(HANDLE handle) : basic_safe_data<HANDLE, Deleter>(handle) {}
		~safe_handle() = default;

		safe_handle(const safe_handle&) = delete;
		safe_handle& operator=(const safe_handle) = delete;
	};
}