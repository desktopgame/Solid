struct Output {
    float4 svPos : SV_POSITION;
    float4 worldPos : POSITION;
    float4 color : COLOR;
};
cbuffer cbuff0 : register(b0) {
    matrix modelMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
}
cbuffer cbuff1 : register(b1) { float4 color; }

Output vsMain(float3 pos : POSITION) {
    Output output;
    output.svPos = mul(modelMatrix, float4(pos, 1));
    output.worldPos = output.svPos;
    output.svPos = mul(viewMatrix, output.svPos);
    output.svPos = mul(projectionMatrix, output.svPos);
    output.color = color;
    return output;
}