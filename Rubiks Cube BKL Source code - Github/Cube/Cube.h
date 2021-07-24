//check the class has not already been declared
#ifndef BOX_HEADER
#define BOX_HEADER

//include directx header file for vector operations
#define DIRECTINPUT_VERSION 0x0800
#define D3D_DEBUG_INFO	// Enable debugging information, so the .NET environment can help you.
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZW | D3DFVF_NORMAL | D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE2(0) |  D3DFVF_TEXCOORDSIZE4(1))

//-----------------------------------------------------------------------------
// Include these files
#include <windows.h>	// Windows library (for window functions, menus, dialog boxes, etc)
#include <d3dx9.h>		// Direct 3D library (for all Direct 3D functions)
#include <math.h>		//for sin, cos, tan and sqrt

// A structure for our custom vertex type
#ifndef MYCUSTOMVERTEX
#define MYCUSTOMVERTEX
struct CUSTOMVERTEX
{
    D3DXVECTOR4 position;	// Position 
	D3DXVECTOR3 normal;		// Normal
	D3DXVECTOR2 texCoord;	// Texture co-ordinates.
	D3DXVECTOR4 colour;
};
#endif

class Cube
{
public:
	Cube(D3DXMATRIX& view, D3DXMATRIX& proj);
	~Cube();

	void init(LPDIRECT3DDEVICE9 device);
	
	void SetViewProjectionMatrix(D3DXMATRIX& viewIn, D3DXMATRIX& projIn);
	void SetObjectWorldViewTransfrom( D3DXMATRIX matIn);

	// to create the special effect for the rubiks cube
	HRESULT createEffect(void);

	//set the parameters
	void getHandlesToParameters();


	//set Colour of face
	void setFlashFactor(FLOAT colour);

	//set diffuse texture of face
	void setDiffuseTexture();

	//set the modelviewproj matrix
	void setModelWorldViewProjMatrix(D3DXMATRIX matrix);	

	//set effect technique
	HRESULT Cube::setEffectTechnique(void);
	
	HRESULT setupEffect();
	//render geometry with effect
	HRESULT RenderWithEffect(D3DXMATRIX g_orientation,D3DXMATRIX orientation, D3DXVECTOR3 position, FLOAT flashFactor);
private:

	bool setupTextures();
	D3DXMATRIX tranScale;
	float scaler;
	D3DXVECTOR3 vPos;
	D3DXMATRIX& view, & proj;
	LPD3DXMESH m_mesh;
	LPDIRECT3DDEVICE9       l_g_pd3dDevice; // The rendering device
	LPDIRECT3DVERTEXBUFFER9 l_pVertexBuffer; // Buffers to hold faces
	LPDIRECT3DINDEXBUFFER9  m_IndexBuffer;
	LPDIRECT3DTEXTURE9		diffuseMap; // The texture.
	ID3DXEffect* myEffect;				//the shader for the cube
	LPD3DXBUFFER* ppCompilationErrors; 	//A buffer to hold the compilation errors
	D3DXHANDLE				diffuseMapHandleOfEffect;
	D3DXHANDLE				ObjWorldViewProjMatrixHandleOfEffect;
	D3DXHANDLE				flashHandleOfEffect;
	D3DXHANDLE				WorldViewProjMatrixHandleOfEffect;
	D3DXHANDLE				objWorldViewMatrixHandleOfEffect;
};
#endif