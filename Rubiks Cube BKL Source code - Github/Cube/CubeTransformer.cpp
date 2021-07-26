#include <dinput.h>
#include "CubeTransformer.h"

RubiksCubeTransformer::~RubiksCubeTransformer(void)
{

}

RubiksCubeTransformer::RubiksCubeTransformer( RubikCube* rubikCube) : mTotalDegrees(0), mRubiksCube(rubikCube)
{

}

void RubiksCubeTransformer::Rotate( int degrees, SIDE_SELECTED side)
{	
	auto normal = mRubiksCube->GetNormalBySide(side);
	
	mTotalDegrees += degrees;
	//create the rotation matrix
	D3DXMATRIX rotation;
	D3DXMatrixIdentity(&rotation);
	D3DXMatrixRotationAxis(&rotation,&normal, D3DXToRadian( degrees));	
	
	//1. For each cube rotate its orientation
	for(auto iter : mCubes)
	{	
		D3DXMatrixMultiply( &((iter)->mOrientation), &((iter)->mOrientation), &rotation); 

		//2. Rotate the position vector
		D3DXVECTOR3 old_position = (iter)->mPosition;
		D3DXVECTOR4 new_positon2( 1, 1, 0, 1);
		D3DXVec3Transform(&new_positon2,&(iter)->mPosition,&rotation);
		(iter)->mPosition.x = new_positon2.x;
		(iter)->mPosition.y = new_positon2.y;
		(iter)->mPosition.z = new_positon2.z;		
	}	
}

bool RubiksCubeTransformer::CanSnap()
{
	return (mTotalDegrees % 90 >= 60 || mTotalDegrees % 90 <= 30);
}

void RubiksCubeTransformer::SnapToPositions(SIDE_SELECTED side)
{	
	// position x| left side position| right side positon
	// -1		 |	-1.5			 | - 0.5
	//  0	     |	-0.5			 | + 0.5
	//  1	     |	+0.5			 | + 1.5
	//rotate the cubes the remaining amount
	if(mTotalDegrees <= 0)
	{
		int check(mTotalDegrees%90);

		if(mTotalDegrees % 90 <= -60)
			Rotate(-(90 + (mTotalDegrees % 90)), side);
		else 
			Rotate(-(mTotalDegrees % 90), side);
	}
	else
	{
		if(mTotalDegrees % 90 >= 60)  
			Rotate(90 - (mTotalDegrees % 90), side);
		else 
			Rotate(-(mTotalDegrees % 90), side);
	}
	mTotalDegrees = 0; //reset

	//now snap to positions
	for(auto this_cube : mCubes)
	{
		if(this_cube->mPosition.x > 0.5) 
			this_cube->mPosition.x = 1;
		else if(this_cube->mPosition.x < -0.5) 
			this_cube->mPosition.x = -1;
		else this_cube->mPosition.x = 0;

		if(this_cube->mPosition.y > 0.5) 
			this_cube->mPosition.y = 1;
		else if(this_cube->mPosition.y < -0.5) 
			this_cube->mPosition.y = -1;
		else this_cube->mPosition.y = 0;

		if(this_cube->mPosition.z > 0.5) 
			this_cube->mPosition.z = 1;
		else if(this_cube->mPosition.z < -0.5) 
			this_cube->mPosition.z = -1;
		else this_cube->mPosition.z = 0;

	}
}

vector<Transform*>* RubiksCubeTransformer::GetCubes()
{
	return &mCubes;
}


bool RubiksCubeTransformer::autoCompleteFinished()
{
	//finished
	if ((auto_final_angle == 0 && getRandomMovesLength() == 0 && auto_final_angle == 0) || autoCompleteRandom == false)
	{
		autoCompleteRandom = false;
		return true;
	}
	//still rotating
	if (auto_final_angle != 0) return false;

	//find out if any more left to rotate
	auto_rotate = autoCompleteRandom = autoUndoOneRandomMoveAndSetAngles();
	return autoCompleteRandom;
}

bool RubiksCubeTransformer::continueUndoOneRandomMove(SIDE_SELECTED  side)
{
	if ( auto_final_angle == 0 || auto_rotate == false) return false;
	int deg(auto_final_angle / abs(auto_final_angle));
	Rotate(deg, side);
	auto_current_angle += deg;
	if (auto_current_angle == auto_final_angle)
	{
		auto_current_angle = 0;
		auto_final_angle = 0;
		auto_rotate = false;
		if (CanSnap())
		{
			SnapToPositions(side);
		}
		return false;
	}

	return true;
}

