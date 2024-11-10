struct Output {
    float4 svpos : SV_POSITION;
    float4 color : COLOR;
};
cbuffer cbuff0 : register(b0) { matrix modelMatrix; }
cbuffer cbuff1 : register(b1) { matrix viewMatrix; }
cbuffer cbuff2 : register(b2) { matrix projectionMatrix; }
cbuffer cbuff3 : register(b3) { float4 color; }

Output vsMain(float2 pos : POSITION) {
    Output output;
    output.svpos = mul(modelMatrix, float4(pos, 0, 1));
    output.svpos = mul(viewMatrix, output.svpos);
    output.svpos = mul(projectionMatrix, output.svpos);
    output.color = color;
    return output;
}