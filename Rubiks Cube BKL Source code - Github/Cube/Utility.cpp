#include "Utility.h"

CRubikCubeUtility::CRubikCubeUtility(void)
	: mRandomRotationCount(0)
{

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
	mRandomRotationCount++;
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
	mRandomRotationCount--;;
	vector<cube_move_pair>::iterator last = moves.end();
	last--;
	cube_move_pair move = *last;
	moves.erase(last,moves.end());
	return move;
}