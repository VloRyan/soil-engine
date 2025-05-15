#version 330
precision highp float;

layout (location = 0) in vec2 aVertexPosition;

uniform mat4 ProjectionViewMatrix;
uniform mat4 Transform;
uniform vec2 Size;
uniform vec4 Color;

out VS_OUT {
    vec2 uv;
    vec4 color;
} vs_out;

uniform int Ticks;

void main() {
    vec4 worldPosition = Transform * vec4(vec3(aVertexPosition * Size, 0.0f), 1.0f);
    gl_Position = ProjectionViewMatrix * worldPosition;

    vs_out.uv = aVertexPosition + vec2(0.5);
    vs_out.color = Color;
}

