#pragma once
#include <cstdint>

class NormalizedColor
{
public:
	NormalizedColor() = delete;
	NormalizedColor(float r, float g, float b, float alpha = 1.f);
	const float* data() const;
	float& operator[](int i);
	static NormalizedColor Convert(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha);
	void Copy_To(float* destination) const;
private:
	float _color[4];
};

