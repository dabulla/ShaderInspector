#version 400

layout( points ) in;
layout( line_strip, max_vertices = 2 ) out;

in worldVertex
{
    vec3 position;
    vec3 normal;
    vec4 color;
} input[];

out screenSpaceData
{
    vec3 normal;
    vec4 color;
} output;

uniform mat4 mvp;
uniform mat4 modelView;
uniform mat3 modelViewNormal;

uniform vec2 viewportSize;

uniform float axisLen;

void main(void)
{
    // custom backface culling
    vec4 posTrans = modelView * vec4(input[0].position, 1.0);
    vec3 posCamSpace = posTrans.xyz/posTrans.w;
    if(dot(normalize( modelViewNormal * input[0].normal ), normalize(-posCamSpace)) < 0.0)
    {
        return;
    }
    vec3 pos;
    pos = input[0].position;
    // base vertex. start of red line along normal
    output.normal = normalize( modelViewNormal * input[0].normal );
    output.color = input[0].color;
    gl_Position = mvp * vec4(pos, 1.0);
    EmitVertex();
    // second vertex of red line
    pos = input[0].position + normalize( input[0].normal ) * axisLen * 0.05;
    gl_Position = mvp * vec4(pos, 1.0);
    EmitVertex();
    EndPrimitive();
}
