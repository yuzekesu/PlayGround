#pragma once
#include <Windows.h>

#include <exception>
#include <string>

#ifdef NEXCEPTION
#define CHECK(hr)
#else
#define CHECK(hr) Exception::Check(__FILE__, __func__, __LINE__, hr)
#endif // !NDEBUG

class Exception : std::exception {
private:
	std::string _string;

public:
	const char* what();
	static void Check(const char* file, const char* func, int line, HRESULT hr);

private:
	Exception() = delete;
	Exception(const char* file, const char* func, int line, HRESULT hr);
	void Extract_HRESULT_to_clipboard(HRESULT hr);
};
