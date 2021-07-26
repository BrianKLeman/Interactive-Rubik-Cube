#include "RubikCubeApp.h"

bool RubikCubeApp::OnSetupD3D()
{
	D3DXMatrixIdentity(&orientation);
	m_rubikCube = new RubikCube(mCamera); //create the cube
	m_rubikCube->Init(mpD3DDevice);
	if (!m_rubikCube->InitEffect())
	{
		MessageBox(NULL, "Shaders failed to initialize This  program will now shut down.", "Program is closing.", MB_OK);
		PostQuitMessage(0);
		return false;
	}
	selectedCubes = new RubiksCubeTransformer(m_rubikCube);

	// Disable the lighting.
	mpD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// Turn on D3D depth buffer to ensure polygons are rendered correctly according to their depth
	mpD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	// Turn off D3D backface culling to ensure both faces of triangles are rendered (clockwise & anti-clockwise)
	mpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	return true;
}

void RubikCubeApp::OnRender()
{
	m_rubikCube->OnRender(current_side, orientation, true, selectedCubes->GetCubes());
}

void RubikCubeApp::OnProcessKeyboardInput(char* buffer)
{
	selectedCubes->OnProcessKeyboardInput(buffer);

	// Rotate cube
	if (KEYDOWN(buffer, DIK_S))
	{
		RotateMatrix(&orientation, -0.1f, 0.0f, 0.0f);
	}

	if (KEYDOWN(buffer, DIK_W))
	{
		RotateMatrix(&orientation, 0.1f, 0.0f, 0.0f); // Back
	}

	// rotate cube right
	if (KEYDOWN(buffer, DIK_D))
	{
		RotateMatrix(&orientation, 0.0f, -0.1f, 0.0f); // Right
	}

	// rotate cube left
	if (KEYDOWN(buffer, DIK_A))
	{
		RotateMatrix(&orientation, 0.0f, +0.1f, 0.0f); // Left		
	}

	//RESET VIEW
	if (KEYDOWN(buffer, DIK_SPACE))
	{
		D3DXMatrixIdentity(&orientation); // Reset		
	}

	if (KEYDOWN(buffer, DIK_F1))
	{
		showControls();
	}
}

void RubikCubeApp::showControls()
{
	MessageBox(mHwnd, "Keyboard Configuration\n\nCURSOR:\tRotate Whole Cube\nSPACE:\tReset View\n\nTab:\tCycle Through Rotatable Sections/Complete Rotation\n\tPage Up Or Page Down Keys:\tRotate\n\nR:\tHOLD to rapidly rotate sections\n\nQ:\tTo Quit", "Controls", MB_OK | MB_ICONINFORMATION);
}

void RubikCubeApp::OnCleanUp()
{
	if (m_rubikCube != nullptr)
		delete m_rubikCube;
}

void RubikCubeApp::RotateMatrix(D3DXMATRIX* orientation, float angleX, float angleY, float angleZ)
{
	//create the rotation matrix
	D3DXMATRIX rotation;
	if (angleX != 0.0f) D3DXMatrixRotationX(&rotation, angleX);
	if (angleY != 0.0f) D3DXMatrixRotationY(&rotation, angleY);
	if (angleZ != 0.0f) D3DXMatrixRotationZ(&rotation, angleZ);

	D3DXMatrixMultiply(orientation, orientation, &rotation);
}