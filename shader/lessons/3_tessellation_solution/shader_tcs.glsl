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
} inp[];


out worldVertex
{
    vec3 position;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
    vec4 color;
} outp[];

//in worldVertex inp[];
//out worldVertex outp[];

uniform float TessLevelInner;
uniform float TessLevelOuter;

#define ID gl_InvocationID


void main()
{
    outp[ID].position = inp[ID].position;
    outp[ID].texCoord = inp[ID].texCoord;
    outp[ID].normal = inp[ID].normal;
    outp[ID].tangent = inp[ID].tangent;
    outp[ID].bitangent = inp[ID].bitangent;
    outp[ID].color = inp[ID].color;
    if (ID == 0) {
        gl_TessLevelInner[0] = TessLevelInner;
        gl_TessLevelOuter[0] = TessLevelOuter;
        gl_TessLevelOuter[1] = TessLevelOuter;
        gl_TessLevelOuter[2] = TessLevelOuter;
    }
}
