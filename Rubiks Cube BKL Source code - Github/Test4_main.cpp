//-----------------------------------------------------------------------------
// DirectInput & Kinematics: Test 4
// Uses Direct Input 8 Interfaces.
// Original provided at SHU During Programming For Games

//-----------------------------------------------------------------------------
// RUBIKS CUBE AUTHOR: BRIAN LEMAN, SID 16024894 D.O.B. 22/01/1985 START DATE: 30/09/2009
#define STRICT
#define DIRECTINPUT_VERSION 0x0800
#define D3D_DEBUG_INFO	// Enable debugging information, so the .NET environment can help you.
#define DEBUG_VS
#define DEBUG_PS 
//-----------------------------------------------------------------------------
// Include these files
#include <windows.h>	// Windows library (for window functions, menus, dialog boxes, etc)
#include <d3dx9.h>		// Direct 3D library (for all Direct 3D functions)
#include <dinput.h>		// Direct Input library (for Direct Input functions)
#include <DxErr.h>
#include "Cube/colour_enum.h"
#include "Cube/Utility.h"
#include "Cube/RubikCube.h" //this rubiks cube class
#include "resource.h"
#include "effect/HLSLEffect.h"
#include "effect/HLSLEffectWithUtilities.h"
//#include "Options Dialog.rc"
#include <ctime>
#include "Trace.h"
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
//-----------------------------------------------------------------------------
// Global variables
HINSTANCE inst; //window instance
HWND hwnd;
bool finshed(false);
bool auto_rotate_finished(false);
int	 auto_final_angle_finished(0);
int	 auto_current_angle_finished(0);
int randomMoves(0);
bool auto_rotate(false);
int	 auto_final_angle(0);
int	 auto_current_angle(0);
CRubikCubeUtility cube_utility;
D3DXMATRIX matView;
D3DXMATRIX matProj;
SIDE_SELECTED current_side(NONE);
LPDIRECT3D9					g_pD3D							= NULL; // The D3D object
LPDIRECT3DDEVICE9      g_pd3dDevice				= NULL; // The rendering device
LPDIRECTINPUT8			g_pDI							= NULL; // The Direct Input object
LPDIRECTINPUTDEVICE8	g_pDIMouseDevice			= NULL; // The Direct Input mouse device.
LPDIRECTINPUTDEVICE8 g_pDIKeyboardDevice	= NULL; // The Direct Input keyboard device.

// Global variables
 D3DXVECTOR3 vCamera(00.0f, 0.0f, 10.0f);
 D3DXVECTOR3 vLookat(0.0f, 0.0f, 0.0f);


//angle of the camera from the origin
double angle(90.0);
bool useShaders(false);
//Rubiks Cube
RubikCube* gpRubikCube = nullptr;
RubiksCubeTransformer* selectedCubes(0);
// Function Prototypes
void WINAPI CleanUpDirectInput();
void CleanUp();

D3DXMATRIX orientation;
float last_update(0), interval(0.0f);
CHLSLEffectWithUtilities* effect1(nullptr);
CHLSLEffectWithUtilities* effect2(nullptr);
CHLSLEffectWithUtilities::vertex_info vertices_struct, vertices_struct_background;
D3DXVECTOR3 colour(1.0f,1.0f,1.0f);
D3DXMATRIX matViewFW, matProjFW;
D3DXVECTOR3 vCameraFW(00.0f, 00.0f, -10.0f);
D3DXVECTOR3 vLookatFW(0.0f, 00.0f, 0.0f);

 D3DXVECTOR3 vBallPositionFW(0.0f, 0.0f, 0.0f);

 FLOAT screenWidth = 800.f, screenHeight = 600.f;
 LPDIRECT3DSURFACE9 gpPrimaryRenderTarget;

