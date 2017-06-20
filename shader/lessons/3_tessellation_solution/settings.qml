import QtQuick 2.0

Item {
    property string basename
    property string vertexShader: basename + "shader.vert"
    property string tessellationControlShader: basename + "shader_tcs.glsl"
    property string tessellationEvaluationShader: basename + "shader_tes.glsl"
    property string geometryShader: basename + "shader.geom"
    property string fragmentShader: basename + "shader.frag"
    property string computeShader: ""
}
