#pragma once
#include <WinSock2.h>

namespace park18::types
{
	template<typename T, typename Deleter>
	class SmartHandle
	{
	public:
		inline SmartHandle(T t, Deleter deleter);
		inline SmartHandle(const SmartHandle& rhs) = delete;
		inline ~SmartHandle();

		inline void Reset(T t);
		inline const T Get() const;

		SmartHandle& operator=(const SmartHandle& rhs) = delete;
		bool operator==(const T t);

	private:
		T _tHandle;
		Deleter _deleter;
	};

	template<typename T, typename Deleter>
	inline SmartHandle<T, Deleter>::SmartHandle(T t, Deleter deleter) : _tHandle(t), _deleter(deleter)
	{

	}

	template<typename T, typename Deleter>
	inline SmartHandle<T, Deleter>::~SmartHandle()
	{
		_deleter(_tHandle);
	}

	template<typename T, typename Deleter>
	inline void SmartHandle<T, Deleter>::Reset(T t)
	{
		_deleter(_tHandle);
		_tHandle = t;
	}

	template<typename T, typename Deleter>
	inline const T SmartHandle<T, Deleter>::Get() const
	{
		return _tHandle;
	}
}