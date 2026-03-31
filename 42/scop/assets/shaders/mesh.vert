#version 450

layout(location = 0) in vec3  inPos;
layout(location = 1) in vec3  inNormal;
layout(location = 2) in vec2  inUV;
layout(location = 3) in uint inFaceId;

layout(location = 0) out vec2  vUV;
layout(location = 1) out vec3  vNormal;
layout(location = 2) out flat uint vFaceId;

// CPU-side UboMVP { Mat4 proj; Mat4 view; Mat4 model; }
layout(set = 0, binding = 0) uniform UBO {
    mat4 proj;
    mat4 view;
    mat4 model;
} ubo;

// Optional: flip Y for Vulkan if your CPU math is GL-style
// (You can also bake this into Mat4::perspective instead.)
mat4 fixVulkanClip(mat4 p) {
    p[1][1] = -p[1][1];
    // If you used GL-style depth (-1..1), you may also need to adjust Z.
    return p;
}

void main() {
    vUV     = inUV;
    vNormal = mat3(ubo.model) * inNormal;
    vFaceId = inFaceId;

    mat4 P = fixVulkanClip(ubo.proj);
    gl_Position = P * ubo.view * ubo.model * vec4(inPos, 1.0);
}
