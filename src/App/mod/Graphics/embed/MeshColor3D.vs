struct Output {
    float4 svpos : SV_POSITION;
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
    output.svpos = mul(modelMatrix, float4(pos, 1));
    output.svpos = mul(viewMatrix, output.svpos);
    output.svpos = mul(projectionMatrix, output.svpos);
    output.color = color;
    return output;
}