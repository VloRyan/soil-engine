//#include header
//#include uniform.Matrices_033

layout (location = 0) in vec2 aVertexPosition;

uniform vec3 uPositionWorldSpace;
uniform vec2 uSize;
uniform vec2 uTexCoords;
uniform vec2 uTexCoordsMax;

out vec2 UV;

void main() {
    // Output position of the vertex, in clip space
    vec4 worldPos = vec4(uPositionWorldSpace + vec3(aVertexPosition * uSize, 0.0), 1.0);
    gl_Position = ProjectionViewMatrix * worldPos;
    vec2 uv;
    uv.x = uTexCoords.x * (aVertexPosition.x - 0.5) * -1.0;
    uv.y = uTexCoordsMax.y * (aVertexPosition.y - 0.5) * -1.0;
    uv.x += uTexCoordsMax.x * (aVertexPosition.x + 0.5);
    uv.y += uTexCoords.y  * (aVertexPosition.y + 0.5);
    UV = uv;
}
