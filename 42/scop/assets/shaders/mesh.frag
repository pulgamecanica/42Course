#version 450

layout(location = 0) in vec2  vUV;
layout(location = 1) in vec3  vNormal;
layout(location = 2) in flat uint vFaceId;

layout(location = 0) out vec4 outColor;

// set=0,binding=1: combined image sampler (bind only if using texture)
layout(set = 0, binding = 1) uniform sampler2D uTex;

layout(push_constant) uniform Push {
    float blend; // 0..1, smooth transition
} pushc;

// simple deterministic gray-ish face color from id
vec3 faceColor(uint id) {
    // xorshift-ish hash -> [0.2,0.9] range for each channel
    uint x = id * 1664525u + 1013904223u;
    float r = float((x >>  0) & 255u) / 255.0;
    float g = float((x >>  8) & 255u) / 255.0;
    float b = float((x >> 16) & 255u) / 255.0;
    vec3 c = vec3(r,g,b);
    // desaturate to “subtle gray shades”
    float l = dot(c, vec3(0.299,0.587,0.114));
    c = mix(vec3(l), c, 0.25);
    return mix(vec3(0.2), c, 0.7);
}

void main() {
    vec3 n = normalize(vNormal);
    float ndotl = clamp(dot(n, normalize(vec3(0.4,0.7,0.6))), 0.0, 1.0);

    vec3 cFace = faceColor(vFaceId) * (0.4 + 0.6 * ndotl);
    vec3 cTex  = texture(uTex, vUV).rgb;

    // smooth blend controlled by pushc.blend
    vec3 rgb = mix(cFace, cTex, clamp(pushc.blend, 0.0, 1.0));
    outColor = vec4(rgb, 1.0);
}
