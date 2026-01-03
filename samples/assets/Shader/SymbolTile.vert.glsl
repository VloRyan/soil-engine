//#include header
//#include uniform.Matrices_033

layout (location = 0) in vec2 aVertexPosition;

uniform vec3 uPositionWorldSpace;
uniform vec2 uSize;

out VS_OUT {
    vec2 uv;
} vs_out;

void main() {
    vec4 worldPos = vec4(uPositionWorldSpace + vec3(aVertexPosition * uSize, 0.0f), 1.0f);
    gl_Position = ProjectionViewMatrix * worldPos;

    vs_out.uv = aVertexPosition + vec2(0.5);
}
