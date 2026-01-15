#include "NormalizedColor.h"
#include <cstdint>
#include <cstring>
#include <stdexcept>

NormalizedColor::NormalizedColor(float r, float g, float b, float alpha)
{
	_color[0] = r;
	_color[1] = g;
	_color[2] = b;
	_color[3] = alpha;
}

const float* NormalizedColor::data() const
{
	return _color;
}

float& NormalizedColor::operator[](int i)
{
	if (i < 0 || i > 4) {
		throw std::out_of_range("Our of the range.");
	}
	return _color[i];
}

NormalizedColor NormalizedColor::Convert(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha)
{
	float rr, bb, gg, aa;
	rr = static_cast<float>(r) / 255.f;
	gg = static_cast<float>(g) / 255.f;
	bb = static_cast<float>(b) / 255.f;
	aa = static_cast<float>(alpha) / 255.f;
	return NormalizedColor(rr, gg, bb, aa);
}

void NormalizedColor::Copy_To(float* destination) const
{
	std::memcpy(destination, _color, sizeof(float) * 4);
}

