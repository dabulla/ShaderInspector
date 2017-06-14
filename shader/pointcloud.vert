#version 400

in vec3 vertexPosition;
in vec3 vertexNormal;
//in vec3 vertexColor;

out vec3 position;
out vec3 normal;
out vec3 color;

uniform mat4 modelMatrix;
uniform mat4 modelView;
uniform mat3 modelViewNormal;
uniform mat4 mvp;
uniform mat4 projectionMatrix;
uniform mat4 viewportMatrix;

uniform float theColorMulti;
uniform vec3 theColorSuper;
uniform bool negative;
uniform int mode;
uniform vec2 vector2;
uniform vec4 vector4;
//uniform vec3 color2;

void main()
{
    gl_Position = mvp * vec4(vertexPosition, 1.0);
    normal = normalize(modelViewNormal * vertexNormal);
    position = vec3(modelView * vec4(vertexPosition, 1.0));
//    float multi = theColorMulti;
//    if(negative) multi = 1.0-multi;
//    vec3 modeColor = vec3(theColorMulti);
//    if(mode == 0)
//    {
//        modeColor = theColorSuper;
//    }
//    else if(mode == 1)
//    {
//        modeColor = theColorSuper*0.333;
//    }
//    else if(mode == 2)
//    {
//        modeColor.rg = vector2.rg;
//        modeColor.b = vector2.r;
//    }
//    else if(mode == 3)
//    {
//        modeColor.rgb = vector4.rgb;
//        modeColor.rgb *= vector4.a;
//    }
    color = normal * theColorMulti;// * color2;
}
