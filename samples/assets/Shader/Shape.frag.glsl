#version 330
precision highp float;

uniform sampler2D Texture;


in VS_OUT {
    vec2 uv;
    vec4 color;
} fs_in;
out vec4 FragColor;

void main() {
    vec4 textureColor = texture(Texture, fs_in.uv);
    if (textureColor.a <= 0.0) {
        discard;
    };
    FragColor.rgb = min(textureColor.rgb * fs_in.color.rgb, 1.0);
    FragColor.a = textureColor.a * fs_in.color.a;
}
