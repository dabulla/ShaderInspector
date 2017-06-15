import QtQuick 2.8
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQml 2.2
import Qt.labs.settings 1.0 as Labs
import fhac 1.0

Item {
    id: root
    anchors.fill: parent
//    visible: true
//    width: 640
//    height: 480
//    title: qsTr("Shader Inspector")

    Shader {
        id: theShader
        anchors.fill: parent
        vertexShader: settings.vertexShader
        geometryShader: settings.geometryShader
        tessellationControlShader: settings.tessellationControlShader
        tessellationEvaluationShader: settings.tessellationEvaluationShader
        fragmentShader: settings.fragmentShader
    }
    SplitView {
        orientation: Qt.Horizontal
        anchors.fill: parent
        ColumnLayout {
            id: sidebar
            width: 400
            RowLayout {
                Button {
                    Layout.fillWidth: true
                    text: "Settings"
                    Settings {
                        id: settings
//                        onVertexShaderChanged: delegateManager.updateShader()
//                        onGeometryChanged: delegateManager.updateShader()
//                        ontessellationControlShaderChanged: delegateManager.updateShader()
//                        ontessellationEvaluationShaderChanged: delegateManager.updateShader()
//                        onFragmentShaderChanged: delegateManager.updateShader()
//                        onComputeShaderChanged: delegateManager.updateShader()
//                        onModelSourceChanged: if(delegateManager.currentScene) delegateManager.currentScene.modelSource = modelSource
                        onAccepted: {
                        }
                    }
                    Labs.Settings {
                        category: "shader_filenames"
                        property alias vertexShader: settings.vertexShader
                        property alias geometryShader: settings.geometryShader
                        property alias tessellationControlShader: settings.tessellationControlShader
                        property alias tessellationEvaluationShader: settings.tessellationEvaluationShader
                        property alias fragmentShader: settings.fragmentShader
                        property alias computeShader: settings.computeShader
                        //property alias modelSource: settings.modelSource
                        property alias sidebarWidth: sidebar.width
                        property alias windowWidth: root.width
                        property alias windowHeight: root.height
                        property alias rotationSpeed: settings.rotationSpeed
                    }
                    onClicked: settings.open()
                }

                Button {
                    Layout.fillWidth: true
                    text: "Reload Shader"
                    onClicked: {
                        theShader.reload()
                    }
                }
            }
            ShaderTreeView {
                id: shaderTreeView
                Layout.fillWidth: true
                Layout.fillHeight: true
                shaderModel: theShader.model
                parameterTarget: scene3d
                parametersProperty: "parameters"
            }
        }
        Timer {
            interval: 1
            repeat: true
            running: true
            onTriggered: {
                cameraController.update(0.001)
                timeParameter.value += 0.001
                rotationParameter.value += 0.001*settings.rotationSpeed
            }
        }

        CameraController {
            id: cameraController
            camera: theCamera
            eventSource: eventSource
        }

        MouseArea {
            id: eventSource
            hoverEnabled: true
            Layout.minimumWidth: 50
            Layout.fillHeight: true
            focus: true
            Model3D {
                transform: Scale {
                    yScale: -1 // qml items are mirrored upside down
                    origin.y: eventSource.height*0.5
                }
                primitiveType: settings.primitiveType
                anchors.fill: parent
                id: scene3d
                filename: ":/models/bunny.obj" // settings.modelSource
                shader: theShader
                camera: Camera {
                    id: theCamera
                    //position: Qt.vector3d(-90, 190, 100)
                    viewCenter: Qt.vector3d(-0.06, 0.12, 0)
                }
                ShaderParameter {
                    id: timeParameter
                    name: "time"
                    value: 0.2
                }
                ShaderParameter {
                    id: rotationParameter
                    name: "rotation"
                    value: 0.2
                }
                ShaderParameter {
                    name: "diffuseTex"
                    value: ":/textures/75692-diffuse.jpg"
//                    Image {
//                        source: ":/textures/75692-diffuse.jpg"
//                        width: 128
//                        height: 128
//                    }
                }
            }
        }
    }
}
