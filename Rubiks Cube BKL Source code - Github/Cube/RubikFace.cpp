#include "Face.h"

RubikFace::RubikFace(Colour t_colour)  
	: mColour(t_colour)
{

}

RubikFace::~RubikFace(void)
{
}

// Map colour to direction a face is facing.
D3DXVECTOR3 RubikFace::GetSurfaceNormal(void)
{
	switch (mColour)
	{
		case Red: 		return D3DXVECTOR3( 0, 1, 0);
		case Green: 	return D3DXVECTOR3( 0,-1, 0);
		case Blue: 		return D3DXVECTOR3( 1, 0, 0);
		case Yellow: 	return D3DXVECTOR3(-1, 0, 0);
		case White: 	return D3DXVECTOR3( 0, 0, 1);
		case Orange: 	return D3DXVECTOR3( 0, 0,-1);
		default:		return D3DXVECTOR3(0,0,0);
	}	
}

