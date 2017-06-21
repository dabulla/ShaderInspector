#version 400

layout( triangles ) in;
layout( triangle_strip, max_vertices = 3 ) out;

in worldVertex
{
    vec3 position;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
    vec4 color;
} inp[];

out screenSpaceData
{
    vec3 normal;
    vec3 positionViewSpace;
    vec4 color;
    vec2 texCoord;
    noperspective vec3 edgeDistance;
} outp;

uniform mat4 mvp;
uniform mat4 modelView;
uniform mat3 modelViewNormal;

uniform vec2 viewportSize;

uniform float axisLen;

// calculate perpendicular distance for each vertex from it's opposite edge.
// the minimal value of the three results is used for wireframe-mode in fragment-shader.
vec3[3] calcEdgeDistances(vec4 clipSpace0, vec4 clipSpace1, vec4 clipSpace2)
{
    // convert to viewport-space for pixel - distances
    // ( this does nothing more than multyplying x * width/2 and y * height/2 )
    vec2 p0 = 0.5 * viewportSize * ( clipSpace0.xy / clipSpace0.w );
    vec2 p1 = 0.5 * viewportSize * ( clipSpace1.xy / clipSpace1.w );
    vec2 p2 = 0.5 * viewportSize * ( clipSpace2.xy / clipSpace2.w );

    // Calculate lengths of 3 edges of triangle
    float a = length( p1 - p2 );
    float b = length( p2 - p0 );
    float c = length( p1 - p0 );

    // Calculate internal angles using the cosine rule
    float alpha = acos( ( b * b + c * c - a * a ) / ( 2.0 * b * c ) );
    float beta = acos( ( a * a + c * c - b * b ) / ( 2.0 * a * c ) );

    // Calculate the perpendicular distance of each vertex from the opposing edge
    vec3 edgeDists[3];
    edgeDists[0] = vec3(abs( c * sin( beta ) ), 0.0, 0.0);
    edgeDists[1] = vec3(0.0, abs( c * sin( alpha ) ), 0.0);
    edgeDists[2] = vec3(0.0, 0.0, abs( b * sin( alpha ) ));
    return edgeDists;
}


void main(void)
{
//    //custom backface culling
//    vec4 posTrans = modelView * vec4(inp[0].position, 1.0);
//    vec3 posCamSpace = posTrans.xyz/posTrans.w;
//    if(dot(normalize( modelViewNormal * inp[0].normal), normalize(-posCamSpace)) < 0.0)
//    {
//        return;
//    }

    vec4 positionClipSpace[3];
    positionClipSpace[0] = mvp * vec4(inp[0].position, 1.0);
    positionClipSpace[1] = mvp * vec4(inp[1].position, 1.0);
    positionClipSpace[2] = mvp * vec4(inp[2].position, 1.0);

    vec3 edgeDists[] = calcEdgeDistances( positionClipSpace[0],
                                        positionClipSpace[1],
                                        positionClipSpace[2]);

    for(int i=0 ; i<3 ; ++i)
    {
        outp.normal = modelViewNormal * inp[i].normal;
        outp.positionViewSpace = (modelView * vec4(inp[i].position, 1.0)).xyz;
        outp.color = inp[i].color;
        outp.texCoord = inp[i].texCoord;
        outp.edgeDistance = edgeDists[i];
        //gl_Position = gl_in[i].gl_Position; // would also work
        gl_Position = positionClipSpace[i];
        EmitVertex();
    }
    EndPrimitive();
}
