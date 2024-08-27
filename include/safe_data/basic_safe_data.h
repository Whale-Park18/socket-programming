#pragma once
#include "global.h"

#include <concepts>

namespace park18::safe_data
{
	template<typename T, typename Deleter>
	requires std::is_same_v<T, HANDLE> || std::is_same_v<T, SOCKET>
	class basic_safe_data
	{
	public:
		basic_safe_data(T t) : _t(t), _deleter() {}
		basic_safe_data(const basic_safe_data&) = delete;
		~basic_safe_data() { _deleter(_t); }

		bool operator==(T othrer) { return _t; }
		operator T() const { return _t; }
		basic_safe_data& operator=(const basic_safe_data&) = delete;

		T get() { return _t; }

	protected:
		T _t;
		Deleter _deleter;
	};
}