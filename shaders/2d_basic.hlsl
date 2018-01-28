Texture2D tex2d_1;
SamplerState tex2D_sampler_1;

cbuffer cbVerts{
	float4 v1;
	float4 v2;
	float4 v3;
	float4 v4;
	float2 t1;
	float2 t2;
	float2 t3;
	float2 t4;
};

cbuffer cbPixel{
	float opacity;
	float3 padding;
};

struct VSIn{
    uint vertexId : SV_VertexID;
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
   
	if( input.vertexId == 0 ){
        output.pos = v1;
		output.uv = t1;
    }else if ( input.vertexId == 1 ){
		output.pos = v2;
		output.uv = t2;
	}else if ( input.vertexId == 2 ){
		output.pos = v3;
		output.uv = t3;
	}else if ( input.vertexId == 3 ){
		output.pos = v1;
		output.uv = t1;
	}else if ( input.vertexId == 4 ){
		output.pos = v3;
		output.uv = t3;
	}else if ( input.vertexId == 5 ){
		output.pos = v4;
		output.uv = t4;
	}
	 
    return output;
}



PSOut PSMain(VSOut input)
{
    PSOut output;
	
	float4 diffuseColor = tex2d_1.Sample(tex2D_sampler_1, input.uv);
	
    output.color = diffuseColor;
	
	if( output.color.a < opacity ) discard;
	
    return output;
}