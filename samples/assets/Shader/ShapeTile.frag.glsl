//#include header
//#include gamma
uniform sampler2DArray uTexture;
uniform vec4 uColor;
uniform int uTileIndex;
uniform float uRadius;
uniform vec2 uResolution;
uniform mat4 uTransform;
uniform vec2 uSize;
in VS_OUT {
    vec2 uv;
} fs_in;

out vec4 FragColor;
const float borderWidth = 0.015;
float roundRect(vec2 p, vec2 size, float radius){
    vec2 d = abs(p) - size + vec2(radius, radius);
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0) - radius;
}

void main() {
    vec2 p = (fs_in.uv-0.5);
    float aspect = uSize.x/uSize.y;
    vec2 size = vec2(0.5);//* vec2(aspect, 1.F);
    /*if (uSize.x < uSize.y){
        aspect = uSize.y/uSize.x;
        size = vec2(0.5) * vec2(1.F, aspect);
    }*/


    float dist = roundRect(p, size, uRadius);
    vec4 textureColor = vec4(0.0, 0.0, 0.0, 1.0);
    if (uTileIndex == -1){
        FragColor = vec4(gammaCorrection(uColor.rgb), uColor.a);
    } else {
        vec4  textureColor = texture(uTexture, vec3(fs_in.uv, uTileIndex));
        if (textureColor.a <= 0.1) {
            discard;
        }

        FragColor.rgb = gammaCorrection(textureColor.rgb * uColor.rgb);
        FragColor.a = textureColor.a * uColor.a;
        if (uRadius > 0.0){
            if (dist >= 0.0){
                discard;
            }
            if (dist >= -borderWidth){
                FragColor.rgb = vec3(0.1);
                FragColor.a = -dist/borderWidth+ 0.1;
            }
        }
    }
}