bool RubiksCubeTransformer::autoUndoOneRandomMoveAndSetAngles()
{
	
	if (CanSnap())
	{
		SnapToPositions(mCurrentSide);
	}
	else
	{
		return false;
	}

	cube_move_pair move = getRandomRotation();
	if (move.second == 0) return false;

	auto selectedCubes = mRubiksCube->GetCubesToRotate(move.first);
	if (selectedCubes.size() != 0)
	{
		auto_current_angle = 0;
		auto_final_angle = -move.second;//inverse angle
		return true; //success
	}
	return false;
}


void RubiksCubeTransformer::alterCubeByRandom(int n_times)
{	
	
	for (int i(0); i < n_times; ++i)
	{
		auto side = getOneSide();
		int angle((((rand() % 3) + 1) * 90) - 180); angle == 0 ? angle = 180 : angle = angle;
		Rotate(angle,side);
		if (CanSnap()) SnapToPositions(side);
		
		AddRandomRotation(side, angle);
	}
}

//Randomise cube - by turning side x, n times
SIDE_SELECTED RubiksCubeTransformer::getOneSide()
{
	switch (rand() % 6)
	{
	case 0:	return FRONT;
		case 1:		return BACK;
		case 2:		return TOP;
		case 3:		return BOTTOM;
		case 4:		return LEFT;
		case 5:		return RIGHT;
	}
	return NONE;
}

void RubiksCubeTransformer::AddRandomRotation(SIDE_SELECTED side, int degrees)
{
	cube_move_pair move;
	move.first = side;
	move.second = degrees;
	moves.push_back(move);
	mRandomRotationCount++;
}

RubiksCubeTransformer::cube_move_pair RubiksCubeTransformer::getRandomRotation()
{
	if (moves.size() < 1)
	{
		cube_move_pair nothing;
		nothing.first = NONE;
		nothing.second = 0;
		return nothing;
	}
	mRandomRotationCount--;;
	vector<cube_move_pair>::iterator last = moves.end();
	last--;
	cube_move_pair move = *last;
	moves.erase(last, moves.end());
	return move;
}

void RubiksCubeTransformer::OnProcessKeyboardInput(char* buffer)
{
	// Check that the specific keys been pressed
	if (auto_rotate == false)
	{
		if (KEYDOWN(buffer, DIK_BACKSPACE)) backspacePressed = true;
		else if (backspacePressed)
		{
			backspacePressed = false;
			auto_rotate = autoUndoOneRandomMoveAndSetAngles();
		}

		if (KEYDOWN(buffer, DIK_DELETE))
		{
			deletePressed = true;
		}
		else if (deletePressed)
		{
			deletePressed = false;
			autoCompleteRandom = true;
		}

		if (KEYDOWN(buffer, DIK_TAB))
		{
			tabPressed = true;
		}
		else if (tabPressed)
		{
			if (mCurrentSide != NONE)
			{
				if (CanSnap())
				{
					SnapToPositions(mCurrentSide);
					
				}
			}
			
			switch (mCurrentSide)
			{
				case NONE:				mCurrentSide = FRONT; 				   break;
				case FRONT:				mCurrentSide = BACK; 					   break;
				case BACK:				mCurrentSide = TOP;	 					   break;
				case TOP:				mCurrentSide = BOTTOM;					   break;
				case BOTTOM:			mCurrentSide = LEFT;					   break;
				case LEFT:				mCurrentSide = RIGHT;				  	   break;
				case RIGHT:				mCurrentSide = NONE;		   break;
				/*case HORIZONTAL_MIDDLE:	mCurrentSide = VERTICAL_MIDDLE;			   break;
				case VERTICAL_MIDDLE:	mCurrentSide = NONE;		break;*/
			}
			mCubes = mRubiksCube->GetCubesToRotate(mCurrentSide);
			tabPressed = false;
		}

		if (KEYDOWN(buffer, DIK_PGUP))
		{
			Rotate(1, mCurrentSide);
		}

		if (KEYDOWN(buffer, DIK_PGDN))
		{
			Rotate(-1, mCurrentSide);
		}
		/*
		if (KEYDOWN(buffer, DIK_R))
		{

			if(mCurrentSide != NONE)
				if (CanSnap())
				{
					SnapToPositions(mCurrentSide);
				}
			alterCubeByRandom(1);
		}*/
	}
	
	if (KEYDOWN(buffer, DIK_RETURN))
	{		
		SnapToPositions(mCurrentSide);		
	}
}

