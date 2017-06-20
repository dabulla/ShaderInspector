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
        // CODE HIER
        // (Tesselation Levels sollen ueber uniform-varibale steuerbar sein)
        // (bitte in GUI nur Werte im Bereich 0.0 bis 10.0 verwenden)
        gl_TessLevelInner[0] = 6.0;
        gl_TessLevelOuter[0] = 6.0;
        gl_TessLevelOuter[1] = 6.0;
        gl_TessLevelOuter[2] = 6.0;
    }
}
