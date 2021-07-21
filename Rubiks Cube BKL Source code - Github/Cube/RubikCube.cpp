#include "RubikCube.h"

RubikCube::RubikCube(D3DXMATRIX &view, D3DXMATRIX &proj)
	:  mBox(view, proj)
{	
	int array_element(0);	

	for(float xPos(-1); xPos <= 1; xPos += 1) // x
		for(float yPos(-1); yPos <= 1; yPos += 1)  // y
			for(float zPos(-1); zPos <= 1; zPos += 1)  // z			
				mChildCubes[array_element++] = Transform(D3DXVECTOR3(xPos,yPos,zPos));
}


RubikCube::~RubikCube(void)
{
}

vector<Transform*> RubikCube::GetCubesToRotate(D3DXVECTOR3 normal)
{
	int* correctCubes = GetSide(normal);
	
	if(correctCubes == nullptr) 
		return vector<Transform*>(0);

	vector<Transform*> cubes_vector(9);

	for(int i(0); i < 9; ++i) 
		cubes_vector[i] = &mChildCubes[correctCubes[i]];

	return cubes_vector; //beware of memory management issues here
}

int* RubikCube::GetSide( D3DXVECTOR3 normal)
{
	int* correctCubes = new int[9];
	int count(0);
	for(int i(0); i < NBCUBES; i++)
	{
		if(IsNotIntersectingWithAnyOfTheOtherCubes(i,normal) == true) 
		{
			correctCubes[count++] = i;
		}		
	}	
	
	if (count != 9) 
	{
		return nullptr; // Not enough cubes found.
	}
	else 
	{
		return correctCubes; // Found the correct cubes.
	}
}

int* RubikCube::FindMiddleSubjectCubes( D3DXVECTOR3 normal)
{
	int* correctCubes = new int[9];

	int* inCorrectCubes = new int[9];
	int count(0);
	for(int i(0); i < NBCUBES; i++)
	{
		if(IsNotIntersectingWithAnyOfTheOtherCubes(i,normal)) 
		{
			inCorrectCubes[count++] = i;
		}		
	}

	int* inCorrectCubes2 = new int[9];
	int count2(0);
	for(int i(0); i < NBCUBES; i++)
	{
		if(IsNotIntersectingWithAnyOfTheOtherCubes(i,-normal)) {inCorrectCubes2[count2] = i;count2++;}
		
	}	
	
	if (count != 9 || count2 != 9) 
		return 0;//not enough cubes found
	
	//get the cubes not in either arrays
	int countFinal(0);
	bool check(false);
	for(int i = 0; i < 27; ++i,check = false)
	{
		for(int j = 0; j < 9; ++j)
		{
			if((inCorrectCubes[j] == i) || (inCorrectCubes2[j] == i)) check = true; //not a middle row cube
			if(j == 8 && check == false) { correctCubes[countFinal] = i; ++countFinal; }//a middle row cube
		}
	}

	if(countFinal == 9) 
		return correctCubes; //found them all
	else 
		return 0;
}

bool RubikCube::IsNotIntersectingWithAnyOfTheOtherCubes(int subjectCube, D3DXVECTOR3 normal)
{
	//calculate end position vector
	D3DXVECTOR3 pos(0,0,0);
	pos.x=mChildCubes[subjectCube].mPosition.x + normal.x;
	pos.y=mChildCubes[subjectCube].mPosition.y + normal.y;
	pos.z=mChildCubes[subjectCube].mPosition.z + normal.z;		

	//check with every cube inc. subject since using an if every time will take longer and more code
	for(int i = 0; i < NBCUBES; i++ )
	{
		//check x
		if(subjectCube == i) 
			continue;

		if(((pos.x > (mChildCubes[i].mPosition.x - HALFWIDTH)) && (pos.x < (mChildCubes[i].mPosition.x + HALFWIDTH)) &&
		   (pos.y > (mChildCubes[i].mPosition.y - HALFWIDTH)) && (pos.y < (mChildCubes[i].mPosition.y + HALFWIDTH)) &&
		   (pos.z > (mChildCubes[i].mPosition.z - HALFWIDTH)) && (pos.z < (mChildCubes[i].mPosition.z + HALFWIDTH))   ) == true)
		   return false; //intersection
	}

	return true;//a correct cube
}

