#version 400

// inp
in vec3 vertexPosition;
in vec3 vertexNormal;
in vec3 vertexTangent;
in vec3 vertexBitangent;

// outp
out worldVertex
{
    vec3 position;
    vec3 normal;
    vec4 color;
} outp;

// uniforms
uniform mat4x4 mvp;

void main(void)
{
    gl_Position = mvp * vec4(vertexPosition, 1.0);
    outp.position = vertexPosition;
    outp.normal = vertexNormal;
    outp.color = vec4(1.0);
}
