vec3 gammaCorrection(vec3 color) {
    const vec3 gamma = vec3(1.0 / 2.2);
    return pow(color, gamma);
}
