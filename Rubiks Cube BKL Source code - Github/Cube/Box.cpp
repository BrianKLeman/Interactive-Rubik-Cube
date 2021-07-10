//----------------------------------------------------------------------------------------
//the implementation of cBox
//created 25/03/08
//----------------------------------------------------------------------------------------

#include "Box.h" //add the declaration of the view box

Box::Box(D3DXMATRIX& view, D3DXMATRIX& proj):
scaler(1.0),vPos(0.0,0.0,0.0),l_g_pd3dDevice(0),
l_pVertexBuffer(0), l_pTexture1(0), view(view), proj(proj)
{
	D3DXMATRIX tran;
	D3DXMatrixIdentity(&tran);
	D3DXMatrixTranslation(&tran, vPos.x, vPos.y, vPos.z);
	D3DXMATRIX scale;
	D3DXMatrixIdentity(&scale);
	D3DXMatrixScaling(&scale,scaler,scaler,scaler);
	D3DXMatrixMultiply(&tranScale,&scale,&tran );

	ppCompilationErrors = new LPD3DXBUFFER;
}

Box::~Box()
{
		// Delete the texture.
	if (l_pTexture1 != nullptr)	 l_pTexture1 -> Release();

	// Release the other resources...
    if (l_pVertexBuffer != nullptr) l_pVertexBuffer -> Release();
	
}

//-----------------------------------------------------------------------------------------
//methods
//-----------------------------------------------------------------------------------------
void Box::init(LPDIRECT3DDEVICE9 device)
{
	l_g_pd3dDevice = device;
	//setup the geometry
	SetupGeometry();
	//set up the textures
	setupTextures();
}

