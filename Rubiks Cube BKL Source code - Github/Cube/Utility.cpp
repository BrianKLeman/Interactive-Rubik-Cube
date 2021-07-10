#include "Utility.h"


CRubikCubeUtility::CRubikCubeUtility(void)
{
	nRandomRotations = 0.0f;
}


CRubikCubeUtility::~CRubikCubeUtility(void)
{
}

void CRubikCubeUtility::AddRandomRotation(D3DXVECTOR3& normal,int degrees)
{
	cube_move_pair move;
	move.first 	= normal;
	move.second = degrees;
	moves.push_back(move);
	nRandomRotations+=1.0f;
}

CRubikCubeUtility::cube_move_pair CRubikCubeUtility::getRandomRotation()
{
	if(moves.size()<1)
	{
		cube_move_pair nothing; 
		nothing.first = D3DXVECTOR3(0,0,0);
		nothing.second= 0;
		return nothing;
	}
	nRandomRotations-=1.0f;
	vector<cube_move_pair>::iterator last = moves.end();
	last--;
	cube_move_pair move = *last;
	moves.erase(last,moves.end());
	return move;
}