#version 330
precision highp float;

layout (location = 0) in vec2 aVertexPosition;
layout (location = 1) in mat4 aMatrix;// 4x4 = location 1...4
layout (location = 5) in vec2 aSize;
layout (location = 6) in vec4 aColor;
layout (location = 7) in uint aTextureIndex;

uniform mat4 ProjectionViewMatrix;

out VS_OUT {
    vec2 uv;
    vec4 color;
    flat uint textureIndex;
    vec3 position;
    vec3 normal;
} vs_out;

uniform int Ticks;

void main() {
    vec4 worldPosition = aMatrix * vec4(vec3(aVertexPosition * aSize, 0.0f), 1.0f);
    gl_Position = ProjectionViewMatrix * worldPosition;

    vs_out.textureIndex = aTextureIndex;
    vs_out.uv = aVertexPosition + vec2(0.5);
    vs_out.color = aColor;
    vs_out.position = worldPosition.xyz;
    vs_out.normal = aMatrix[1].xyz;
}
