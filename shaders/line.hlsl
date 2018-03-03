cbuffer cbPerObject{
	float4x4 WVP;
	float4 p1;
	float4 p2;
	float4 color;
};

struct VSIn{
	uint vertexId : SV_VertexID;
};

struct VSOut{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
};

VSOut VSMain(VSIn input)
{
	VSOut output;
	
	if( input.vertexId == 0 ){
		output.position = mul( p1, WVP );
	}else if( input.vertexId == 1 ){
		output.position = mul( p2, WVP );
	}

	output.color = color;

    return output;
}



float4 PSMain(VSOut input) : SV_TARGET
{
    return input.color;
}