//end fire works
void showControls()
{
	MessageBox(hwnd,"Keyboard Configuration\n\nCURSOR:\tRotate Whole Cube\nSPACE:\tReset View\n\nTab:\tCycle Through Rotatable Sections/Complete Rotation\n\tPage Up Or Page Down Keys:\tRotate\n\nR:\tHOLD to rapidly rotate sections\n\nQ:\tTo Quit","Controls",MB_OK | MB_ICONINFORMATION);
}

bool autoCompleteRandom(false);
bool autoCompleteFinished()
{
	//finished
	if((auto_final_angle == 0 && cube_utility.getRandomMovesLength() == 0 && auto_final_angle == 0) || autoCompleteRandom == false)
	{
		autoCompleteRandom = false;
		return true;
	}
	//still rotating
	if(auto_final_angle != 0) return false;

	//find out if any more left to rotate
	bool autoUndoOneRandomMoveAndSetAngles();
	auto_rotate = autoCompleteRandom = autoUndoOneRandomMoveAndSetAngles();
	return autoCompleteRandom;
}

bool continueUndoOneRandomMove()
{
	if(selectedCubes == 0 || auto_final_angle == 0 || auto_rotate == false) return false;
	int deg(auto_final_angle/abs(auto_final_angle));
	selectedCubes->Rotate(	deg);
	auto_current_angle+=deg;
	if(auto_current_angle == auto_final_angle) 
	{ 
		auto_current_angle = 0; 
		auto_final_angle = 0; 
		auto_rotate = false;
		if(selectedCubes->CanSnap())
		{
			selectedCubes->SnapToPositions();
			delete selectedCubes; selectedCubes = 0;
		}
		return false;
	}
	
	return true;
}

bool autoUndoOneRandomMoveAndSetAngles()
{
	if(selectedCubes != 0)
	{
		if(selectedCubes->CanSnap())
		{
			selectedCubes->SnapToPositions();
			delete selectedCubes; selectedCubes = 0;
		}
		else
		{
			return false;
		}
	}

	CRubikCubeUtility::cube_move_pair move = cube_utility.getRandomRotation();
	if(move.second == 0) return false; 
	
	selectedCubes = new RubiksCubeTransformer(move.first,	gpRubikCube->GetCubesToRotate(move.first));
	if(selectedCubes != 0)
	{
		auto_current_angle 	= 0;
		auto_final_angle 	= -move.second;//inverse angle
		return true; //success
	}
	return false;
}

//Randomise cube - by turning side x, n times
D3DXVECTOR3 getOneSide()
{	
	switch(rand()%6)
	{
		case 0:		return D3DXVECTOR3(0, 0,-1);
		case 1:		return D3DXVECTOR3(0, 0,1);
		case 2:		return D3DXVECTOR3(0, 1,0);
		case 3:		return D3DXVECTOR3(0,-1,0);
		case 4:		return D3DXVECTOR3(-1,0,0);
		case 5:		return D3DXVECTOR3(1,0, 0);
	}
	return D3DXVECTOR3(1,0, 0);
}

void alterCubeByRandom(int n_times=3)
{
	if(gpRubikCube != 0)
	{
		D3DXVECTOR3 side;//stack
		RubiksCubeTransformer* transformer = 0;//stack
		for(int i(0); i < n_times; ++i)
		{
			side = getOneSide();
			RubiksCubeTransformer* transformer = new RubiksCubeTransformer(side,(gpRubikCube->GetCubesToRotate(side)));
			int angle((((rand()%3)+1)*90)-180); angle == 0 ? angle = 180 : angle = angle;
			transformer->Rotate(angle);
			if(transformer->CanSnap()) transformer->SnapToPositions();
			delete transformer; transformer = 0; //safe and no memory leaks
			//utilites
			cube_utility.AddRandomRotation(side,angle);
		}
	}
}

