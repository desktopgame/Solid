struct GSInput {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

struct GSOutput {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

[maxvertexcount(18)]
void gsMain(triangle GSInput input[3], inout TriangleStream<GSOutput> triStream) {
    float lineWidth = 50;
    float ViewportHeight = 600;
    float ndcLineWidth = lineWidth / ViewportHeight;

    for (int i = 0; i < 3; ++i) {
        int nextIndex = (i + 1) % 3; // 次の頂点

        float4 p0 = input[i].position;
        float4 p1 = input[nextIndex].position;

        float2 dir = normalize(float2(p1.x - p0.x, p1.y - p0.y));

        float2 normal = float2(-dir.y, dir.x) * ndcLineWidth;

        GSOutput v0, v1, v2, v3;

        v0.position = p0 + float4(normal, 0.0, 0.0);
        v0.color = input[i].color;

        v1.position = p1 + float4(normal, 0.0, 0.0);
        v1.color = input[nextIndex].color;

        v2.position = p0 - float4(normal, 0.0, 0.0);
        v2.color = input[i].color;

        v3.position = p1 - float4(normal, 0.0, 0.0);
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
