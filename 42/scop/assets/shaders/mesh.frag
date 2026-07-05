#version 450

layout(location = 0) in vec3      vLocalPos;
layout(location = 1) in vec3      vLocalNormal;
layout(location = 2) in vec3      vWorldNormal;
layout(location = 3) in flat uint vFaceId;
layout(location = 4) in vec2      vUV;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform sampler2D uTex;

layout(push_constant) uniform Push {
    float blend;
    uint  useUV;   // 1 => sample with per-vertex UVs, 0 => triplanar fallback
} pushc;

// Per-face deterministic gray-ish color (so faces are visually distinguishable
// without requiring a texture). Hash + desaturate to subtle shades.
vec3 faceColor(uint id) {
    uint x = id * 1664525u + 1013904223u;
    float r = float((x >>  0) & 255u) / 255.0;
    float g = float((x >>  8) & 255u) / 255.0;
    float b = float((x >> 16) & 255u) / 255.0;
    vec3 c = vec3(r, g, b);
    float l = dot(c, vec3(0.299, 0.587, 0.114));
    c = mix(vec3(l), c, 0.25);
    return mix(vec3(0.2), c, 0.7);
}

// Triplanar sampling: pick the projection plane (YZ, XZ, XY) per-pixel based
// on which axis of the local normal dominates. Weights are sharpened with a
// power so each side picks a single dominant projection and the blend zones
// stay narrow.
vec3 triplanar(vec3 pos, vec3 normal, float scale) {
    vec3 w = pow(abs(normalize(normal)), vec3(8.0));
    w /= max(w.x + w.y + w.z, 1e-5);

    vec3 p = pos * scale;
    vec3 cX = texture(uTex, p.yz).rgb;
    vec3 cY = texture(uTex, p.zx).rgb;
    vec3 cZ = texture(uTex, p.xy).rgb;

    return cX * w.x + cY * w.y + cZ * w.z;
}

void main() {
    vec3 n     = normalize(vWorldNormal);
    float ndl  = clamp(dot(n, normalize(vec3(0.4, 0.7, 0.6))), 0.0, 1.0);
    // Shared diffuse term so both the face-color and textured views have the
    // same sense of form (a fullbright texture reads as a flat, melted blob).
    float shade = 0.4 + 0.6 * ndl;

    vec3 cFace = faceColor(vFaceId) * shade;

    // Prefer the model's authored UVs when it has them; otherwise fall back to
    // triplanar projection (e.g. the teapot, which ships without vt coords).
    // These assets author UVs with V=0 at the top, matching the PNG's row
    // order, so we sample vUV directly -- flipping V would make each face of
    // the dense per-face atlas hit an unrelated patch (a scrambled "camo" look).
    vec3 albedo;
    if (pushc.useUV != 0u)
        albedo = texture(uTex, vUV).rgb;
    else
        albedo = triplanar(vLocalPos, vLocalNormal, 0.5);
    vec3 cTex  = albedo * shade;

    vec3 rgb   = mix(cFace, cTex, clamp(pushc.blend, 0.0, 1.0));
    outColor   = vec4(rgb, 1.0);
}
