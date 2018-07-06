Texture2D tex2d_1;
SamplerState tex2D_sampler_1;

cbuffer cbPixel{
	float4 diffuseColor;
	bool boolean[128];
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
	
    output.color = tex2d_1.Sample(tex2D_sampler_1, input.uv) * float4( diffuseColor.r, diffuseColor.g, diffuseColor.b, 1.f);
	
	if( boolean[0] ){
		if( output.color.a < 0.5f ){
			discard;
		}
		output.color.a = 1.f - diffuseColor.a;
	}else{
		output.color.a *= diffuseColor.a;
	}

	
    return output;
}