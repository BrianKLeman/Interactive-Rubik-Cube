#pragma once

#include "colour_enum.h"
#include <d3dx9math.h>

class RubikFace
{
public:
	RubikFace(Colour colour);
	~RubikFace(void);
public:
	Colour mColour;

	// this decides based on the colour assigned, what its surface normal is
	D3DXVECTOR3 GetSurfaceNormal(void);
};