HRESULT Box::SetupGeometry()
{
	// Calculate the number of vertices required, and the size of the buffer to hold them.
	int Vertices = 36;	// 6
	int BufferSize = Vertices * sizeof(CUSTOMVERTEX);

	// Create the vertex buffer.
	if (FAILED(l_g_pd3dDevice -> CreateVertexBuffer(BufferSize, 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &l_pVertexBuffer, NULL)))
	{
		return E_FAIL; // if the vertex buffer could not be created.
	}

	// Fill the buffer with appropriate vertices to describe the cube...

	// Create a pointer to the first vertex in the buffer.
	CUSTOMVERTEX *pVertices;
    if (FAILED(l_pVertexBuffer -> Lock(0, 0, (void**)&pVertices, 0)))
    {
		return E_FAIL;  // if the pointer to the vertex buffer could not be established.
	}

	// Fill the vertex buffers with data...
	const float length = 0.5;
	//        1--------------2
	//       /|             /|
	//      / |            / |
    //     /  |           /  |
	//    0---+----------3   |
	//    |   5----------+---6
	//    |  /           |  /
	//    | /            | /
	//    |/             |/
	//    4--------------7

	D3DXVECTOR4 cubeVertices[8] = 
	{
		D3DXVECTOR4(-length, length, length,1.0f),
		D3DXVECTOR4(-length, length,-length,1.0f),
		D3DXVECTOR4( length, length,-length,1.0f),
		D3DXVECTOR4( length, length, length,1.0f),

		D3DXVECTOR4(-length,-length, length,1.0f),
		D3DXVECTOR4(-length,-length,-length,1.0f),
		D3DXVECTOR4( length,-length,-length,1.0f),
		D3DXVECTOR4( length,-length, length,1.0f)
	};

	D3DXVECTOR3 normals[6] = 
	{
		D3DXVECTOR3(1.f, 0.f, 0.f), // Right = 0
		D3DXVECTOR3(-1.f, 0.f, 0.f), // Left = 1

		D3DXVECTOR3(0.f, 0.f, 1.f), // Forward = 2
		D3DXVECTOR3(0.f, 0.f,-1.f), // Backward =3

		D3DXVECTOR3(0.f, 1.f, 0.f), // Up = 4
		D3DXVECTOR3(0.f,-1.f, 0.f)  // Down = 5
	};
	enum NORMALS { N_RIGHT = 0, N_LEFT = 1, N_FORWARD = 2, N_BACKWARD=3, N_UP = 4, N_DOWN = 5};

	D3DXVECTOR2 texCoords[4] =
	{
		D3DXVECTOR2(0.f, 0.f), // Top Left Corner = 0
		D3DXVECTOR2(1.f, 0.f), // Top Right Corner = 1
		D3DXVECTOR2(0.f, 1.f), // Bottom Left Corner = 2
		D3DXVECTOR2(1.f, 1.f)  // Bottom Right Corner = 3
	};
	enum TEX_COORDS { TEXCOORD_TOPLEFT = 0, TEXCOORD_TOPRIGHT = 1, TEXCOORD_BOTTOMLEFT = 2, TEXCOORD_BOTTOMRIGHT = 3};

	int vertexIndex = 0;
	// Side 1 - Front face
	pVertices[vertexIndex].position = cubeVertices[0];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_TOPLEFT];
	pVertices[vertexIndex].normal = normals[N_FORWARD];
	pVertices[vertexIndex].tangent = normals[N_RIGHT];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	pVertices[++vertexIndex].position = cubeVertices[3];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_TOPRIGHT];
	pVertices[vertexIndex].normal = normals[N_FORWARD];
	pVertices[vertexIndex].tangent = normals[N_RIGHT];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	pVertices[++vertexIndex].position = cubeVertices[4];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_BOTTOMLEFT];
	pVertices[vertexIndex].normal = normals[N_FORWARD];
	pVertices[vertexIndex].tangent = normals[N_RIGHT];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	pVertices[++vertexIndex].position = cubeVertices[3];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_TOPRIGHT];
	pVertices[vertexIndex].normal = normals[N_FORWARD];
	pVertices[vertexIndex].tangent = normals[N_RIGHT];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	pVertices[++vertexIndex].position = cubeVertices[7];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_BOTTOMRIGHT];
	pVertices[vertexIndex].normal = normals[N_FORWARD];
	pVertices[vertexIndex].tangent = normals[N_RIGHT];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	pVertices[++vertexIndex].position = cubeVertices[4];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_BOTTOMLEFT];
	pVertices[vertexIndex].normal = normals[N_FORWARD];
	pVertices[vertexIndex].tangent = normals[N_RIGHT];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	// Side 1 - Right face
	pVertices[++vertexIndex].position = cubeVertices[3];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_TOPLEFT];
	pVertices[vertexIndex].normal = normals[N_RIGHT];
	pVertices[vertexIndex].tangent = normals[N_BACKWARD];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	pVertices[++vertexIndex].position = cubeVertices[2];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_TOPRIGHT];
	pVertices[vertexIndex].normal = normals[N_RIGHT];
	pVertices[vertexIndex].tangent = normals[N_BACKWARD];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	pVertices[++vertexIndex].position = cubeVertices[7];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_BOTTOMLEFT];
	pVertices[vertexIndex].normal = normals[N_RIGHT];
	pVertices[vertexIndex].tangent = normals[N_BACKWARD];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	pVertices[++vertexIndex].position = cubeVertices[2];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_TOPRIGHT];
	pVertices[vertexIndex].normal = normals[N_RIGHT];
	pVertices[vertexIndex].tangent = normals[N_BACKWARD];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	pVertices[++vertexIndex].position = cubeVertices[6];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_BOTTOMRIGHT];
	pVertices[vertexIndex].normal = normals[N_RIGHT];
	pVertices[vertexIndex].tangent = normals[N_BACKWARD];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	pVertices[++vertexIndex].position = cubeVertices[7];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_BOTTOMLEFT];
	pVertices[vertexIndex].normal = normals[N_RIGHT];
	pVertices[vertexIndex].tangent = normals[N_BACKWARD];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	// Side 1 - Back face
	pVertices[++vertexIndex].position = cubeVertices[2];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_TOPLEFT];
	pVertices[vertexIndex].normal = normals[N_BACKWARD];
	pVertices[vertexIndex].tangent = normals[N_LEFT];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	pVertices[++vertexIndex].position = cubeVertices[1];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_TOPRIGHT];
	pVertices[vertexIndex].normal = normals[N_BACKWARD];
	pVertices[vertexIndex].tangent = normals[N_LEFT];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	pVertices[++vertexIndex].position = cubeVertices[6];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_BOTTOMLEFT];
	pVertices[vertexIndex].normal = normals[N_BACKWARD];
	pVertices[vertexIndex].tangent = normals[N_LEFT];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	pVertices[++vertexIndex].position = cubeVertices[1];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_TOPRIGHT];
	pVertices[vertexIndex].normal = normals[N_BACKWARD];
	pVertices[vertexIndex].tangent = normals[N_LEFT];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	pVertices[++vertexIndex].position = cubeVertices[5];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_BOTTOMRIGHT];
	pVertices[vertexIndex].normal = normals[N_BACKWARD];
	pVertices[vertexIndex].tangent = normals[N_LEFT];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	pVertices[++vertexIndex].position = cubeVertices[6];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_BOTTOMLEFT];
	pVertices[vertexIndex].normal = normals[N_BACKWARD];
	pVertices[vertexIndex].tangent = normals[N_LEFT];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	// Side 1 - Left face
	pVertices[++vertexIndex].position = cubeVertices[1];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_TOPLEFT];
	pVertices[vertexIndex].normal = normals[N_LEFT];
	pVertices[vertexIndex].tangent = normals[N_FORWARD];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	pVertices[++vertexIndex].position = cubeVertices[0];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_TOPRIGHT];
	pVertices[vertexIndex].normal = normals[N_LEFT];
	pVertices[vertexIndex].tangent = normals[N_FORWARD];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	pVertices[++vertexIndex].position = cubeVertices[5];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_BOTTOMLEFT];
	pVertices[vertexIndex].normal = normals[N_LEFT];
	pVertices[vertexIndex].tangent = normals[N_FORWARD];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	pVertices[++vertexIndex].position = cubeVertices[0];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_TOPRIGHT];
	pVertices[vertexIndex].normal = normals[N_LEFT];
	pVertices[vertexIndex].tangent = normals[N_FORWARD];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	pVertices[++vertexIndex].position = cubeVertices[4];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_BOTTOMRIGHT];
	pVertices[vertexIndex].normal = normals[N_LEFT];
	pVertices[vertexIndex].tangent = normals[N_FORWARD];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	pVertices[++vertexIndex].position = cubeVertices[5];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_BOTTOMLEFT];
	pVertices[vertexIndex].normal = normals[N_LEFT];
	pVertices[vertexIndex].tangent = normals[N_FORWARD];
	pVertices[vertexIndex].coTangent = normals[N_DOWN];

	// Top Face
	pVertices[++vertexIndex].position = cubeVertices[1];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_TOPLEFT];
	pVertices[vertexIndex].normal = normals[N_UP];
	pVertices[vertexIndex].tangent = normals[N_RIGHT];
	pVertices[vertexIndex].coTangent = normals[N_FORWARD];

	pVertices[++vertexIndex].position = cubeVertices[2];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_TOPRIGHT];
	pVertices[vertexIndex].normal = normals[N_UP];
	pVertices[vertexIndex].tangent = normals[N_RIGHT];
	pVertices[vertexIndex].coTangent = normals[N_FORWARD];

	pVertices[++vertexIndex].position = cubeVertices[0];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_BOTTOMLEFT];
	pVertices[vertexIndex].normal = normals[N_UP];
	pVertices[vertexIndex].tangent = normals[N_RIGHT];
	pVertices[vertexIndex].coTangent = normals[N_FORWARD];

	pVertices[++vertexIndex].position = cubeVertices[2];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_TOPRIGHT];
	pVertices[vertexIndex].normal = normals[N_UP];
	pVertices[vertexIndex].tangent = normals[N_RIGHT];
	pVertices[vertexIndex].coTangent = normals[N_FORWARD];

	pVertices[++vertexIndex].position = cubeVertices[3];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_BOTTOMRIGHT];
	pVertices[vertexIndex].normal = normals[N_UP];
	pVertices[vertexIndex].tangent = normals[N_RIGHT];
	pVertices[vertexIndex].coTangent = normals[N_FORWARD];

	pVertices[++vertexIndex].position = cubeVertices[0];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_BOTTOMLEFT];
	pVertices[vertexIndex].normal = normals[N_UP];
	pVertices[vertexIndex].tangent = normals[N_RIGHT];
	pVertices[vertexIndex].coTangent = normals[N_FORWARD];

	// Bottom Face
	pVertices[++vertexIndex].position = cubeVertices[4];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_TOPLEFT];
	pVertices[vertexIndex].normal = normals[N_DOWN];
	pVertices[vertexIndex].tangent = normals[N_RIGHT];
	pVertices[vertexIndex].coTangent = normals[N_BACKWARD];

	pVertices[++vertexIndex].position = cubeVertices[7];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_TOPRIGHT];
	pVertices[vertexIndex].normal = normals[N_DOWN];
	pVertices[vertexIndex].tangent = normals[N_RIGHT];
	pVertices[vertexIndex].coTangent = normals[N_BACKWARD];

	pVertices[++vertexIndex].position = cubeVertices[5];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_BOTTOMLEFT];
	pVertices[vertexIndex].normal = normals[N_DOWN];
	pVertices[vertexIndex].tangent = normals[N_RIGHT];
	pVertices[vertexIndex].coTangent = normals[N_BACKWARD];

	pVertices[++vertexIndex].position = cubeVertices[7];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_TOPRIGHT];
	pVertices[vertexIndex].normal = normals[N_DOWN];
	pVertices[vertexIndex].tangent = normals[N_RIGHT];
	pVertices[vertexIndex].coTangent = normals[N_BACKWARD];

	pVertices[++vertexIndex].position = cubeVertices[6];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_BOTTOMRIGHT];
	pVertices[vertexIndex].normal = normals[N_DOWN];
	pVertices[vertexIndex].tangent = normals[N_RIGHT];
	pVertices[vertexIndex].coTangent = normals[N_BACKWARD];

	pVertices[++vertexIndex].position = cubeVertices[5];
	pVertices[vertexIndex].texCoord = texCoords[TEXCOORD_BOTTOMLEFT];
	pVertices[vertexIndex].normal = normals[N_DOWN];
	pVertices[vertexIndex].tangent = normals[N_RIGHT];
	pVertices[vertexIndex].coTangent = normals[N_BACKWARD];
	// Unlock the vertex buffer...
	l_pVertexBuffer -> Unlock();

	return S_OK;
}

