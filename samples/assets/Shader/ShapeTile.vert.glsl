//#include header

layout (location = 0) in vec2 aVertexPosition;

uniform mat4 ProjectionViewMatrix;
uniform mat4 uTransform;
uniform vec2 uSize;
uniform vec2 uTileScale;

out VS_OUT {
    vec2 uv;
} vs_out;

uniform int Ticks;

void main() {
    vec4 worldPosition = uTransform * vec4(vec3(aVertexPosition * uSize, 0.0f), 1.0f);
    gl_Position = ProjectionViewMatrix * worldPosition;

    vs_out.uv = (aVertexPosition + vec2(0.5))*uTileScale;
}
