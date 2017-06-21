#version 400

layout(triangles, equal_spacing, ccw) in;

uniform sampler2D heightTex;
uniform float heightScale;

//// inp:
// 1) whole primitive (inp[])
// 2) interpolation information (gl_TessCoord)
//    - gl_TessCoord.x => how much influence has inp[0]
//    - gl_TessCoord.y => how much influence has inp[1]
//    - ...
in worldVertex
{
    vec3 position;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
    vec4 color;
} inp[];

//// outp:
// one vertex
out worldVertex
{
    vec3 position;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
    vec4 color;
} outp;

void main()
{
    outp.texCoord = gl_TessCoord.x * inp[0].texCoord
                    + gl_TessCoord.y * inp[1].texCoord
                    + gl_TessCoord.z * inp[2].texCoord;
    outp.normal = gl_TessCoord.x * inp[0].normal
                    + gl_TessCoord.y * inp[1].normal
                    + gl_TessCoord.z * inp[2].normal;

    //CODE HIER
    // Uniforms:
    // heightTex
    // heightScale (sollte in GUI groesser 1.0 gesetzt werden, um Effekt zu sehen)

    /* outp.position = ?;*/
    float examplePeaks = min(gl_TessCoord.x, min(gl_TessCoord.y, gl_TessCoord.z));
    examplePeaks = pow(examplePeaks, 3.0);
    vec3 exampleBumpedPositionOffset = examplePeaks * outp.normal * (1.0-heightScale);

    outp.position = gl_TessCoord.x * inp[0].position
                    + gl_TessCoord.y * inp[1].position
                    + gl_TessCoord.z * inp[2].position;
    outp.position += exampleBumpedPositionOffset;

    outp.tangent = gl_TessCoord.x * inp[0].tangent
                    + gl_TessCoord.y * inp[1].tangent
                    + gl_TessCoord.z * inp[2].tangent;
    outp.bitangent = gl_TessCoord.x * inp[0].bitangent
                    + gl_TessCoord.y * inp[1].bitangent
                    + gl_TessCoord.z * inp[2].bitangent;
    outp.color = gl_TessCoord.x * inp[0].color
                    + gl_TessCoord.y * inp[1].color
                    + gl_TessCoord.z * inp[2].color;
}
