#pragma once
#include <SFML/System/Vector2.hpp>
#include <math.h>

constexpr double PI = 3.141592653589793238463;

// Class using complex numbers to represent rotation
class VectorRotation
{
public:
	VectorRotation() : mReal(1), mImaginary(0) {}
	VectorRotation(float real, float imaginary) : mReal(real), mImaginary(imaginary) {}
	VectorRotation(float radians) : mReal(cosf(radians)), mImaginary(sinf(radians)) {}
	VectorRotation(sf::Vector2f v) : mReal(v.x), mImaginary(v.y) {}
	VectorRotation(const VectorRotation &vr) : mReal(vr.mReal), mImaginary(vr.mImaginary) {}

	static VectorRotation inDegrees(float degrees)
	{
		return VectorRotation(degrees * PI / 180.f);
	}

	VectorRotation operator+(const VectorRotation &vr2) { return VectorRotation(mReal + vr2.mReal, mImaginary + vr2.mImaginary); }
	void operator+=(const VectorRotation &vr2)
	{
		mReal += vr2.mReal;
		mImaginary += vr2.mImaginary;
	}

	VectorRotation operator-() { return VectorRotation(-mReal, -mImaginary); }
	void operator-=(const VectorRotation &vr2)
	{
		mReal -= vr2.mReal;
		mImaginary -= mImaginary;
	}

	VectorRotation operator*(const float &f) { return VectorRotation(mReal * f, mImaginary * f); }
	VectorRotation operator*(const VectorRotation &vr2)
	{
		return VectorRotation(mReal * vr2.mReal - mImaginary * vr2.mImaginary,
							  mReal * vr2.mImaginary + mImaginary * vr2.mReal);
	}

	float length()
	{
		return sqrtf(mReal * mReal + mImaginary * mImaginary);
	}

	void normalize()
	{
		float l = length();
		if (l > 0.f)
		{
			mReal *= 1.0f / l;
			mImaginary *= 1.0f / l;
		}
	}

	sf::Vector2f toVector() { return sf::Vector2f(mReal, mImaginary); }

private:
	float mReal;
	float mImaginary;
};