float4 main(float2 inPos : POSITION) : SV_Position
{
    return float4(inPos, 0, 1);
}