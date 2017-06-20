#version 400

layout(triangles, equal_spacing, ccw) in;

uniform sampler2D heightTex;
uniform float heightScale;

//// input:
// 1) whole primitive (input[])
// 2) interpolation information (gl_TessCoord)
//    - gl_TessCoord.x => how much influence has input[0]
//    - gl_TessCoord.y => how much influence has input[1]
//    - ...
in worldVertex
{
    vec3 position;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
    vec4 color;
} input[];

//// output:
// one vertex
out worldVertex
{
    vec3 position;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
    vec4 color;
} output;

void main()
{
    output.texCoord = gl_TessCoord.x * input[0].texCoord
                    + gl_TessCoord.y * input[1].texCoord
                    + gl_TessCoord.z * input[2].texCoord;
    output.normal = gl_TessCoord.x * input[0].normal
                    + gl_TessCoord.y * input[1].normal
                    + gl_TessCoord.z * input[2].normal;

    //CODE HIER
    // Uniforms:
    // heightTex
    // heightScale (sollte in GUI groesser 1.0 gesetzt werden, um Effekt zu sehen)

    /* output.position = ?;*/
    float examplePeaks = min(gl_TessCoord.x, min(gl_TessCoord.y, gl_TessCoord.z));
    examplePeaks = pow(examplePeaks, 3.0);
    vec3 exampleBumpedPositionOffset = examplePeaks * output.normal * (1.0-heightScale);

    output.position = gl_TessCoord.x * input[0].position
                    + gl_TessCoord.y * input[1].position
                    + gl_TessCoord.z * input[2].position;
    output.position += exampleBumpedPositionOffset;

    output.tangent = gl_TessCoord.x * input[0].tangent
                    + gl_TessCoord.y * input[1].tangent
                    + gl_TessCoord.z * input[2].tangent;
    output.bitangent = gl_TessCoord.x * input[0].bitangent
                    + gl_TessCoord.y * input[1].bitangent
                    + gl_TessCoord.z * input[2].bitangent;
    output.color = gl_TessCoord.x * input[0].color
                    + gl_TessCoord.y * input[1].color
                    + gl_TessCoord.z * input[2].color;
}
