/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

/* functions BKL */
float textureFlashFactor = 1.0f;
Texture2D normalTexture;
sampler NormalTexture = sampler_state
{ 
	MinFilter = LINEAR; 
	MagFilter = LINEAR; 
	Texture	  = (normalTexture);
};

Texture2D diffuseTexture;
sampler DiffuseTexture = sampler_state
{ 
	MinFilter = LINEAR; 
	MagFilter = LINEAR; 
	Texture	  = (diffuseTexture);
};

float3 textureBlendFactor = float3(1.0f,1.0f,1.0f);

float4 ambientLight()
{
	return float4(0.1f,0.1f,0.1f,1.0f);
}

//Kd(L.N)Id
//L = unit vector pointing towards the Light from the surface
//N = normal unit vector of the surface
//Kd = ratio of incoming light which is reflected
//Id = intensity of light r,g,b
//this implemented as a function in case there is more than one Light
float3 diffuseLight(in float3 L, in float3 N, in float3 Id, in float Kd)
{	
	return float3(Kd*max(dot(L,N), 0.f)*Id);
}

//specular light
//Ks(R.V)^a * Is
//R = unit vector pointing towards the Light from the surface
//V = viewer direction
//L = unit vector pointing towards the Light from the surface
//N = normal unit vector of the surface
//Ks = specular material property i.e. colour
//Is = intensity of light r,g,b
//this implemented as a function in case there is more than one Light
float3 specularLight(in float3 V,in float3 L, in float3 N, in float3 Is, in float Ks, in float a)
{		
	//the light needs to be normalised
	float3 R = reflect(L,N);
	float  dotRV = dot(R,V);
	float  power = pow(dotRV,a);
	float3  colour1 = Ks*power*Is;
	
	return float3(saturate(colour1));
}


/*end of functions */
float4x4 InvObjWorldView;
float4x4 ObjWorldView;
float4x4 ObjWorldViewProj;
float4x4 WorldViewProj : WorldViewProjection;
						 
//create an output structure containing the normal in the texture coordinate
struct VS_OUT
{
	float4 pos		: POSITION;
	float3  light 		: TEXCOORD1;
	float2 diffTexture	: TEXCOORD0;
	float3 viewer : TEXCOORD2;
};


VS_OUT mainVS(in float4 pos : POSITION,  in float3 N:NORMAL,in float2 text : TEXCOORD0, in float3 tangent: TEXCOORD1, in float3 cotangent : TEXCOORD2)
{	
	VS_OUT vs_out;	
	float3 position = mul(pos, ObjWorldView);
	float3 light = normalize( float3(0.0f, 0.0f, 10.0f) - position);
	vs_out.pos = mul(pos, ObjWorldViewProj);	
	vs_out.diffTexture = text;
	// WorldSpace To ObjectSpace To TangentSpace float4x4( float4(tangent.xyz, 0.f), float4(cotangent.xyz, 0.f), float4(N.xyz, 0.f), float4(0.f,0.f,0.f,1.0f))
	float4x4 tangentSpace = mul( InvObjWorldView , float4x4( float4(tangent.x, cotangent.x, N.x, 0.f), float4(tangent.y, cotangent.y, N.y, 0.f), float4(tangent.z, cotangent.z, N.z, 0.f), float4(0.f,0.f,0.f,1.0f)));
						  
	vs_out.light = normalize(mul(float4(light.xyz,0.f), tangentSpace));
	vs_out.viewer = normalize(mul(float4(light.xyz,0.f), tangentSpace));
	return vs_out;
}

float4 mainPS(in VS_OUT vs) : COLOR 
{	
	//calculate the light vector
	float3 V = float3(0.0f,0.0f,1.0f);
	float3 Is = float3(1.0f,1.0f,1.0f);
	const float  Ks = 0.5f;
	const float  a = 32.0f;
	float3 diffuseColour = tex2D(DiffuseTexture,vs.diffTexture);	
	diffuseColour *= textureBlendFactor;
	
	float3 texNormal = 2.0f*(tex2D(NormalTexture,vs.diffTexture) - float3(0.5f,0.5f,0.5f));	//
	return float4( diffuseColour*diffuseLight(vs.light, texNormal, Is, 1.f) + specularLight(vs.viewer,vs.light, texNormal, Ks, 1.0f, a), 1.f);
	return float4( vs.light, 1.0f);
}

technique technique0 {
	pass p0 {
		//CullMode = cw;
// Set render states
		Lighting = FALSE;
		ZEnable = TRUE; // We want z-buffering enabled

		Sampler[0] = DiffuseTexture;
		Sampler[1] = NormalTexture;
		VertexShader = compile vs_2_0 mainVS();
		PixelShader = compile ps_2_0 mainPS();
	}
}

