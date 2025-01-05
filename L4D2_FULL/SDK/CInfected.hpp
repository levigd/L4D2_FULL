#pragma once

#include <cstdint>

#include "ViewScene.hpp"

class CInfected
{
public:
	char pad_0000[148]; //0x0000
	Vector position; //0x0094
	char pad_00A0[76]; //0x00A0
	int32_t m_iHealth; //0x00EC //У заражённых оффсет хп другой???
	char pad_00F8[88]; //0x00F8

	[[nodiscard]] bool IsAlive() const noexcept
	{
		return GetHealth() > 0; //А правильно ли.
	}

	[[nodiscard]] bool GetHealth() const noexcept
	{
		return m_iHealth;
	}
};