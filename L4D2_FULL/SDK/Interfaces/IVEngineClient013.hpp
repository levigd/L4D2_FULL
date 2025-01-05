#pragma once

//#include "ViewScene.hpp"

class VMatrix;

class IVEngineClient013
{
public:
	// Gets the dimensions of the game window
	void GetScreenSize(int& width, int& height);

	// Get access to the world to screen transformation matrix
	const VMatrix& GetWorldToScreenMatrix();

};

