#version 400

layout(vertices = 3) out;

in worldVertex
{
    vec3 position;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
    vec4 color;
} input[];


out worldVertex
{
    vec3 position;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
    vec4 color;
} output[];

//in worldVertex input[];
//out worldVertex output[];

uniform float TessLevelInner;
uniform float TessLevelOuter;

#define ID gl_InvocationID


void main()
{
    output[ID].position = input[ID].position;
    output[ID].texCoord = input[ID].texCoord;
    output[ID].normal = input[ID].normal;
    output[ID].tangent = input[ID].tangent;
    output[ID].bitangent = input[ID].bitangent;
    output[ID].color = input[ID].color;
    if (ID == 0) {
        gl_TessLevelInner[0] = TessLevelInner;
        gl_TessLevelOuter[0] = TessLevelOuter;
        gl_TessLevelOuter[1] = TessLevelOuter;
        gl_TessLevelOuter[2] = TessLevelOuter;
    }
}
