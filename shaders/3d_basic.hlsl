Texture2D tex2d_1;
SamplerState tex2D_sampler_1;

cbuffer cbPerObject{
	float4x4 WVP;
	float4x4 W;
	float4x4 WI;
    float4x4 V;
    float4x4 P;
	float4 	 eye;
};

cbuffer cbMaterial{
	float4 sunAmbient;
	float4 sunDiffuse;
	float4 sunSpecular;
	float4 eyePosition;
	float4 sunPosition;
	
	float  shininess;
	float  opacity;
	int    isLume;
	int    reserved;
};

struct VertexInputType{
    float4 lay_position : POSITION;
	float2 lay_uv       : TEXCOORD;
	float3 lay_normal   : NORMAL;
};

struct PixelInputType{
	float4 position   : SV_POSITION;
	float2 uv         : TEXCOORD0;
	float4 w_position : TEXCOORD1;
	float3 normal     : NORMAL;
};

PixelInputType VSMain( VertexInputType input ){
	input.lay_position.w = 1.0f;

	PixelInputType output;
	output.position = mul( input.lay_position, WVP );
	output.w_position = mul( input.lay_position, W );

	output.uv = input.lay_uv;
	
	//fragNormal = ( V * W * vec4(lay_normal,0)).xyz;
	output.normal = normalize( mul( input.lay_normal, WI ));
	
    return output;
}



float4 PSMain( PixelInputType input ) : SV_TARGET{
	float4 diffuseColor = tex2d_1.Sample(tex2D_sampler_1, input.uv);
	float4 ambientColor = sunAmbient * diffuseColor;
	
	float4 normal = normalize(float4(input.normal,1.0));
	
	float4 sunLightDir = normalize( float4(sunPosition.xyz,1.f) );
	float diff = max(dot( normal, sunLightDir ), 0.0f );
	
	float4 diffuse = sunDiffuse;
	if( isLume == 1 ) diffuse *= diff;
	diffuse *= diffuseColor;
	
	float4 viewDir = normalize( float4(eyePosition.xyz,1.f) - input.w_position );
	float4 reflectDir = reflect( -sunLightDir, normal );
	float spec = pow(max(dot( viewDir, reflectDir), 0.0),shininess);
	float4 specular = sunSpecular * spec * diffuseColor;
	
	float4 color = ambientColor + diffuse + specular;
	color.a = 1.f;
	
	return color;
}