struct Output {
    float4 svpos : SV_POSITION;
    float3 wpos : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float lifetime : LIFETIME;
};
cbuffer cbuff0 : register(b0) {
    matrix modelMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
}
cbuffer cbuff1 : register(b1) { float4 color; }

Output vsMain(float3 pos : POSITION, float3 normal : NORMAL, float3 offset : OFFSET, float3 velocity : VELOCITY, float lifetime : LIFETIME) {
    Output output;
    output.svpos = mul(modelMatrix, float4(pos, 1)) + float4(offset, 0);
    output.svpos = mul(viewMatrix, output.svpos);
    output.svpos = mul(projectionMatrix, output.svpos);
    output.wpos = mul(modelMatrix, float4(pos, 1)) + float4(offset, 0);
    output.normal = normal;
    output.color = color;
    output.lifetime = lifetime;
    return output;
}