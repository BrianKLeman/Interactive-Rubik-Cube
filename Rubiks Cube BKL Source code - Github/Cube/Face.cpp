#include "Face.h"


Face::Face()  
{
	mColour = Red;
}


Face::~Face(void)
{
}


// this decides based on the colour assigned, what its surface normal is
D3DXVECTOR3 Face::GetSurfaceNormal(void)
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


void Face::SetColour(Colour t_colour)
{
	mColour = t_colour;
}