int WINAPI dlgProc(HWND hDlg, UINT uMsg,WPARAM wParam, LPARAM lParam)
{	
	switch (uMsg) 
    { 
        case WM_COMMAND: 
            switch (LOWORD(wParam)) 
            { 
                case IDOK: 
                case IDCANCEL: 
                    EndDialog(hDlg, wParam); 
                    return TRUE; 
				case IDC_RADIO1:
					randomMoves = 3;
					return TRUE;
				case IDC_RADIO2:
					randomMoves = 9;
					return TRUE;
				case IDC_RADIO3:
					randomMoves = 27;
					return TRUE;
				case IDC_BUTTON2:
					randomMoves = 0;
					 EndDialog(hDlg, wParam); 
					return TRUE;
				case IDC_BUTTON1:
					 EndDialog(hDlg, wParam); 
					return TRUE;
            } 
    } 
    return FALSE; 
}

void setupOptions()
{	
	DialogBoxA(inst,MAKEINTRESOURCE(101),hwnd,&dlgProc);
	alterCubeByRandom(randomMoves);
}

void RotateMatrix(D3DXMATRIX* orientation,float angleX,float angleY,float angleZ)
{
	//create the rotation matrix
	D3DXMATRIX rotation;
	if(angleX != 0.0f) D3DXMatrixRotationX(&rotation,angleX);
	if(angleY != 0.0f) D3DXMatrixRotationY(&rotation,angleY);
	if(angleZ != 0.0f) D3DXMatrixRotationZ(&rotation,angleZ);

	D3DXMatrixMultiply(orientation,orientation,&rotation);
}

// Initialise Direct 3D.
HRESULT SetupD3D(HWND hWnd)
{
    // Create the D3D object, return failure if this can't be done.
    if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION))) return E_FAIL;

    // Set up the structure used to create the D3DDevice
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                    D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                    &d3dpp, &g_pd3dDevice)))
	{
		MessageBox(hWnd,"Error","Error - Direct3D Device Hardware Acceleration Failed",MB_OK);
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                    &d3dpp, &g_pd3dDevice)))
		{
			MessageBox(hWnd,"Error","Error - Direct3D Device Software Failed",MB_OK);

			return E_FAIL;
		}
        
	}
	// Enable the lighting.
	g_pd3dDevice -> SetRenderState(D3DRS_LIGHTING, FALSE);
    // Turn on D3D depth buffer to ensure polygons are rendered correctly according to their depth
	g_pd3dDevice -> SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    // Turn on D3D backface culling to ensure both faces of triangles are rendered (clockwise & anti-clockwise)
    g_pd3dDevice -> SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	gpRubikCube = new RubikCube(matView, matProj); //create the cube
	gpRubikCube->Init(g_pd3dDevice,matView,matProj);
	useShaders = gpRubikCube->InitEffect();
	if(!useShaders) MessageBox(NULL,"Shaders failed to initialize.","Programming is closing.",MB_OK);

	
	D3DVIEWPORT9 vp;
	g_pd3dDevice->GetViewport(&vp);
	 HR(g_pd3dDevice->GetRenderTarget(0, &gpPrimaryRenderTarget));
	 return S_OK;
}

//-----------------------------------------------------------------------------
// Terminate Direct Input and clean up (always unacquire device before calling Release())
void WINAPI CleanUpDirectInput()
{
    if (g_pDI != NULL)
    {
        // Release the mouse.
		if (g_pDIMouseDevice != NULL)
        {
            g_pDIMouseDevice -> Unacquire();
            g_pDIMouseDevice -> Release();
            g_pDIMouseDevice = NULL;
        }

		// Release the keyboard
		if (g_pDIKeyboardDevice != NULL)
        {
            g_pDIKeyboardDevice -> Unacquire();
            g_pDIKeyboardDevice -> Release();
            g_pDIKeyboardDevice = NULL;
        }

		// Release the Direct Input Object.
		g_pDI -> Release();
        g_pDI = NULL;
    }
}

//-----------------------------------------------------------------------------
// Release (delete) all the resources used by this program.
VOID CleanUp()
{
    CleanUpDirectInput();

    if (g_pd3dDevice != NULL)
	{
		g_pd3dDevice -> Release();
		g_pd3dDevice = NULL;
	}

    if (g_pD3D != NULL)	
	{
		g_pD3D -> Release();
		g_pD3D = NULL;
	}
	
	//clear the cube
	if (gpRubikCube != NULL)	
	{
		delete gpRubikCube;
	}
}


