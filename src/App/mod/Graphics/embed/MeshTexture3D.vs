struct Output {
    float4 svPos : SV_POSITION;
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
    float4 color : COLOR;
};
cbuffer cbuff0 : register(b0) {
    matrix modelMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
}
cbuffer cbuff1 : register(b1) { float4 color; }

Output vsMain(float3 pos : POSITION, float3 normal : NORMAL, float2 texCoord : TEXCOORD) {
    Output output;
    output.svPos = mul(modelMatrix, float4(pos, 1));
    output.svPos = mul(viewMatrix, output.svPos);
    output.svPos = mul(projectionMatrix, output.svPos);
    output.worldPos = mul(modelMatrix, float4(pos, 1));
    output.normal = normal;
    output.texCoord = texCoord;
    output.color = color;
    return output;
}