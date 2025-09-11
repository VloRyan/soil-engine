//#include header
//#include gamma
uniform sampler2DArray Texture;


in VS_OUT {
    vec2 uv;
    vec4 color;
    flat int tileIndex;
} fs_in;
out vec4 FragColor;
float near = 0.1;
float far  = 100.0;
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;// back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
    vec4 textureColor = vec4(0.0, 0.0, 0.0, 1.0);
    if (fs_in.tileIndex == -1){
        FragColor = vec4(gammaCorrection(fs_in.color.rgb), fs_in.color.a);
    } else {
        vec4  textureColor = texture(Texture, vec3(fs_in.uv, fs_in.tileIndex));
        if (textureColor.a <= 0.0) {
            discard;
        };
        FragColor.rgb = gammaCorrection(min(textureColor.rgb * fs_in.color.rgb, 1.0));
        FragColor.a = textureColor.a * fs_in.color.a;
    }
    //float depth = LinearizeDepth(gl_FragCoord.z) / far;// divide by far for demonstration
    //FragColor = vec4(vec3(depth), 1.0);
}
