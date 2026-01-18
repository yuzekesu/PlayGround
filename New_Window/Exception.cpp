#include "ClipBoard.h"
#include "Exception.h"
#include <Windows.h>
#include <ios>
#include <sstream>

const char* Exception::what() {
	return _string.c_str();
}

void Exception::Check(const char* file, const char* func, int line, HRESULT hr) {
	if (FAILED(hr)) {
		throw Exception(file, func, line, hr);
	}
}

Exception::Exception(const char* message) {
	_string = message;
}

Exception::Exception(const char* file, const char* func, int line, HRESULT hr) {
	Extract_HRESULT_to_clipboard(hr);
	char linebreak = '\n';
	std::stringstream _stream;
	const char* file_not_full_path = file;
	for (int i = 0; file[i] != '\0'; i++) {
		if (file[i] == '\\' || file[i] == '/') {
			file_not_full_path = file + i + 1;
		}
	}
	_stream << "File:\t\t" << file_not_full_path << linebreak;
	_stream << "Method:\t\t" << func << linebreak;
	_stream << "Line:\t\t" << line << linebreak;
	_stream << "HRESULT:\t\t" << "0x" << std::uppercase << std::hex << hr << linebreak;
	_stream << std::nouppercase << std::dec;
	char* new_buffer = nullptr;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hr, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPSTR)&new_buffer, 0, nullptr);
	if (new_buffer == nullptr) {
		_stream << linebreak << "UNABLE_TO_FORMAT_ERROR_FROM_HRESULT";
	}
	else {
		_stream << linebreak << new_buffer;
	}
	_string = _stream.str();
	LocalFree(new_buffer);
}

void Exception::Extract_HRESULT_to_clipboard(HRESULT hr) {
	std::stringstream stream;
	stream << "0x" << std::uppercase << std::hex << hr;
	ClipBoard::Copy(stream.str().c_str());
}
