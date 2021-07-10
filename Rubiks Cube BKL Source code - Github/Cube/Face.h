#pragma once

#include "colour_enum.h"
#include <d3dx9math.h>

class Face
{
public:
Face();
~Face(void);

public:
	Colour mColour;
public:
	// this decides based on the colour assigned, what its surface normal is
	D3DXVECTOR3 GetSurfaceNormal(void);
	void SetColour(Colour colour);
};

