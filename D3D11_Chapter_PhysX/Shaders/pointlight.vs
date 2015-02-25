cbuffer cbPerObject
{
	float4x4 WVP;
	float4x4 World;
};

struct VertexInputType
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD;
	float3 Norm : NORMAL;
};

struct PixelInputType
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 Norm : NORMAL;
	float4 worldPos : POSITION;
};

PixelInputType VS(VertexInputType input)
{
	PixelInputType output;
	output.pos = mul(input.pos, WVP);
	output.worldPos = mul(input.pos, World);
	output.Norm = mul(input.Norm, World);
	output.tex = input.tex;
	return output;
}