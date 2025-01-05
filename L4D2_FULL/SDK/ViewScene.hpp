#pragma once

#include "CInterfaceWrapper.hpp"
#include "IVEngineClient013.hpp"

using vec_t = float;

class Vector
{
public:
	Vector(float X, float Y, float Z)
	{
		x = X; y = Y; z = Z;
	}
	Vector() = default;
	// Members
	vec_t x, y, z;

	vec_t operator[](int i) const
	{
		return ((vec_t*)this)[i];
	}
	vec_t& operator[](int i)
	{
		return ((vec_t*)this)[i];
	}
};

class VMatrix
{
public:
	vec_t		m[4][4];

	inline float* operator[](int i)
	{
		return m[i];
	}

	inline const float* operator[](int i) const
	{
		return m[i];
	}

	// Get a pointer to m[0][0]
	inline float* Base()
	{
		return &m[0][0];
	}

	inline const float* Base() const
	{
		return &m[0][0];
	}
};

//Todo: Работает ужасно, какие-то мерцания и иногда рисует не там, где ентити.
static bool FrustumTransform(const VMatrix& worldToSurface, const Vector& point, Vector& screen)
{
	static const auto iec = reinterpret_cast<IVEngineClient013*>(CInterfaceWrapper::GetInterface("engine.dll", "VEngineClient013"));
	// UNDONE: Clean this up some, handle off-screen vertices
	float w;

	screen.x = worldToSurface[0][0] * point[0] + worldToSurface[0][1] * point[1] + worldToSurface[0][2] * point[2] + worldToSurface[0][3];
	screen.y = worldToSurface[1][0] * point[0] + worldToSurface[1][1] * point[1] + worldToSurface[1][2] * point[2] + worldToSurface[1][3];
	//	z		 = worldToSurface[2][0] * point[0] + worldToSurface[2][1] * point[1] + worldToSurface[2][2] * point[2] + worldToSurface[2][3];
	w = worldToSurface[3][0] * point[0] + worldToSurface[3][1] * point[1] + worldToSurface[3][2] * point[2] + worldToSurface[3][3];

	// Just so we have something valid here
	screen.z = 0.0f;

	bool behind;
	if (w < 0.001f)
	{
		behind = true;
		screen.x *= 100000;
		screen.y *= 100000;
	}
	else
	{
		behind = false;
		float invw = 1.0f / w;
		screen.x *= invw;
		screen.y *= invw;

		int x;
		int y;
		iec->GetScreenSize(x, y);
		screen.x = (x / static_cast<float>((2))) + (0.5f * screen.x * x + 0.5f);
		screen.y = (y / static_cast<float>((2))) - (0.5f * screen.y * y + 0.5f);
	}

	return behind;
}
