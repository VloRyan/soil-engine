#version 330

uniform sampler2D Textures[3];

in VS_OUT {
    vec2 uv;
    vec4 color;
    flat uint textureIndex;
} fs_in;
out vec4 FragColor;

void main() {
    vec4 textureColor;
    switch (int(fs_in.textureIndex)) {
        case 0:
        textureColor = texture(Textures[0], fs_in.uv);
        break;
        case 1:
        textureColor = texture(Textures[1], fs_in.uv);
        break;
        case 2:
        textureColor = texture(Textures[2], fs_in.uv);
        break;
        default :
        break;
    }
    if (textureColor.a <= 0.0) {
        discard;
    };
    FragColor.rgb = min(textureColor.rgb * fs_in.color.rgb, 1.0);
    FragColor.a = textureColor.a * fs_in.color.a;
}