bool Box::setupTextures()
{
	
	D3DXCreateTextureFromFile(l_g_pd3dDevice, "textures/rubik_texture.jpg", &l_pTexture1);
	D3DXCreateTextureFromFile(l_g_pd3dDevice, "textures/rubik_texture.jpg", &alt_texture1);

	D3DXCreateTextureFromFile(l_g_pd3dDevice, "textures/rubik_bump.jpg", &l_pTexture2);
	D3DXCreateTextureFromFile(l_g_pd3dDevice, "textures/rubik_bump.jpg", &alt_texture2);

	if(l_pTexture1 == NULL || alt_texture1 == NULL) 
	{
		MessageBox(0,"Texture","failed to load texture",MB_OK);
		return false;
	}

	if(l_pTexture2 == NULL || alt_texture2 == NULL) 
	{
		MessageBox(0,"Texture","failed to load texture",MB_OK);
		return false;
	}
	return true;
}

void Box::Render(D3DXMATRIX orientation, D3DXVECTOR3 position)
{
		
		
		D3DXMATRIX m,m2;
		
		
		D3DXMatrixTranslation(&m,position.x,position.y,position.z);
		D3DXMatrixMultiply(&m2,&orientation,&m);
		l_g_pd3dDevice->SetTransform(D3DTS_WORLD, &m2);

		// Select the texture, and initialise the texture stage state...
		l_g_pd3dDevice -> SetTexture(0, l_pTexture1);
		l_g_pd3dDevice -> SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		l_g_pd3dDevice -> SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		

		// Render the contents of the vertex buffer.
        l_g_pd3dDevice -> SetStreamSource(0, l_pVertexBuffer, 0, sizeof(CUSTOMVERTEX));
        l_g_pd3dDevice -> SetFVF(D3DFVF_CUSTOMVERTEX);
		l_g_pd3dDevice -> SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);

		//assign material
		l_g_pd3dDevice -> SetMaterial(&Mtl[0]);
		//draw
        l_g_pd3dDevice -> DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
		//assign material
		l_g_pd3dDevice -> SetMaterial(&Mtl[1]);
		//draw
        l_g_pd3dDevice -> DrawPrimitive(D3DPT_TRIANGLELIST, 6, 2);
		//assign material
		l_g_pd3dDevice -> SetMaterial(&Mtl[2]);
		//draw
        l_g_pd3dDevice -> DrawPrimitive(D3DPT_TRIANGLELIST, 12, 2);
		//assign material
		l_g_pd3dDevice -> SetMaterial(&Mtl[3]);
		//draw
        l_g_pd3dDevice -> DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);
		//assign material
		l_g_pd3dDevice -> SetMaterial(&Mtl[4]);
		//draw
        l_g_pd3dDevice -> DrawPrimitive(D3DPT_TRIANGLELIST, 24, 2);
		//assign material
		l_g_pd3dDevice -> SetMaterial(&Mtl[5]);
		//draw
        l_g_pd3dDevice -> DrawPrimitive(D3DPT_TRIANGLELIST, 30, 2);
		

		//l_g_pd3dDevice -> SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
		
}

