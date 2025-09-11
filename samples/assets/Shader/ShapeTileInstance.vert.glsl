//#include header

layout (location = 0) in vec2 aVertexPosition;
layout (location = 1) in mat4 aMatrix;// 4x4 = location 1...4
layout (location = 5) in vec2 aSize;
layout (location = 6) in vec4 aColor;
layout (location = 7) in int aTileIndex;
layout (location = 8) in vec2 aTileScale;

uniform mat4 ProjectionViewMatrix;

out VS_OUT {
    vec2 uv;
    vec4 color;
    flat int tileIndex;
} vs_out;

uniform int Ticks;

void main() {
    vec4 worldPosition = aMatrix * vec4(vec3(aVertexPosition * aSize, 0.0f), 1.0f);
    gl_Position = ProjectionViewMatrix * worldPosition;

    vs_out.uv = (aVertexPosition + vec2(0.5))*aTileScale;
    vs_out.color = aColor;
    vs_out.tileIndex = aTileIndex;
}
