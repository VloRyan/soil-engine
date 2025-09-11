//#include header

uniform mat4 ProjectionViewMatrix;

layout (location = 0) in int aVertexData;
layout (location = 1) in vec3 aStart;
layout (location = 2) in vec3 aEnd;
layout (location = 3) in vec4 aColor;

out vec4 Color;

void main(void) {
    vec3 point = mix(aStart, aEnd, aVertexData);
    gl_Position = ProjectionViewMatrix * vec4(point, 1.0f);
    Color = aColor;
}
