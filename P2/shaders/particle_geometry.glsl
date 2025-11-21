#version 410 core

layout(points) in;
layout(triangle_strip, max_vertices = 1024) out;

uniform mat4 Proj; 
uniform float time;
uniform float radius;
uniform float particleSize;

in vec4 vViewPos[];
out vec4 vColor;

float rand(in uint x) {
    x = (x ^ 61u) ^ (x >> 16);
    x *= 9u;
    x = x ^ (x >> 4);
    x *= 0x27d4eb2du;
    x = x ^ (x >> 15);
    return float(x) / 4294967295.0;
}

float rnd(int prim, int idx) {
    uint key = uint(prim);
    uint base = uint(floor(time * 1.0)); 
    key = key * 1664525u + uint(idx) * 1013904223u + base;
    return rand(key);
}

void emitQuad(vec3 center, float size, vec3 color) {
    vec3 right = vec3(1.0, 0.0, 0.0);
    vec3 up = vec3(0.0, 1.0, 0.0);

    vec3 bl = center + right * (-size) + up * (-size);
    vec3 br = center + right * ( size) + up * (-size);
    vec3 tl = center + right * (-size) + up * ( size);
    vec3 tr = center + right * ( size) + up * ( size);

    vColor = vec4(color, 1.0);
    gl_Position = Proj * vec4(bl, 1.0);
    EmitVertex();
    vColor = vec4(color, 1.0);
    gl_Position = Proj * vec4(br, 1.0);
    EmitVertex();
    vColor = vec4(color, 1.0);
    gl_Position = Proj * vec4(tl, 1.0);
    EmitVertex();
    vColor = vec4(color, 1.0);
    gl_Position = Proj * vec4(tr, 1.0);
    EmitVertex();
    EndPrimitive();
}

void main() {
    int prim = gl_PrimitiveIDIn;
    vec3 base = vViewPos[0].xyz;

    int PARTICLES = 100;
    for (int i = 0; i < PARTICLES; ++i) {
        float a = rnd(prim, i*2);
        float b = rnd(prim, i*2+1);
        float c = rnd(prim, i*2+2);

        float theta = a * 6.2831853;
        float phi = acos(2.0 * b - 1.0);
        float r = radius * pow(c, 0.33);
        vec3 offset = vec3(r * sin(phi) * cos(theta), r * sin(phi) * sin(theta), r * cos(phi));

        float size = particleSize;

        vec3 color = vec3(0.8 + 0.2 * rnd(prim, i+11), 0.7 + 0.3 * rnd(prim, i+13), 0.4 + 0.6 * rnd(prim, i+17));

        emitQuad(base + offset, size, color);
    }
}
