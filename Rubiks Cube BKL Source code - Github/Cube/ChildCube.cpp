#include "RubikCube.h"




ChildCube::ChildCube( D3DXVECTOR3 position)
: 
	mPosition(position)
{	
	D3DXMatrixIdentity(&mOrientation);
}

	ChildCube::ChildCube( void )
	{
		D3DXMatrixIdentity(&mOrientation);
	}


ChildCube::~ChildCube(void)
{	
}
