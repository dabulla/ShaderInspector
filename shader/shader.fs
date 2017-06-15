#version 400

//input
// (everything in 'view space')
in screenSpaceData
{
    vec3 normal;
    vec3 position;
    vec4 color;
    vec2 texCoord;
} input;

// output
out vec4 out_Color;

// uniforms
uniform sampler2D diffuseTex;
uniform sampler2D specularTex;

uniform mat3 modelViewNormalMatrix;

// uniforms from Gui
uniform vec3 lightDir;
uniform float shininess;

// declare type of subroutine
subroutine vec4 colorLookupType(vec2);

// declare uniform variable to hold a specific
// subroutine of type "colorLookupType"
subroutine uniform colorLookupType colorMode;

void main(void)
{
    out_Color = colorMode( input.texCoord );
}

vec4 phong(vec3 lightDirection, vec3 pos, vec3 normal, vec2 uv)
{
    // CODE HIER
    // Uniform Variablen:
    // float shininess
    // sampler2D diffuseTex
    // sampler2D specularTex
    // Beispiel: http://www.lighthouse3d.com/tutorials/glsl-tutorial/directional-lights-per-pixel/
    return vec4(mix(vec3(1.0,1.0-shininess,0.0),normal, 0.07), 1.0); //< not implemented color
}

// define subroutines of type "colorLookupType"
// which can be assigned to a uniform
// variable of type "colorLookupType"

subroutine(colorLookupType)
vec4 simpleLightOnly(vec2 uv)
{
    return input.color;
}

subroutine(colorLookupType)
vec4 textureOnly(vec2 uv)
{
    return texture(diffuseTex, uv);
}

subroutine(colorLookupType)
vec4 lightAndTexture(vec2 uv)
{
    return simpleLightOnly( uv ) * textureOnly( uv );
}

subroutine(colorLookupType)
vec4 phongShading(vec2 uv)
{
    // since varyings are in view space, light
    // has do be transformed to view space too
    return phong( normalize(modelViewNormalMatrix * lightDir),
                  input.position,
                  normalize(input.normal),
                  uv);
}
