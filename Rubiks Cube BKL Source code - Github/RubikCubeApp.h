#pragma once
#include "Window.h"
#include "Cube/Utility.h"
#include "Cube/CubeTransformer.h"
#include "Cube/RubikCube.h"
#include "Camera.h"

class RubikCubeApp :
    public D3DWindow
{
public:
	RubikCubeApp(HINSTANCE hInstance, HINSTANCE hPrevInstance)
		: D3DWindow(hInstance, hPrevInstance)
	{
	}
	
private:
	//Rubiks Cube
	RubikCube* m_rubikCube = nullptr;
	RubiksCubeTransformer* selectedCubes = nullptr;
	SIDE_SELECTED current_side = NONE;
	// Global variables
	Camera mCamera = Camera(Transform(D3DXVECTOR3(00.0f, 0.0f, 10.0f)));

	D3DXMATRIX orientation;
	float last_update = 0.f;  float interval = 0.0f;

	void RotateMatrix(D3DXMATRIX* orientation, float angleX, float angleY, float angleZ);
protected:
	bool OnSetupD3D() override;
	void OnRender() override;
	void OnProcessKeyboardInput(char* buffer) override;
	void showControls();
	virtual void OnCleanUp() override;
};