//-----------------------------------------------------------------------------
// Set up the view and projection matrices.
void SetupViewMatrices()
{
	// Set up the view matrix.
	// This defines which way the 'camera' will look at, and which way is up.
    D3DXVECTOR3 vUpVector(0.0f, 1.0f, 0.0f);    
    D3DXMatrixLookAtRH( &matView, &vCamera, &vLookat, &vUpVector);
    //g_pd3dDevice -> SetTransform(D3DTS_VIEW, &matView);

	// Set up the projection matrix.
	// This transforms 2D geometry into a 3D space.    
	D3DXMatrixPerspectiveFovRH(&matProj, 45, screenWidth/screenHeight, 5.0f, -5.0f);
}

//-----------------------------------------------------------------------------
// Render the scene.
void Render()
{		
	HR(g_pd3dDevice->SetRenderTarget(0, gpPrimaryRenderTarget));

	SetupViewMatrices();
	// Clear the backbuffer to a black color
	HR(g_pd3dDevice -> Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0));

	HR(g_pd3dDevice->BeginScene());

	SetupViewMatrices();
	gpRubikCube->OnRender(current_side, orientation,useShaders, selectedCubes->GetCubes());
	HR(g_pd3dDevice->EndScene());	

    // Present the backbuffer to the display.
    g_pd3dDevice -> Present(NULL, NULL, NULL, NULL);
}

