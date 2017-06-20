#version 400

//input
in screenSpaceData
{
    vec3 normal;
    vec3 positionViewSpace;
    vec4 color;
    vec2 texCoord;
    noperspective vec3 edgeDistance;
} input;

// output
out vec4 out_Color;

const vec4 gamma = vec4(vec3(2.2), 1.0);
const vec4 invgamma = 1.0/gamma;

// uniforms
uniform sampler2D diffuseTex;
uniform sampler2D specularTex;

uniform mat3 modelViewNormal;

// uniforms from Gui
uniform vec3 lightDir;

uniform vec4 baseColor;
uniform float shininess;

uniform struct LineInfo {
  float width;
} line;

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

vec4 wireFrame( vec4 color, vec4 wireFrameColor )
{
    // Find the smallest distance between the fragment and a triangle edge
    float d = min( input.edgeDistance.x, input.edgeDistance.y );
    d = min( d, input.edgeDistance.z );

    // Blend between line color and triangle color
    float mixVal = smoothstep(0.0, 1.5, line.width - d + 0.5 );

    //color = pow(color, gamma);
    //wireFrameColor = pow(wireFrameColor, gamma);
    vec4 c = mix( color, wireFrameColor, mixVal * wireFrameColor.a );
    //c = pow(c, invgamma);
    return c;
}

// define subroutines of type "colorLookupType"
// which can be assigned to a uniform
// variable of type "colorLookupType"

subroutine(colorLookupType)
vec4 lightOnly(vec2 uv)
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
    return lightOnly( uv ) * textureOnly( uv );
}

subroutine(colorLookupType)
vec4 phongShading(vec2 uv)
{
    return phong( normalize(modelViewNormal * lightDir),
                  input.positionViewSpace,
                  normalize(input.normal),
                  uv);
}

subroutine(colorLookupType)
vec4 wireFrameMode(vec2 uv)
{
    return wireFrame( vec4(0.0) , phongShading( uv ) );
}