HRESULT Box::RenderWithEffect(D3DXMATRIX g_orientation,D3DXMATRIX orientation, D3DXVECTOR3 position, FLOAT flashFactor)
{
		HRESULT result;
		
		D3DXMATRIX object, objectWorld, objectWorldView, WorldViewProjectionMatrix;	

		D3DXMATRIX identity;
		D3DXMatrixIdentity(&identity);

		// Set Object World View transform.

		//create worldviewproj matrix
		D3DXMatrixMultiply(&WorldViewProjectionMatrix, &identity, &view	);
		D3DXMatrixMultiply(&WorldViewProjectionMatrix, &WorldViewProjectionMatrix, &proj);
		setWorldViewProjMatrix(WorldViewProjectionMatrix);

		//create modelworld matrix
		D3DXMatrixIdentity(&object);
		
		D3DXMatrixTranslation(&object,position.x,position.y,position.z);
		D3DXMatrixMultiply(&object,&orientation,&object	);
		D3DXMatrixMultiply(&objectWorld,&object,&g_orientation);


		D3DXMatrixMultiply( &objectWorldView, &objectWorld, &view	);
		SetObjectWorldViewTransfrom(objectWorldView);

		D3DXMATRIX inverseObjectWorldView;
		FLOAT determinent = D3DXMatrixDeterminant(&objectWorldView);
		D3DXMatrixInverse( &inverseObjectWorldView, &determinent, &objectWorldView);
		SetInverseObjectWorldViewTransfrom(inverseObjectWorldView);
		//create ModelWorldViewProj
		D3DXMatrixMultiply( &WorldViewProjectionMatrix, &objectWorldView, &proj);
		
		D3DXMATRIX worldViewProjX;

		setModelWorldViewProjMatrix(WorldViewProjectionMatrix);
		

	   	// Render the contents of the vertex buffer.
		l_g_pd3dDevice -> SetFVF(D3DFVF_CUSTOMVERTEX);
	    l_g_pd3dDevice -> SetStreamSource(0, l_pVertexBuffer, 0, sizeof(CUSTOMVERTEX));

		UINT nbPasses(0);
		
		D3DXVECTOR4 colour(1.0f,0.0f,0.0f,1.0f);
		setColourOfFace(&colour);
		setFlashFactor(flashFactor);
		//setOrientationMatrix(orientation);
		myEffect->CommitChanges();
		result = myEffect->Begin(&nbPasses,0);
		for(unsigned int i = 0; i < nbPasses; i++)
		{
			myEffect->BeginPass(i);

	        l_g_pd3dDevice -> DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
			
			colour = D3DXVECTOR4(0.0f,1.0f,0.0f,1.0f);
			setColourOfFace(&colour);
			myEffect->CommitChanges();
	        l_g_pd3dDevice -> DrawPrimitive(D3DPT_TRIANGLELIST, 6, 2);
			
			colour = D3DXVECTOR4(0.0f,0.0f,1.0f,1.0f);
			setColourOfFace(&colour);
			myEffect->CommitChanges();
	        l_g_pd3dDevice -> DrawPrimitive(D3DPT_TRIANGLELIST, 12, 2);
		
			colour = D3DXVECTOR4(1.0f,0.5f,0.0f,1.0f);
			setColourOfFace(&colour);
			myEffect->CommitChanges();
	       l_g_pd3dDevice -> DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);
		
			colour = D3DXVECTOR4(1.0f,1.0f,0.0f,1.0f);
			setColourOfFace(&colour);
			myEffect->CommitChanges();
	        l_g_pd3dDevice -> DrawPrimitive(D3DPT_TRIANGLELIST, 24, 2);
	
			colour = D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f);
			setColourOfFace(&colour);
			myEffect->CommitChanges();
	        l_g_pd3dDevice -> DrawPrimitive(D3DPT_TRIANGLELIST, 30, 2);

			myEffect->EndPass();
		}

		myEffect->End();

		
		return result;
}
void Box::SetMaterials()
{
	ZeroMemory(&Mtl[0], sizeof(D3DMATERIAL9));
	Mtl[0].Diffuse.r = 1;
	Mtl[0].Diffuse.g = 0;
	Mtl[0].Diffuse.b = 0;

	ZeroMemory(&Mtl[1], sizeof(D3DMATERIAL9));
	Mtl[1].Diffuse.r = 0;
	Mtl[1].Diffuse.g = 1;
	Mtl[1].Diffuse.b = 0;

	ZeroMemory(&Mtl[2], sizeof(D3DMATERIAL9));
	Mtl[2].Diffuse.r = 0;
	Mtl[2].Diffuse.g = 0;
	Mtl[2].Diffuse.b = 1;

	ZeroMemory(&Mtl[3], sizeof(D3DMATERIAL9));
	Mtl[3].Diffuse.r = 1;
	Mtl[3].Diffuse.g = 1;
	Mtl[3].Diffuse.b = 1;

	ZeroMemory(&Mtl[4], sizeof(D3DMATERIAL9));
	Mtl[4].Diffuse.r = 1;
	Mtl[4].Diffuse.g = 1;
	Mtl[4].Diffuse.b = 0;

	ZeroMemory(&Mtl[5], sizeof(D3DMATERIAL9));
	Mtl[5].Diffuse.r = 1;
	Mtl[5].Diffuse.g = 1;
	Mtl[5].Diffuse.b = 0.5;

}