void RubikCube::SnapCubesToPosition(void)
{
	//ok we have three postions from above
	//NB in one case these figures should be calculated 
	// position x| left side position| right side positon
	// -1		 |	-1.5			 | - 0.5
	// 0	     |	-0.5			 | + 0.5
	// 1	     |	+0.5			 | + 1.5
	for(int i(0); i < NBCUBES; i++)
	{
		if(mChildCubes[i].mPosition.x > 0.5) 
			mChildCubes[i].mPosition.x = 1;
		else if(mChildCubes[i].mPosition.x < -0.5) 
			mChildCubes[i].mPosition.x = -1;
		else 
			mChildCubes[i].mPosition.x = 0;

		if(mChildCubes[i].mPosition.y > 0.5) 
			mChildCubes[i].mPosition.y = 1;
		else if(mChildCubes[i].mPosition.y < -0.5) 
			mChildCubes[i].mPosition.y = -1;
		else 
			mChildCubes[i].mPosition.y = 0;

		if(mChildCubes[i].mPosition.z > 0.5) 
			mChildCubes[i].mPosition.z = 1;
		else if(mChildCubes[i].mPosition.z < -0.5) 
			mChildCubes[i].mPosition.z = -1;
		else 
			mChildCubes[i].mPosition.z = 0;
	}
}

void RubikCube::OnRender(SIDE_SELECTED current_side,D3DXMATRIX g_orientation,bool useShader,vector<Transform*>* selectedCubes)
{
	static float  angle(0);
	float flashFactor = 1.0f;	
	flashFactor += sin(angle++)/2;

	for(int i(0); i < NBCUBES; i++) 
	{
		if(selectedCubes != 0)
		{
			if (IsCubeInList(&mChildCubes[i], selectedCubes, 9))
			{
				HR(mBox.RenderWithEffect(g_orientation, mChildCubes[i].mOrientation, mChildCubes[i].mPosition, flashFactor));
			}
			else
			{
				HR(mBox.RenderWithEffect(g_orientation, mChildCubes[i].mOrientation, mChildCubes[i].mPosition, 1.0f));
			}
		}
		else
		{
			HR(mBox.RenderWithEffect(g_orientation,mChildCubes[i].mOrientation,mChildCubes[i].mPosition,1.0f));
		}
	}	
}

void RubikCube::Init(LPDIRECT3DDEVICE9 device,D3DXMATRIX& viewIn, D3DXMATRIX& projIn)
{
	mBox.SetViewProjectionMatrix(viewIn,projIn);
	mBox.init(device);
}

bool RubikCube::InitEffect()
{
	if(mBox.setupEffect() == D3D_OK)  return true;
	else
	{
		MessageBox(NULL,"failed to set up effect","effect failiure",MB_OK);
		return false;
	}
}


vector<Transform*> RubikCube::GetCubesToRotate(D3DXVECTOR3 normal1,bool middle)
{
	int* correctCubes;
	correctCubes = FindMiddleSubjectCubes(normal1);	
	if(correctCubes == 0) 
		return vector<Transform*>();

	vector<Transform*> cubes_vector(9);
	for(int i(0); i < 9; ++i) (cubes_vector)[i] = &mChildCubes[correctCubes[i]];
	return cubes_vector;
}

bool RubikCube::IsComplete()
{	
	//is complete if all cube have nearly identical orientations
	//each element of the orientation matrice must be with in 0.1 units
	D3DXMATRIX firstMatrix = mChildCubes[0].mOrientation;
	D3DXMATRIX nMatrix;
	for(int i(1); i < 27; i++)
	{
		nMatrix = mChildCubes[i].mOrientation;
		for(int m(0); m < 16; m++) if(!(firstMatrix[m] <= nMatrix[m] + 0.1f && firstMatrix[m] >= nMatrix[m] - 0.1f)) return false;
	}
	return true;
}

bool RubikCube::IsCubeInList( Transform* subject,vector<Transform*>* list, int list_length )
{
	for(int i = 0; i < list_length;++i)
	{
		if((*list)[i] == subject) return true;
	}
	return false;
}
