#pragma once
//-----------------------------------------------------------------------------
// RUBIKS CUBE AUTHOR: BRIAN LEMAN, SID 16024894 D.O.B. 22/01/1985 START DATE: 30/09/2009
#include <math.h>
#include <vector>
using namespace std;
#include "colour_enum.h"
#include "Transform.h"
#include "Cube.h"


#if defined(DEBUG) | defined(_DEBUG)
#include <DxErr.h>
#ifndef HR
#define HR(x)                                      \
{                                                  \
	HRESULT hr = x;                                \
{                                              \
	\
}                                              \
}
#endif

#else
#ifndef HR
#define HR(x) x;
#endif
#endif 
// Define a macro to represent the key detection predicate.
#ifndef KEYDOWN
	#define KEYDOWN(name, key) (name[key] & 0x80)	
#endif
class RubikCube
{
private:
	Transform mChildCubes[NBCUBES];
	Cube mBox;

	int* GetSide( D3DXVECTOR3 normal);
	int* FindMiddleSubjectCubes( D3DXVECTOR3 normal);
	bool IsNotIntersectingWithAnyOfTheOtherCubes(int subjectCube, D3DXVECTOR3 normal);

public:
	RubikCube(Camera camera);
	~RubikCube(void);
	vector<Transform*> GetCubesToRotate(D3DXVECTOR3 normal);
	vector<Transform*> GetCubesToRotate(D3DXVECTOR3 normal1, bool middle);
	vector<Transform*> GetCubesToRotate(SIDE_SELECTED side);
	bool IsComplete();
	void SnapCubesToPosition(void);
	bool IsCubeInList(Transform* subject,vector<Transform*>* list, int list_length);
	// Rendering.
	void OnRender(SIDE_SELECTED selected_side, D3DXMATRIX g_orientation, bool useShader, vector<Transform*>* selectedCubes);
	bool InitEffect();
	void Init(LPDIRECT3DDEVICE9 device);

	D3DXVECTOR3 GetNormalBySide(SIDE_SELECTED side);
};

