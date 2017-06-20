#version 400

// input
in vec3 vertexPosition;
in vec3 vertexNormal;
in vec3 vertexTangent;
in vec3 vertexBitangent;

// output
out worldVertex
{
    vec3 position;
    vec3 normal;
    vec4 color;
} output;

// uniforms
uniform mat4x4 mvp;

void main(void)
{
    gl_Position = mvp * vec4(vertexPosition, 1.0);
    output.position = vertexPosition;
    output.normal = vertexNormal;
    output.color = vec4(1.0);
}
