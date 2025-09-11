//#include header

in vec2 UV;
uniform vec4 uColor;
uniform vec3 uBorderColor;
uniform vec2 uCharacterOutline;
uniform vec2 uBorderOutline;
uniform sampler2D uTexture;

out vec4 FragColor;

void main() {
    float distance = 1.0f - texture(uTexture, UV).a;

    float alpha = smoothstep(uCharacterOutline.x + uCharacterOutline.y, uCharacterOutline.x, distance);
    float outlineAlpha = smoothstep(uBorderOutline.x + uBorderOutline.y, uBorderOutline.x, distance);

    float overallAlpha = alpha + (1.0f - alpha) * outlineAlpha;
    vec3 overallColor = mix(uBorderColor, uColor.rgb, alpha / overallAlpha);
    overallAlpha = min(overallAlpha, uColor.a);
    FragColor = vec4(overallColor, overallAlpha);
}
