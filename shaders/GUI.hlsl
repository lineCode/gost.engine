Texture2D tex2d_1;
SamplerState tex2D_sampler_1;

cbuffer cbPixel{
	float opacity;
	float3 padding;
};

struct VSIn{
    float4 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

struct VSOut{
    float4 pos : SV_Position;
	float2 uv : TEXCOORD0;
};

struct PSOut{
    float4 color : SV_Target;
};

VSOut VSMain(VSIn input)
{
    VSOut output;
	output.pos = input.position;
	output.uv = input.uv;
	
	return output;
}



PSOut PSMain(VSOut input)
{
    PSOut output;
	
	float4 diffuseColor = tex2d_1.Sample(tex2D_sampler_1, input.uv);
	
    output.color = diffuseColor;
	
//	if( output.color.a < opacity ) discard;
	
    return output;
}