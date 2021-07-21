#pragma once
#include <vector>
using namespace std;
#include "Cube.h"

class CRubikCubeUtility
{
public:
	CRubikCubeUtility(void);
	~CRubikCubeUtility(void);
	void AddRandomRotation(D3DXVECTOR3&,int);
	
	typedef pair<D3DXVECTOR3,int> cube_move_pair;
	cube_move_pair getRandomRotation();
private:
	float nRandomRotations;
	float nPlayerMoves;	
	vector<cube_move_pair> moves;
public:
	int getRandomMovesLength(){ return moves.size(); }
};

