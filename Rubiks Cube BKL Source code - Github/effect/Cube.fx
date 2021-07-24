

/* functions BKL */
float textureFlashFactor = 1.0f;

Texture2D diffuseTexture;
sampler DiffuseTexture = sampler_state
{ 
	MinFilter = LINEAR; 
	MagFilter = LINEAR; 
	Texture	  = (diffuseTexture);
};


float4x4 ObjWorldView;
float4x4 ObjWorldViewProj;
						 
struct VS_OUT
{
	float4 position : POSITION;
	float3 lightDirection : TEXCOORD1;
	float3 normal : TEXCOORD2;
	float2 uv : TEXCOORD0;
	float4 colour : COLOR;
};


VS_OUT mainVS(in float4 pos : POSITION, in float3 N:NORMAL,in float2 uv : TEXCOORD0, in float4 colour : COLOR)
{	
	VS_OUT vs_out;	
	vs_out.position = mul(pos, ObjWorldViewProj);
	vs_out.lightDirection = normalize( float3(0.0f, 0.0f, 10.0f) - pos.xyz);	
	vs_out.uv = uv;
	vs_out.normal = N;
	vs_out.colour = colour;
	return vs_out;
}

//Kd(L.N)Id
//L = unit vector pointing towards the Light from the surface
//N = normal unit vector of the surface
//Kd = ratio of incoming light which is reflected
//Id = intensity of light r,g,b
//this implemented as a function in case there is more than one Light
float3 diffuseLight(in float3 L, in float3 N, in float3 Id, in float Kd)
{	
	return float3(Kd*max(dot(L,N), 0.0f)*Id);
}

float4 mainPS(in VS_OUT vs) : COLOR 
{	
	float3 diffuse = diffuseLight(vs.lightDirection, vs.normal, float3(1.0f,1.0f,1.0f), 1.0f);	
	return float4(vs.colour.xyz, 1.0f);
	//return float4(textureColour.rgb*vs.colour.rgb+diffuse.rgb, 1.0f);
}



technique technique0 {
	pass p0 {
		// Set render states
		Lighting = FALSE;
		ZEnable = TRUE;
		Sampler[0] = DiffuseTexture;
		VertexShader = compile vs_2_0 mainVS();
		PixelShader = compile ps_2_0 mainPS();
	}
}

