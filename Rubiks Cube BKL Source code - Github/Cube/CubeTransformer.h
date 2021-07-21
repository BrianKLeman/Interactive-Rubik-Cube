#pragma once

#include <vector>
using namespace std;

#include "Transform.h"

class RubiksCubeTransformer
{
public:

	RubiksCubeTransformer(D3DXVECTOR3 normal_t, vector<Transform*> cubes);
	~RubiksCubeTransformer(void);

 	void Rotate( int amount );
	void SnapToPositions(void);
	bool CanSnap();

	vector<Transform*>* GetCubes();
	
	vector<Transform*> mCubes;
	int mTotalDegrees;
	D3DXVECTOR3 mNormal;
};

