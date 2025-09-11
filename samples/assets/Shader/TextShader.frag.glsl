#version 330

// Interpolated values from the vertex shaders
in vec2 UV;
in vec4 Color;
in vec3 BorderColor;
in vec2 CharOutline;
in vec2 BorderOutline;

// Ouput data
out vec4 FragColor;

// Values that stay constant for the whole mesh.
uniform sampler2D Texture;

void main() {
    float distance = 1.0f - texture(Texture, UV).a;

    float alpha = smoothstep(CharOutline.x + CharOutline.y, CharOutline.x, distance);
    float outlineAlpha = smoothstep(BorderOutline.x + BorderOutline.y, BorderOutline.x, distance);

    float overallAlpha = alpha + (1.0f - alpha) * outlineAlpha;
    vec3 overallColor = mix(BorderColor, Color.rgb, alpha / overallAlpha);
    overallAlpha = min(overallAlpha, Color.a);
    FragColor = vec4(overallColor, overallAlpha);
}
