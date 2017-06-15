#version 400

// input
in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexCoord;
in vec3 vertexTangent;
in vec3 vertexBitangent;

// output
out screenSpaceData
{
    vec3 normal;
    vec3 position;
    vec4 color;
    vec2 texCoord;
} output;

// uniforms
uniform mat4x4 modelViewProjectionMatrix;
uniform mat4x4 modelViewMatrix;
uniform mat3x3 modelViewNormalMatrix;

uniform mat4 modelMatrix;
uniform mat4 modelView;
uniform mat3 modelViewNormal;
uniform mat4 mvp;
uniform mat4 projectionMatrix;
uniform mat4 viewportMatrix;

// uniforms from Gui
uniform vec3 lightDir;

void main(void)
{
    vec4 positionProjected = mvp * vec4(vertexPosition, 1.0);
    vec4 positionViewSpace = modelView * vec4(vertexPosition, 1.0);
    gl_Position = positionProjected;
    output.position = positionViewSpace.xyz / positionViewSpace.w;
    output.texCoord = vertexTexCoord;
    output.normal = modelViewNormal * vertexNormal;

    // simple light effect. (not phong lighting)
    float lit = dot(normalize(vertexNormal), normalize(lightDir)) * .5 + .5;
    output.color = vec4(lit, lit, lit, 1.0);
}
