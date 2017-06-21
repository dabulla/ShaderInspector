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
    vec3 tangent;
    vec3 bitangent;
} outp;

// uniforms
uniform mat4x4 mvp;

void main(void)
{
    gl_Position = mvp * vec4(vertexPosition, 1.0);
    outp.position = vertexPosition;
    outp.normal = vertexNormal;
    outp.tangent = vertexTangent;
    outp.bitangent = vertexBitangent;
}