// to create the special effect for the rubiks cube
HRESULT Box::createEffect(void)
{
	
	return D3DXCreateEffectFromFile(l_g_pd3dDevice,"effect/Cube.fx",NULL,NULL,NULL ,NULL,&myEffect,ppCompilationErrors);
}

// to create the special effect for the rubiks cube
HRESULT Box::setEffectTechnique(void)
{
	HRESULT result;
	D3DXHANDLE toEffectTechnique;
	result = myEffect->FindNextValidTechnique(NULL,&toEffectTechnique);
	if(result == D3D_OK)
	{
		result = myEffect->SetTechnique(toEffectTechnique);
		
	} 

	return result;
}

// to create the special effect for the rubiks cube
void Box::getHandlesToParameters(void)
{
	
	colourHandleOfEffect = myEffect->GetParameterByName( NULL,"textureBlendFactor");
	if(colourHandleOfEffect == NULL) MessageBox(NULL,"failed to get handle to param","effect failed",MB_OK);

	ObjWorldViewProjMatrixHandleOfEffect = myEffect->GetParameterByName(NULL,"ObjWorldViewProj");
	if(ObjWorldViewProjMatrixHandleOfEffect == NULL) MessageBox(NULL,"failed to get handle to param","effect failed",MB_OK);

	diffuseMapHandleOfEffect = myEffect->GetParameterByName(NULL,"diffuseTexture");
	if(diffuseMapHandleOfEffect == NULL) MessageBox(NULL,"failed to get handle to param","effect failed",MB_OK);

	normalMapHandleOfEffect = myEffect->GetParameterByName(NULL,"normalTexture");
	if(normalMapHandleOfEffect == NULL) MessageBox(NULL,"failed to get handle to param","effect failed",MB_OK);

	flashHandleOfEffect = myEffect->GetParameterByName(NULL,"textureFlashFactor");
	if(normalMapHandleOfEffect == NULL) MessageBox(NULL,"failed to get handle to param","effect failed",MB_OK);

	WorldViewProjMatrixHandleOfEffect = myEffect->GetParameterByName(NULL,"WorldViewProj");
	if(WorldViewProjMatrixHandleOfEffect == NULL) MessageBox(NULL,"failed to get handle to param","effect failed",MB_OK);

	objWorldViewMatrixHandleOfEffect = myEffect->GetParameterByName(NULL,"ObjWorldView");
	if(WorldViewProjMatrixHandleOfEffect == NULL) MessageBox(NULL,"failed to get handle to param","effect failed",MB_OK);

	InvObjWorldView = myEffect->GetParameterByName(NULL,"InvObjWorldView");
	if(InvObjWorldView == NULL) MessageBox(NULL,"failed to get handle to param","effect failed",MB_OK);
	
}

