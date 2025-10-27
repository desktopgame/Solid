struct Output {
    float4 svPos : SV_POSITION;
    float4 worldPos : POSITION;
    float2 texCoord : TEXCOORD;
    float4 axis : NORMAL;
    float4 color : COLOR;
    float3 cameraPosition : POSITION1;
    float4 borderColor : COLOR2;
    float4 fogColor : COLOR3;
};
cbuffer cbuff0 : register(b0) {
    matrix modelMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};
cbuffer cbuff1 : register(b1)
{
    matrix translateMatrixTable[6];
    matrix rotationMatrixTable[6];
    float3 normalVectorTable[6];
};
cbuffer cbuff2 : register(b2)
{
    float4 colorTable[64];
};
cbuffer cbuff3 : register(b3)
{
    float3 cameraPosition;
};
cbuffer cbuff4 : register(b4)
{
    float4 borderColor;
};
cbuffer cbuff5 : register(b5)
{
    float4 fogColor;
};

static const float4 axisTable[6] = {
    // posY
    float4(-1, 0, 0, 90),
    // negY
    float4(1, 0, 0, 90),
    // posX
    float4(0, 1, 0, 90),
    // negX
    float4(0, -1, 0, 90),
    // posZ
    float4(0, 0, 1, 0),
    // negZ
    float4(0, 1, 0, 180)
};

Output vsMain(float3 pos : POSITION, float2 texCoord : TEXCOORD, float4 tileData : INSTANCE0) {
    Output output;
    int tileInfo = int(tileData.w);
    int tileRotationID = tileInfo % 10;
    int tileColorID = tileInfo / 10;
    matrix tileRotation = rotationMatrixTable[tileRotationID];
    matrix tileTranslate = translateMatrixTable[tileRotationID];
    matrix tileTransform = mul(tileTranslate, tileRotation);
    float3 tileOffset = tileData.xyz;

    float4 tmp = float4(mul(tileTransform, float4(pos, 1)) + tileOffset, 1);

    // メッシュのつなぎ目（gap, seamsなどと呼ばれる）が描画されないための対策
    // 隙間なくタイルを敷き詰めているつもりでも、
    // 浮動小数点数演算の誤差によってタイルの間に隙間が生じることがある。
    // これを抑えるために、小数点第二位以下を切り捨てる。
    tmp = round(tmp * 100.0f) / 100.0f;

    tmp = mul(modelMatrix, tmp);
    output.worldPos = tmp;

    tmp = mul(viewMatrix, tmp);
    tmp = mul(projectionMatrix, tmp);

    output.svPos = tmp;
    output.color = colorTable[tileColorID];
    output.texCoord = texCoord;
    output.cameraPosition = cameraPosition;

    float3 normal = normalVectorTable[tileRotationID];
    output.axis = axisTable[tileRotationID];

    output.borderColor = borderColor;
    output.fogColor = fogColor;
    return output;
}