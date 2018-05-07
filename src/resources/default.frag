#version 450

in vec4 carryColr;
out vec4 finalColor;

void main() {
    vec3 N;
    N.xy = gl_PointCoord * 2.0 - vec2(1.0);
    float mag = dot(N.xy, N.xy);
    if (mag > 1.0) discard;
    N.z = sqrt(1.0 - mag);

    vec4 tmp = carryColr;
    tmp.w *= sqrt(1 - mag);
    finalColor = tmp;


    // calculate lighting
    //float diffuse = max(0.0, dot(lightDir, N));
    //float diffuse = max(N, 0.0);
    //finalColor = carryColr * diffuse;

    //finalColor = carryColr;
}