//------------------------------------------------------------------------------
// Initialise Direct Input (& terminate initialisation if an error is detected.)
BOOL WINAPI SetupDirectInput(HINSTANCE g_hinst, HWND g_hwndMain)
{
    HRESULT hr;

    // Create the DirectInput object.
    hr = DirectInput8Create(g_hinst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDI, NULL);
    if FAILED(hr)
		return FALSE;

    hr = g_pDI -> CreateDevice(GUID_SysKeyboard, &g_pDIKeyboardDevice, NULL);
    if FAILED(hr)
    {
        CleanUpDirectInput();
        return FALSE;
    }

    // Set the data format using the predefined mouse data format.
    hr = g_pDIKeyboardDevice -> SetDataFormat(&c_dfDIKeyboard);
    if FAILED(hr)
    {
        CleanUpDirectInput(); // Terminate Direct Input initialisation if an error is detected.
        return FALSE;
    }

    // Set the cooperative level
    hr = g_pDIKeyboardDevice -> SetCooperativeLevel(g_hwndMain,
													DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if FAILED(hr)
    {
        CleanUpDirectInput();
        return FALSE;
    }

    return TRUE;
}


//------------------------------------------------------------------------------------------
// Process keyboard inputs...
void WINAPI ProcessKeyboardInput()
{
	static bool tabPressed(false),pg_down_pressed(false),pg_up_pressed(false),backspacePressed(false),deletePressed(false);

	// Define a macro to represent the key detection predicate.
	#define KEYDOWN(name, key) (name[key] & 0x80)	
	
 	// Create a buffer (memory space) to contain the key press data from the keyboard.
	char     buffer[256];
	HRESULT  hr;

    // Clear the keyboard data buffer - just in case.
	ZeroMemory(buffer, 256);

    // What is the current state of the keyboard?
	hr = g_pDIKeyboardDevice -> GetDeviceState(sizeof(buffer),(LPVOID)&buffer);
	if FAILED(hr)
	{
		// If this failed, the device has probably been lost.
		// Check for (hr == DIERR_INPUTLOST) and attempt to reacquire it here.
		hr = g_pDIKeyboardDevice -> Acquire();
        while (hr == DIERR_INPUTLOST)
		{
			hr = g_pDIKeyboardDevice -> Acquire();
		}
		hr = g_pDIKeyboardDevice -> GetDeviceState(sizeof(buffer),(LPVOID)&buffer);
	}

	// Check that the specific keys been pressed
	if(auto_rotate == false)
	{
		if (KEYDOWN(buffer, DIK_BACKSPACE)) backspacePressed= true;
		else if(backspacePressed)
		{
			backspacePressed = false;			
			auto_rotate = autoUndoOneRandomMoveAndSetAngles();			
		}

		if (KEYDOWN(buffer, DIK_DELETE)) 
		{
			deletePressed= true;
		}
		else if(deletePressed)
		{
			deletePressed = false;
			autoCompleteRandom = true;
		}

		if (KEYDOWN(buffer, DIK_TAB))
		{
			tabPressed= true;
		}
		else if(tabPressed)
		{
			if(selectedCubes != 0)
			{
				if(selectedCubes->CanSnap())
				{ 
					selectedCubes->SnapToPositions();
					delete selectedCubes; selectedCubes = 0;
				}
			}
			if(selectedCubes == 0)
			{
				switch(current_side)
				{
					case NONE:				current_side = FRONT;					selectedCubes = new RubiksCubeTransformer(D3DXVECTOR3(0, 0,-1),	gpRubikCube->GetCubesToRotate(D3DXVECTOR3(0,0,-1)));   		break;
					case FRONT:				current_side = BACK; 					selectedCubes = new RubiksCubeTransformer(D3DXVECTOR3(0, 0,1),	gpRubikCube->GetCubesToRotate(D3DXVECTOR3(0,0,1)));	   		break;
					case BACK:				current_side = TOP;	 					selectedCubes = new RubiksCubeTransformer(D3DXVECTOR3(0, 1,0),	gpRubikCube->GetCubesToRotate(D3DXVECTOR3(0,1,0)));	   		break;
					case TOP:				current_side = BOTTOM;					selectedCubes = new RubiksCubeTransformer(D3DXVECTOR3(0,-1,0),	gpRubikCube->GetCubesToRotate(D3DXVECTOR3(0,-1,0)));   		break;
					case BOTTOM:			current_side = LEFT;					selectedCubes = new RubiksCubeTransformer(D3DXVECTOR3(-1,0,0),	gpRubikCube->GetCubesToRotate(D3DXVECTOR3(-1,0,0)));   		break;
					case LEFT:				current_side = RIGHT;					selectedCubes = new RubiksCubeTransformer(D3DXVECTOR3(1,0,  0), 	gpRubikCube->GetCubesToRotate(D3DXVECTOR3(1,0,0)));	   		break;
					case RIGHT:				current_side = HORIZONTAL_MIDDLE;		selectedCubes = new RubiksCubeTransformer(D3DXVECTOR3(0,1,0),		gpRubikCube->GetCubesToRotate(D3DXVECTOR3(0,1,0),true));    break;
					case HORIZONTAL_MIDDLE:	current_side = VERTICAL_MIDDLE;			selectedCubes = new RubiksCubeTransformer(D3DXVECTOR3(1,0,0), 	gpRubikCube->GetCubesToRotate(D3DXVECTOR3(1,0,0),true));    break;
					case VERTICAL_MIDDLE:	current_side = NONE;		break;
				}
			}
			tabPressed = false;
			if(gpRubikCube != 0) finshed = gpRubikCube->IsComplete();
		}

		if (KEYDOWN(buffer, DIK_PGUP)) 
		{	
			if(selectedCubes != 0) selectedCubes->Rotate(1);
		}

		if (KEYDOWN(buffer, DIK_PGDN)) 
		{
			
			if(selectedCubes != 0) selectedCubes->Rotate(-1);
		}    

		if (KEYDOWN(buffer, DIK_R))
		{

			if(selectedCubes != 0)
			{
				if(selectedCubes->CanSnap())
				{ 
					selectedCubes->SnapToPositions();
					delete selectedCubes; selectedCubes = 0;
				}
			}
			alterCubeByRandom(1);
			finshed = false;
		}
	}
	
	// Rotate cube
	if (KEYDOWN(buffer, DIK_S))
	{		
		RotateMatrix(&orientation,-0.1f,0.0f,0.0f);
		SetupViewMatrices();
	}

	if (KEYDOWN(buffer, DIK_W))
	{ 
		RotateMatrix(&orientation, 0.1f,0.0f,0.0f); // Back
		SetupViewMatrices();
	}

	// rotate cube right
	if (KEYDOWN(buffer, DIK_D))
	{		
		RotateMatrix(&orientation,0.0f,-0.1f,0.0f); // Right
	}

	// rotate cube left
	if (KEYDOWN(buffer, DIK_A))
	{ 
		RotateMatrix(&orientation, 0.0f,+0.1f,0.0f); // Left		
	}

	//RESET VIEW
	if (KEYDOWN(buffer, DIK_SPACE))
	{
		D3DXMatrixIdentity(&orientation); // Reset		
	}

	//finalise side rotation
	if (KEYDOWN(buffer, DIK_R))
	{
		if(selectedCubes != 0) 
		{
			selectedCubes->SnapToPositions();
			delete selectedCubes; selectedCubes = 0;
		}
		
		alterCubeByRandom();		
	}

	if (KEYDOWN(buffer, DIK_RETURN))
	{
		if(selectedCubes != 0) 
		{
			selectedCubes->SnapToPositions();
			delete selectedCubes; selectedCubes = 0;
		}	
	}

	if (KEYDOWN(buffer, DIK_F1))
	{
		showControls();	
	}	
}


//-----------------------------------------------------------------------------
// The window's message handling function.
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
	
		case WM_SIZE:
		{					
			screenWidth = (float)LOWORD( lParam );
			screenHeight = (float)HIWORD( lParam );
			SetupViewMatrices();
			break;
		}
        case WM_DESTROY:
		{
            // Respond to a Windows destroy event.
			// Usually generated by clicking on the close box on the window.
            PostQuitMessage(0);
            return 0;
		}
	}

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
// WinMain() - The application's entry point.
// This sort of procedure is mostly standard, and could be used in most DirectX applications.
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
    // Register the window class
	// Register the window class.
	WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = MsgProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = sizeof(LONG_PTR);
	wc.hInstance     = hInst;
	wc.hbrBackground = NULL;
	// The main menu is no longer required.
	wc.lpszMenuName  =  NULL; // MAKEINTRESOURCE(IDR_MAIN);
	wc.hCursor       = NULL;
	
	wc.lpszClassName = "Cube";
	wc.hIconSm = (HICON)::LoadImage(NULL, "cube.ico", IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_LOADFROMFILE);
	wc.hIcon = (HICON)::LoadImage(NULL, "cube.ico", IMAGE_ICON, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), LR_LOADFROMFILE);

    RegisterClassEx(&wc);
	HWND hWnd = CreateWindow( "Cube", "Cube", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 999, 545, NULL, NULL, hInst, NULL);
    
	D3DXMatrixIdentity(&orientation);
	hwnd = hWnd;
	inst = hInst;
	SetupViewMatrices();
	
    if (SUCCEEDED(SetupD3D(hWnd)))
    {        		
		// Initialise Direct Input and acquire the keyboard
		if (SUCCEEDED(SetupDirectInput(hInst, hWnd)))
		{				
			//MessageBox(hwnd,"Keyboard Configuration\n\nCURSOR:\tRotate Whole Cube\nSPACE:\tReset View\n\nTab:\tCycle Through Rotatable Sections/Complete Rotation\n\tPage Up Or Page Down Keys:\tRotate\n\nR:\tHOLD to rapidly rotate sections\n\nQ:\tTo Quit","Controls",MB_OK | MB_ICONINFORMATION);
			// Show the window
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);
			setupOptions();		

			// Enter the message loop
			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
						
					Render();
					ProcessKeyboardInput();
				}
			}            
        }
    }
	
	CleanUp();

    UnregisterClass("Cube", wc.hInstance);
    return 0;
}



