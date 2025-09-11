//#include header
//#include gamma
uniform sampler2DArray uTexture;
uniform vec4 uColor;
uniform int uTileIndex;

in VS_OUT {
    vec2 uv;
} fs_in;

out vec4 FragColor;

void main() {
    vec4 textureColor = vec4(0.0, 0.0, 0.0, 1.0);
    if (uTileIndex == -1){
        FragColor = vec4(gammaCorrection(uColor.rgb), uColor.a);
    } else {
        vec4  textureColor = texture(uTexture, vec3(fs_in.uv, uTileIndex));
        if (textureColor.a <= 0.0) {
            discard;
        }
        FragColor.rgb = gammaCorrection(textureColor.rgb * uColor.rgb);
        FragColor.a = textureColor.a * uColor.a;
    }
}
