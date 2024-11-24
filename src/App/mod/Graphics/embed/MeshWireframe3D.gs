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
    {
        GSOutput output1;
        output1.position = input[0].position;
        output1.color = input[0].color;
        stream.Append(output1);

        GSOutput output2;
        output2.position = input[1].position;
        output2.color = input[1].color;
        stream.Append(output2);
    }
    {
        GSOutput output1;
        output1.position = input[1].position;
        output1.color = input[1].color;
        stream.Append(output1);

        GSOutput output2;
        output2.position = input[2].position;
        output2.color = input[2].color;
        stream.Append(output2);
    }
    {
        GSOutput output1;
        output1.position = input[0].position;
        output1.color = input[0].color;
        stream.Append(output1);

        GSOutput output2;
        output2.position = input[2].position;
        output2.color = input[2].color;
        stream.Append(output2);
    }
}
