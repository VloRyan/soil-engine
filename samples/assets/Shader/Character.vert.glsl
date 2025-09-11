//#include header

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec2 aVertexPosition;

uniform vec3 uPositionWorldspace;// Position of the center of the shape
uniform vec2 uSize;
uniform vec2 uTexCoords;
uniform vec2 uTexCoordsMax;
uniform mat4 ProjectionViewMatrix;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

void main() {
    // Output position of the vertex, in clip space
    vec4 worldPos = vec4(uPositionWorldspace + vec3(aVertexPosition * uSize, 0.0f), 1.0f);
    gl_Position = ProjectionViewMatrix * worldPos;
    UV.x = uTexCoords.x * (aVertexPosition.x - 0.5) * -1;
    UV.y = uTexCoordsMax.y * (aVertexPosition.y - 0.5) * -1;;
    UV.x += uTexCoordsMax.x * (aVertexPosition.x + 0.5);
    UV.y += uTexCoords.y  * (aVertexPosition.y + 0.5);
}
