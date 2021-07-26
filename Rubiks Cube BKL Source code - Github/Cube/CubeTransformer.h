#pragma once

#include <vector>
using namespace std;

#include "Transform.h"
#include "RubikCube.h"

class RubiksCubeTransformer
{
public:

	RubiksCubeTransformer( RubikCube* rubikCube);
	~RubiksCubeTransformer(void);

 	void Rotate( int amount, SIDE_SELECTED side );
	void SnapToPositions(SIDE_SELECTED side);
	bool CanSnap();

	vector<Transform*>* GetCubes();

	bool autoCompleteFinished();

	bool continueUndoOneRandomMove(SIDE_SELECTED normal);

	bool autoUndoOneRandomMoveAndSetAngles();

	//Randomise cube - by turning side x, n times
	SIDE_SELECTED getOneSide();

	void alterCubeByRandom(int n_times = 3);
	void AddRandomRotation(SIDE_SELECTED, int);
	typedef pair<SIDE_SELECTED, int> cube_move_pair;
	cube_move_pair getRandomRotation();
	int getRandomMovesLength() { return moves.size(); }
	vector<Transform*> mCubes = vector<Transform*>(0);
	int mTotalDegrees;

	void OnProcessKeyboardInput(char* buffer);

	void RotateMatrix(D3DXMATRIX* orientation, float angleX, float angleY, float angleZ);
private:
	bool auto_rotate_finished = false;
	int	 auto_final_angle_finished = 0;
	int	 auto_current_angle_finished = 0;
	int randomMoves = 0;
	bool auto_rotate = false;
	int	 auto_final_angle = 0;
	int	 auto_current_angle = 0;
	bool autoCompleteRandom = false;
	float mRandomRotationCount;
	vector<cube_move_pair> moves;
	bool deletePressed = false, backspacePressed = false, tabPressed = false;
	RubikCube* mRubiksCube;
	SIDE_SELECTED mCurrentSide = NONE;
};

