#version 400

//input
in screenSpaceData
{
    vec3 normal;
    vec4 color;
} input;

// output
out vec4 out_Color;

const vec4 gamma = vec4(vec3(2.2), 1.0);
const vec4 invgamma = 1.0/gamma;

void main(void)
{
    out_Color = input.color;
}
