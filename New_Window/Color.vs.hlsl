/*//////////////////////////////////////////////////////////////
// cbuffer:
//////////////////////////////////////////////////////////////*/
cbuffer MyConstants : register(b0)
{
	float4x4 WorldViewProj;
}

/*//////////////////////////////////////////////////////////////
// My Types:
//////////////////////////////////////////////////////////////*/
struct VS_INPUT
{
	float3 Pos : POSITION;
	float4 Color : COLOR0;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.Pos = mul(WorldViewProj, float4(input.Pos, 1.0f));
    output.Color = input.Color;
    return output;
}