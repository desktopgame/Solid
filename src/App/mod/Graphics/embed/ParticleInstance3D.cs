struct InstanceData {
    float3 offset : OFFSET;
    float3 velocity : VELOCITY;
};

RWStructuredBuffer<InstanceData> instanceBuffer : register(u0);

cbuffer Constants : register(b0) {
    float deltaTime;
}

[numthreads(256, 1, 1)]
void csMain(uint3 dispatchThreadId : SV_DispatchThreadID) {
    uint id = dispatchThreadId.x;
    InstanceData data = instanceBuffer[id];
    data.offset += data.velocity * deltaTime;
    instanceBuffer[id] = data;
}