void Box::setFlashFactor(FLOAT colour)
{
	if(myEffect->SetFloat(flashHandleOfEffect,colour) != D3D_OK) MessageBox(NULL,"SET FLASH FACTOR FAILED","COLOUR",MB_OK);
}
HRESULT Box::setupEffect()
{
	HRESULT result;

	result = createEffect();
	
	if(result == D3D_OK)
	{
		getHandlesToParameters();
		
		//set the textures
		setNormalTexture();
		setDiffuseTexture();

		result = setEffectTechnique();
		
		return result;
	}
	return result;


	
}
void Box::setColourOfFace(D3DXVECTOR4* colour)
{
	if(myEffect->SetVector(colourHandleOfEffect,colour) != D3D_OK) MessageBox(NULL,"SET COLOUR FAILED","COLOUR",MB_OK);
}



void Box::setDiffuseTexture()
{
	if(myEffect->SetTexture(diffuseMapHandleOfEffect,alt_texture1) != D3D_OK) MessageBox(NULL,"SET TEXTURE FAILED","TEXTURE",MB_OK);
	

}

void Box::setNormalTexture()
{
	if(myEffect->SetTexture(normalMapHandleOfEffect,alt_texture2) != D3D_OK) MessageBox(NULL,"SET TEXTURE FAILED","TEXTURE",MB_OK);
}

