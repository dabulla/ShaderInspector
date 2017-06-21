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
        // CODE HIER
        // (Tesselation Levels sollen ueber uniform-varibale steuerbar sein)
        // (bitte in GUI nur Werte im Bereich 0.0 bis 10.0 verwenden)
        gl_TessLevelInner[0] = 6.0;
        gl_TessLevelOuter[0] = 6.0;
        gl_TessLevelOuter[1] = 6.0;
        gl_TessLevelOuter[2] = 6.0;
    }
}
