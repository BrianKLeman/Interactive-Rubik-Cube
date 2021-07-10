#pragma once
//-----------------------------------------------------------------------------
// RUBIKS CUBE AUTHOR: BRIAN LEMAN, SID 16024894 D.O.B. 22/01/1985 START DATE: 30/09/2009
#include <math.h>
#include <fstream>
using namespace std;
#include "colour_enum.h"
#include "CubeTransformer.h"
#include "ChildCube.h"
#include "Box.h"
#include "MathLogger.h"


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

class RubikCube
{
private:
	ChildCube mChildCubes[NBCUBES];
	Box mBox;

	int* GetSide( D3DXVECTOR3 normal);
	int* FindMiddleSubjectCubes( D3DXVECTOR3 normal);
	bool CorrectCube(D3DXVECTOR3 normal);
	bool IsNotIntersectingWithAnyOfTheOtherCubes(int subjectCube, D3DXVECTOR3 normal);
	void LogDebug(string filename);

public:
	RubikCube(D3DXMATRIX& view, D3DXMATRIX & proj);
	~RubikCube(void);

	vector<ChildCube*> GetCubesToRotate(D3DXVECTOR3 normal);
	vector<ChildCube*> GetCubesToRotate(D3DXVECTOR3 normal1, bool middle);

	bool IsComplete();
	void SnapCubesToPosition(void);
	bool IsCubeInList(ChildCube* subject,vector<ChildCube*>* list, int list_length);

	// Rendering.
	void OnRender(SIDE_SELECTED selected_side, D3DXMATRIX g_orientation, bool useShader, vector<ChildCube*>* selectedCubes);
	bool InitEffect();
	void Init(LPDIRECT3DDEVICE9 device,D3DXMATRIX& viewIn, D3DXMATRIX& projIn);
};

