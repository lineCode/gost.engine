Texture2D tex2d_1;
SamplerState tex2D_sampler_1;

cbuffer cbPerObject{
	float4x4 WVP;
	float2 t1;
	float2 t2;
	float2 t3;
	float2 t4;
};

struct VSIn{
	uint indexId : SV_VertexID;
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
	
	if( input.indexId == 0 ){
		output.uv = t1;
    }else if( input.indexId == 1 ){
		output.uv = t2;
	}else if( input.indexId == 2 ){
		output.uv = t3;
	}else if( input.indexId == 0 ){
		output.uv = t1;
	}else if( input.indexId == 2 ){
		output.uv = t3;
	}else if( input.indexId == 3 ){
		output.uv = t4;
	}
	
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