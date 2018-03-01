cbuffer cbPerObject{
	float4x4 WVP;
	float4 p1;
	float4 p2;
};

struct VSIn{
	uint indexId : SV_VertexID;
};

struct VSOut{
	float4 position : SV_POSITION;
};

VSOut VSMain(VSIn input)
{
	VSOut output;
	
	output.position = mul( p1, WVP );
	
    return output;
}



float4 PSMain(VSOut input) : SV_TARGET
{
    float4 color = float4 (1.f,1.f,1.f,1.f);
	
    return color;
}