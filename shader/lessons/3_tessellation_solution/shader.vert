#version 400

// input
in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexCoord;
in vec3 vertexTangent;
in vec3 vertexBitangent;

// output
out worldVertex
{
    vec3 position;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
    vec4 color;
} output;

// uniforms
uniform mat4x4 mvp;
uniform mat3x3 modelViewNormal;

// uniforms from Gui
uniform vec3 lightDir;

void main(void)
{
    gl_Position = mvp * vec4(vertexPosition, 1.0);
    output.position = vertexPosition;
    output.texCoord = vertexTexCoord;
    output.normal = vertexNormal;
    output.tangent = vertexTangent;
    output.bitangent = vertexBitangent;

    float lit = dot(normalize(vertexNormal), normalize(lightDir)) * .5 + .5;
    output.color = vec4(lit, lit, lit, 1.0);
}
