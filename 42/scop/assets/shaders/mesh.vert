#version 450

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;
layout(location = 3) in uint inFaceId;

// We pass the local (pre-model-matrix) position and normal to the fragment
// shader so it can do triplanar texture sampling. World-space normal is
// derived in the fragment shader from the model matrix on the fly.
layout(location = 0) out vec3      vLocalPos;
layout(location = 1) out vec3      vLocalNormal;
layout(location = 2) out vec3      vWorldNormal;
layout(location = 3) out flat uint vFaceId;

layout(set = 0, binding = 0) uniform UBO {
    mat4 proj;
    mat4 view;
    mat4 model;
} ubo;

mat4 fixVulkanClip(mat4 p) {
    p[1][1] = -p[1][1];
    return p;
}

void main() {
    vLocalPos    = inPos;
    vLocalNormal = inNormal;
    vWorldNormal = mat3(ubo.model) * inNormal;
    vFaceId      = inFaceId;

    mat4 P = fixVulkanClip(ubo.proj);
    gl_Position = P * ubo.view * ubo.model * vec4(inPos, 1.0);
}
