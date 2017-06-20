#version 400

//input
// (everything in 'view space')
in screenSpaceData
{
    vec3 normal;
    vec3 position;
    vec3 worldPos;
    vec4 color;
    vec2 texCoord;
} input;

// output
out vec4 out_Color;

// uniforms
uniform sampler2D diffuseTex;
uniform sampler2D specularTex;

uniform mat3 modelViewNormal;

// uniforms from Gui
uniform vec3 lightDir;
uniform float shininess;
uniform float jiggleFactor;
uniform vec3 baseColor;

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
    float lambertian = max(dot(lightDirection, normal), 0.0);
    float specular = 0.0;

    if(lambertian > 0.0)
    {
        vec3 reflectDir = reflect(-lightDirection, normal);
        vec3 viewDir = normalize(-pos);

        float specAngle = max(dot(reflectDir, viewDir), 0.0);
        specular = pow(specAngle, shininess * 128.0);
        specular *= texture(specularTex, uv).r;
    }
    vec4 diffuse = texture(diffuseTex, uv);

    return vec4( lambertian*baseColor.rgb*diffuse.rgb +
                          specular*vec3(1.0), 1.0);
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
    return phong( normalize(modelViewNormal * lightDir),
                  input.position,
                  normalize(input.normal),
                  uv);
}

subroutine(colorLookupType)
vec4 anisotrophicShading(vec2 uv)
{
    vec4 finalColor = vec4(0.0);
    for ( int i = 0; i < 2; i++) {
        vec3 offset = (i==0) ? input.worldPos : -input.worldPos;
        offset.y = 0.0;
        vec3 jiggledNormal = normalize( input.normal + jiggleFactor * normalize( offset*1.0 ) );
        finalColor += phong( normalize(modelViewNormal * lightDir),
                          input.position,
                          normalize(jiggledNormal),
                          vec2(0.0))*0.5;
    }
    return finalColor;
}


