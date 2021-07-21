#include "CubeTransformer.h"

RubiksCubeTransformer::~RubiksCubeTransformer(void)
{

}

RubiksCubeTransformer::RubiksCubeTransformer(D3DXVECTOR3 normal_t, vector<Transform*> cubes) : mTotalDegrees(0), mNormal(normal_t)
{
	mCubes = cubes;
}

void RubiksCubeTransformer::Rotate( int degrees)
{	
	mTotalDegrees += degrees;
	//create the rotation matrix
	D3DXMATRIX rotation;
	D3DXMatrixIdentity(&rotation);
	D3DXMatrixRotationAxis(&rotation,&mNormal, D3DXToRadian( degrees));	
	
	//1. For each cube rotate its orientation
	for(auto iter = mCubes.begin(); iter != mCubes.end(); iter++)
	{	
		D3DXMatrixMultiply( &((*iter)->mOrientation), &((*iter)->mOrientation), &rotation); 

		//2. Rotate the position vector
		D3DXVECTOR3 old_position = (*iter)->mPosition;
		D3DXVECTOR4 new_positon2( 1, 1, 0, 1);
		D3DXVec3Transform(&new_positon2,&(*iter)->mPosition,&rotation);
		(*iter)->mPosition.x = new_positon2.x;
		(*iter)->mPosition.y = new_positon2.y;
		(*iter)->mPosition.z = new_positon2.z;		
	}	
}

bool RubiksCubeTransformer::CanSnap()
{
	return (mTotalDegrees % 90 >= 60 || mTotalDegrees % 90 <= 30);
}

void RubiksCubeTransformer::SnapToPositions(void)
{
	//NB in one case these figures should be calculated 
	// position x| left side position| right side positon
	// -1		 |	-1.5			 | - 0.5
	//  0	     |	-0.5			 | + 0.5
	//  1	     |	+0.5			 | + 1.5

	//rotate the cubes the remaining amount
	if(mTotalDegrees <= 0)
	{
		int check(mTotalDegrees%90);

		if(mTotalDegrees % 90 <= -60)
			Rotate(-(90 + (mTotalDegrees % 90)));
		else 
			Rotate(-(mTotalDegrees % 90));
	}
	else
	{
		if(mTotalDegrees % 90 >= 60)  
			Rotate(90 - (mTotalDegrees % 90));
		else 
			Rotate(-(mTotalDegrees % 90));
	}
	mTotalDegrees = 0; //reset

	//now snap to positions
	for(vector<Transform*>::iterator this_cube = mCubes.begin(); this_cube != mCubes.end(); this_cube++)
	{
		if((*this_cube)->mPosition.x > 0.5) 
			(*this_cube)->mPosition.x = 1;
		else if((*this_cube)->mPosition.x < -0.5) 
			(*this_cube)->mPosition.x = -1;
		else (*this_cube)->mPosition.x = 0;

		if((*this_cube)->mPosition.y > 0.5) 
			(*this_cube)->mPosition.y = 1;
		else if((*this_cube)->mPosition.y < -0.5) 
			(*this_cube)->mPosition.y = -1;
		else (*this_cube)->mPosition.y = 0;

		if((*this_cube)->mPosition.z > 0.5) 
			(*this_cube)->mPosition.z = 1;
		else if((*this_cube)->mPosition.z < -0.5) 
			(*this_cube)->mPosition.z = -1;
		else (*this_cube)->mPosition.z = 0;

	}
}

vector<Transform*>* RubiksCubeTransformer::GetCubes()
{
	return &mCubes;
}
