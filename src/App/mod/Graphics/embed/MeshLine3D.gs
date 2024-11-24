struct GSInput {
    float4 svpos : SV_POSITION;
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct GSOutput {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

cbuffer cbuff0 : register(b0) {
    matrix modelMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
}

cbuffer cbuff1 : register(b1) {
    float lineWidth;
}

cbuffer cbuff2 : register(b2) {
    float4 cameraPosition;
}

[maxvertexcount(18)]
void gsMain(triangle GSInput input[3], inout TriangleStream<GSOutput> triStream) {
    matrix vpMat = mul(viewMatrix, projectionMatrix);
    for (int i = 0; i < 3; ++i) {
        int nextIndex = (i + 1) % 3;

        float3 worldP0 = input[i].pos;
        float3 worldP1 = input[nextIndex].pos;

        float3 camToEdge = normalize(worldP0 - cameraPosition.xyz);
        float3 edgeDir = normalize(worldP1 - worldP0);
        float3 edgeNormal = normalize(cross(edgeDir, camToEdge));

        float3 offset = edgeNormal * (lineWidth * 0.5);

        GSOutput v0, v1, v2, v3;

        v0.position = mul(vpMat, float4(worldP0 + offset, 1.0));
        v0.color = input[i].color;

        v1.position = mul(vpMat, float4(worldP1 + offset, 1.0));
        v1.color = input[nextIndex].color;

        v2.position = mul(vpMat, float4(worldP0 - offset, 1.0));
        v2.color = input[i].color;

        v3.position = mul(vpMat, float4(worldP1 - offset, 1.0));
        v3.color = input[nextIndex].color;

        triStream.Append(v0);
        triStream.Append(v1);
        triStream.Append(v2);

        triStream.RestartStrip();

        triStream.Append(v2);
        triStream.Append(v1);
        triStream.Append(v3);

        triStream.RestartStrip();
    }
}
