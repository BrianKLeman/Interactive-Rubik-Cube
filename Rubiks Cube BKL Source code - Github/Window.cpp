#include "Window.h"

D3DWindow::D3DWindow(HINSTANCE hInst, HINSTANCE pInstance)
	: mInst(hInst), mpInst(pInstance)
{
	// Register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = &D3DWindow::MsgProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = sizeof(LONG_PTR);
	wc.hInstance = hInst;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.hCursor = NULL;

	wc.lpszClassName = "Cube";
	wc.hIconSm = (HICON)::LoadImage(NULL, "cube.ico", IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_LOADFROMFILE);
	wc.hIcon = (HICON)::LoadImage(NULL, "cube.ico", IMAGE_ICON, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), LR_LOADFROMFILE);

	RegisterClassEx(&wc);
	mHwnd = CreateWindow("Cube", "Cube", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 999, 545, NULL, NULL, hInst, NULL);

}

HRESULT D3DWindow::SetupD3D()
{
	// Create the D3D object, return failure if this can't be done.
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION))) 
		return E_FAIL;

	// Set up the structure used to create the D3DDevice
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mHwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &mpD3DDevice)))
	{
		MessageBox(mHwnd, "Error", "Error - Direct3D Device Hardware Acceleration Failed", MB_OK);
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mHwnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &mpD3DDevice)))
		{
			MessageBox(mHwnd, "Error", "Error - Direct3D Device Software Failed", MB_OK);

			return E_FAIL;
		}
	}

	OnSetupD3D();

	D3DVIEWPORT9 vp;
	mpD3DDevice->GetViewport(&vp);
	HR(mpD3DDevice->GetRenderTarget(0, &gpPrimaryRenderTarget));
	return S_OK;
}

//-----------------------------------------------------------------------------
// Terminate Direct Input and clean up (always unacquire device before calling Release())
void WINAPI D3DWindow::CleanUpDirectInput()
{
	if (g_pDI != NULL)
	{
		// Release the mouse.
		if (g_pDIMouseDevice != NULL)
		{
			g_pDIMouseDevice->Unacquire();
			g_pDIMouseDevice->Release();
			g_pDIMouseDevice = NULL;
		}

		// Release the keyboard
		if (mpKeyboardDevice != NULL)
		{
			mpKeyboardDevice->Unacquire();
			mpKeyboardDevice->Release();
			mpKeyboardDevice = NULL;
		}

		// Release the Direct Input Object.
		g_pDI->Release();
		g_pDI = NULL;
	}
}

//-----------------------------------------------------------------------------
// Release (delete) all the resources used by this program.
VOID D3DWindow::CleanUp()
{
	CleanUpDirectInput();

	if (mpD3DDevice != NULL)
	{
		mpD3DDevice->Release();
		mpD3DDevice = NULL;
	}

	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	OnCleanUp();


	UnregisterClass("Cube", mInst);
}

//-----------------------------------------------------------------------------
// Render the scene.
void D3DWindow::Render()
{
	HR(mpD3DDevice->SetRenderTarget(0, gpPrimaryRenderTarget));


	// Clear the backbuffer to a black colour
	HR(mpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0));

	HR(mpD3DDevice->BeginScene());

	OnRender();
	HR(mpD3DDevice->EndScene());

	// Present the backbuffer to the display.
	mpD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//------------------------------------------------------------------------------
// Initialise Direct Input (& terminate initialisation if an error is detected.)
BOOL WINAPI D3DWindow::SetupDirectInput()
{
	HRESULT hr;

	// Create the DirectInput object.
	hr = DirectInput8Create(mInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDI, NULL);
	if FAILED(hr)
		return FALSE;

	hr = g_pDI->CreateDevice(GUID_SysKeyboard, &mpKeyboardDevice, NULL);
	if FAILED(hr)
	{
		CleanUpDirectInput();
		return FALSE;
	}

	// Set the data format using the predefined mouse data format.
	hr = mpKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	if FAILED(hr)
	{
		CleanUpDirectInput(); // Terminate Direct Input initialisation if an error is detected.
		return FALSE;
	}

	// Set the cooperative level
	hr = mpKeyboardDevice->SetCooperativeLevel(mHwnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
	{
		CleanUpDirectInput();
		return FALSE;
	}

	return TRUE;
}


//------------------------------------------------------------------------------------------
// Process keyboard inputs...
void WINAPI D3DWindow::ProcessKeyboardInput()
{
	static bool tabPressed(false), pg_down_pressed(false), pg_up_pressed(false), backspacePressed(false), deletePressed(false);

	

// Create a buffer (memory space) to contain the key press data from the keyboard.
	char     buffer[256];
	HRESULT  hr;

	// Clear the keyboard data buffer - just in case.
	ZeroMemory(buffer, 256);

	// What is the current state of the keyboard?
	hr = mpKeyboardDevice->GetDeviceState(sizeof(buffer), (LPVOID)&buffer);
	if FAILED(hr)
	{
		// If this failed, the device has probably been lost.
		// Check for (hr == DIERR_INPUTLOST) and attempt to reacquire it here.
		hr = mpKeyboardDevice->Acquire();
		while (hr == DIERR_INPUTLOST)
		{
			hr = mpKeyboardDevice->Acquire();
		}
		hr = mpKeyboardDevice->GetDeviceState(sizeof(buffer), (LPVOID)&buffer);
	}

	OnProcessKeyboardInput(buffer);
}

