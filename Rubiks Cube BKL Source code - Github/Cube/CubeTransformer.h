#pragma once

#include <vector>
using namespace std;

#include "ChildCube.h"

class RubiksCubeTransformer
{
public:

	RubiksCubeTransformer(D3DXVECTOR3 normal_t, vector<ChildCube*> cubes);
	~RubiksCubeTransformer(void);

 	void Rotate( int amount );
	void SnapToPositions(void);
	bool CanSnap();

	vector<ChildCube*>* GetCubes();
	
	vector<ChildCube*> mCubes;
	int mTotalDegrees;
	D3DXVECTOR3 mNormal;
};

