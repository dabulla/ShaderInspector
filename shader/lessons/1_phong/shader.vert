#version 400

// inp
in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexCoord;
in vec3 vertexTangent;
in vec3 vertexBitangent;

// outp
out screenSpaceData
{
    vec3 normal;
    vec3 position;
    vec3 worldPos;
    vec4 color;
    vec2 texCoord;
} outp;

// uniforms
uniform mat4 modelMatrix;
uniform mat4 modelView;
uniform mat3 modelViewNormal;
uniform mat4 mvp;
uniform mat4 projectionMatrix;

// uniforms from Gui
uniform vec3 lightDir;

void main(void)
{
    vec4 positionProjected = mvp * vec4(vertexPosition, 1.0);
    vec4 positionViewSpace = modelView * vec4(vertexPosition, 1.0);
    gl_Position = positionProjected;
    outp.position = positionViewSpace.xyz / positionViewSpace.w;
    outp.worldPos = vertexPosition;
    outp.texCoord = vertexTexCoord;
    outp.normal = modelViewNormal * vertexNormal;

    // simple light effect. (not phong lighting)
    float lit = dot(normalize(vertexNormal), normalize(lightDir)) * .5 + .5;
    outp.color = vec4(lit, lit, lit, 1.0);
}
