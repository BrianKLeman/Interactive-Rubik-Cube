
#include "Cube.h"

Cube::Cube(D3DXMATRIX& view, D3DXMATRIX& proj):
scaler(1.0),vPos(0.0,0.0,0.0),l_g_pd3dDevice(0),
l_pVertexBuffer(0), view(view), proj(proj)
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

Cube::~Cube()
{
	// Release the other resources...
    if (l_pVertexBuffer != nullptr) l_pVertexBuffer -> Release();	
}

//-----------------------------------------------------------------------------------------
//methods
//-----------------------------------------------------------------------------------------
void Cube::init(LPDIRECT3DDEVICE9 device)
{
	l_g_pd3dDevice = device;

	D3DXLoadMeshFromX("effect/cube.x", D3DXMESH_MANAGED, l_g_pd3dDevice, nullptr, nullptr, nullptr, nullptr, &m_mesh);
	m_mesh->GetVertexBuffer(&l_pVertexBuffer);
	m_mesh->GetIndexBuffer(&m_IndexBuffer);
}

HRESULT Cube::RenderWithEffect(D3DXMATRIX g_orientation,D3DXMATRIX orientation, D3DXVECTOR3 position, FLOAT flashFactor)
{
		HRESULT result;
		
		D3DXMATRIX object, objectWorld, objectWorldView, WorldViewProjectionMatrix;	

		D3DXMATRIX identity;
		D3DXMatrixIdentity(&identity);

		// Set Object World View transform.

		//create worldviewproj matrix
		D3DXMatrixMultiply(&WorldViewProjectionMatrix, &identity, &view	);
		D3DXMatrixMultiply(&WorldViewProjectionMatrix, &WorldViewProjectionMatrix, &proj);

		//create modelworld matrix
		D3DXMatrixIdentity(&object);
		D3DXMatrixTranslation(&object,position.x,position.y,position.z);
		D3DXMatrixMultiply(&object,&orientation,&object	);
		D3DXMatrixMultiply(&objectWorld,&object,&g_orientation);


		D3DXMatrixMultiply( &objectWorldView, &objectWorld, &view	);
		SetObjectWorldViewTransfrom(objectWorldView);

		//create ModelWorldViewProj
		D3DXMatrixMultiply( &WorldViewProjectionMatrix, &objectWorldView, &proj);
		
		D3DXMATRIX worldViewProjX;
		setModelWorldViewProjMatrix(WorldViewProjectionMatrix);		

	   	// Render the contents of the vertex buffer.
		l_g_pd3dDevice -> SetFVF(m_mesh->GetFVF());		
	    l_g_pd3dDevice -> SetStreamSource(0, l_pVertexBuffer, 0, m_mesh->GetNumBytesPerVertex());
		UINT nbPasses(0);
		
	
		l_g_pd3dDevice->SetIndices(m_IndexBuffer);
		setFlashFactor(flashFactor);
		myEffect->CommitChanges();
		result = myEffect->Begin(&nbPasses,0);
		for(unsigned int i = 0; i < nbPasses; i++)
		{
			myEffect->BeginPass(i);

	        l_g_pd3dDevice -> DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_mesh->GetNumVertices(),0, m_mesh->GetNumFaces());			
			
			myEffect->EndPass();
		}

		myEffect->End();

		
		return result;
}

// to create the special effect for the rubiks cube
HRESULT Cube::createEffect(void)
{	
	return D3DXCreateEffectFromFile(l_g_pd3dDevice,"effect/Cube.fx",NULL,NULL,NULL ,NULL,&myEffect,ppCompilationErrors);
}

// to create the special effect for the rubiks cube
HRESULT Cube::setEffectTechnique(void)
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
void Cube::getHandlesToParameters(void)
{	
	ObjWorldViewProjMatrixHandleOfEffect = myEffect->GetParameterByName(NULL,"ObjWorldViewProj");
	if(ObjWorldViewProjMatrixHandleOfEffect == NULL) MessageBox(NULL,"failed to get handle to param","effect failed",MB_OK);

	flashHandleOfEffect = myEffect->GetParameterByName(NULL,"textureFlashFactor");
	if(flashHandleOfEffect == NULL) MessageBox(NULL,"failed to get handle to param","effect failed",MB_OK);

	objWorldViewMatrixHandleOfEffect = myEffect->GetParameterByName(NULL,"ObjWorldView");
	if(WorldViewProjMatrixHandleOfEffect == NULL) MessageBox(NULL,"failed to get handle to param","effect failed",MB_OK);

}

void Cube::setFlashFactor(FLOAT colour)
{
	if(myEffect->SetFloat(flashHandleOfEffect,colour) != D3D_OK) MessageBox(NULL,"SET FLASH FACTOR FAILED","COLOUR",MB_OK);
}

HRESULT Cube::setupEffect()
{
	HRESULT result;

	result = createEffect();
	
	if(result == D3D_OK)
	{
		getHandlesToParameters();	
		return setEffectTechnique();		
	}
	return result;
}

void Cube::setModelWorldViewProjMatrix(D3DXMATRIX matrix)
{
	if(myEffect->SetMatrix(ObjWorldViewProjMatrixHandleOfEffect,&matrix) != D3D_OK) MessageBox(NULL,"SET MATRIX FAILED","MATRIX",MB_OK);
}

void Cube::SetViewProjectionMatrix( D3DXMATRIX& viewIn, D3DXMATRIX& projIn )
{
	view = viewIn; proj = projIn;
}

void Cube::SetObjectWorldViewTransfrom( D3DXMATRIX matIn )
{
	if(myEffect->SetMatrix(objWorldViewMatrixHandleOfEffect,&matIn)!=D3D_OK) MessageBox(NULL,"SET ORIENTATION MATRIX FAILED","MATRIX",MB_OK) ;
}
