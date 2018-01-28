Texture2D tex2d_1;
SamplerState tex2D_sampler_1;

cbuffer cbPerObject{
	float4x4 WVP;
};

struct VSIn{
    float4 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

struct VSOut{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL;
};

VSOut VSMain(VSIn input)
{
	VSOut output;
	output.uv = input.uv;
	
	//output.uv.y = 1-output.uv.y;
	
	output.position = mul( input.position, WVP );
	output.normal = input.normal;
	
    return output;
}



float4 PSMain(VSOut input) : SV_TARGET
{
    float4 diffuseColor = tex2d_1.Sample(tex2D_sampler_1, input.uv);
	
    float4 color = diffuseColor;
	
    return color;
}