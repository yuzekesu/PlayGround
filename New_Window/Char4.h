#pragma once
#include <cstdint>
struct Char4 {
public:
	Char4() = delete;
	Char4(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : _r(r), _g(g), _b(b), _a(a) {}
public:
	uint8_t _r;
	uint8_t _g;
	uint8_t _b;
	uint8_t _a;
};
static_assert(sizeof(Char4) == 4, "Char4 should be 4 bytes.");

