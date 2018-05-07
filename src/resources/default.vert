#version 450

uniform mat4 camera;
uniform mat4 model;

in vec3 vert;

in vec4 colr;
out vec4 carryColr;

void main() {
    // Pass colr through to fragment shader
    carryColr = colr;

    //gl_PointSize = 2.5;
    gl_PointSize = 8;

    // Apply all matrix transformations to vert
    gl_Position = camera * model * vec4(vert, 1);
}
