#version 330

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec2 aVertexPosition;
layout (location = 1) in vec3 aPositionWorldspace;// Position of the center of the shape
layout (location = 2) in vec2 aSize;
layout (location = 3) in vec4 aColor;
layout (location = 4) in vec3 aBorderColor;
layout (location = 5) in vec2 aTexCoords;
layout (location = 6) in vec2 aTexCoordsMax;
layout (location = 7) in vec2 aCharOutline;
layout (location = 8) in vec2 aBorderOutline;

uniform mat4 ProjectionViewMatrix;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec4 Color;
out vec3 BorderColor;
out vec2 CharOutline;
out vec2 BorderOutline;

void main() {
    // Output position of the vertex, in clip space
    vec4 worldPos = vec4(aPositionWorldspace + vec3(aVertexPosition * aSize, 0.0f), 1.0f);
    gl_Position = ProjectionViewMatrix * worldPos;
    UV.x = aTexCoords.x * (aVertexPosition.x - 0.5) * -1;
    UV.y = aTexCoordsMax.y * (aVertexPosition.y - 0.5) * -1;;
    UV.x += aTexCoordsMax.x * (aVertexPosition.x + 0.5);
    UV.y += aTexCoords.y  * (aVertexPosition.y + 0.5);
    Color = aColor;
    BorderColor = aBorderColor;
    CharOutline = aCharOutline;
    BorderOutline = aBorderOutline;
}
