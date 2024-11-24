struct GSInput {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

struct GSOutput {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

[maxvertexcount(6)]
void gsMain(triangle GSInput input[3], inout LineStream<GSOutput> stream) {
    for (int i = 0; i < 3; ++i) {
        GSOutput outputStart, outputEnd;

        int nextIndex = (i + 1) % 3;
        outputStart.position = input[i].position;
        outputEnd.position = input[nextIndex].position;

        outputStart.color = input[i].color;
        outputEnd.color = input[nextIndex].color;

        stream.Append(outputStart);
        stream.Append(outputEnd);

        stream.RestartStrip();
    }
}