void Box::setModelWorldViewProjMatrix(D3DXMATRIX matrix)
{
	if(myEffect->SetMatrix(ObjWorldViewProjMatrixHandleOfEffect,&matrix) != D3D_OK) MessageBox(NULL,"SET MATRIX FAILED","MATRIX",MB_OK);
	//if(result == D3D_OK) MessageBox(NULL,"SUCCESS ","SUCCESS",MB_OK);
	/*else MessageBox(NULL,"NO_SUCCESS ","NO_SUCCESS",MB_OK);*/
}

//set the modelviewproj matrix
void Box::setWorldViewProjMatrix(D3DXMATRIX matrix)
{
	if(myEffect->SetMatrix(WorldViewProjMatrixHandleOfEffect,&matrix)!=D3D_OK) MessageBox(NULL,"SET ORIENTATION MATRIX FAILED","MATRIX",MB_OK) ;
}

void Box::SetViewProjectionMatrix( D3DXMATRIX& viewIn, D3DXMATRIX& projIn )
{
	view = viewIn; proj = projIn;
}

void Box::SetObjectWorldViewTransfrom( D3DXMATRIX matIn )
{
	if(myEffect->SetMatrix(objWorldViewMatrixHandleOfEffect,&matIn)!=D3D_OK) MessageBox(NULL,"SET ORIENTATION MATRIX FAILED","MATRIX",MB_OK) ;
}

void Box::SetInverseObjectWorldViewTransfrom( D3DXMATRIX matIn )
{
	if(myEffect->SetMatrix(InvObjWorldView,&matIn)!=D3D_OK) MessageBox(NULL,"SET ORIENTATION MATRIX FAILED","MATRIX",MB_OK) ;
}
