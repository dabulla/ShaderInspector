import QtQuick 2.8
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import Qt.labs.settings 1.0 as Labs
import fhac 1.0

Item {
    anchors.fill: parent
//    visible: true
//    width: 640
//    height: 480
//    title: qsTr("Shader Inspector")

    Shader {
        id: theShader
        anchors.fill: parent
        vertexShader: "/home/dbulla/ShaderEditor/shader/pointcloud.vert"
//        geometryShader:
//        tessellationControlShader:
//        tessellationEvaluationShader:
        fragmentShader:"/home/dbulla/ShaderEditor/shader/pointcloud.frag"
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
//                    Settings {
//                        id: settings
////                        onVertexShaderChanged: delegateManager.updateShader()
////                        onGeometryChanged: delegateManager.updateShader()
////                        ontessellationControlShaderChanged: delegateManager.updateShader()
////                        ontessellationEvaluationShaderChanged: delegateManager.updateShader()
////                        onFragmentShaderChanged: delegateManager.updateShader()
////                        onComputeShaderChanged: delegateManager.updateShader()
////                        onModelSourceChanged: if(delegateManager.currentScene) delegateManager.currentScene.modelSource = modelSource
//                        onAccepted: {
//                            //TODO: only if shader/model changed
//                            delegateManager.updateShader()
//                        }
//                    }
//                    Labs.Settings {
//                        category: "shader_filenames"
//                        property alias vertexShader: settings.vertexShader
//                        property alias geometryShader: settings.geometryShader
//                        property alias tessellationControlShader: settings.tessellationControlShader
//                        property alias tessellationEvaluationShader: settings.tessellationEvaluationShader
//                        property alias fragmentShader: settings.fragmentShader
//                        property alias computeShader: settings.computeShader
//                        property alias modelSource: settings.modelSource
//                        property alias sidebarWidth: sidebar.width
//                        property alias windowWidth: root.width
//                        property alias windowHeight: root.height
//                        Component.onCompleted: delegateManager.updateShader()
//                    }
//                    onClicked: settings.open()
                }

                Button {
                    Layout.fillWidth: true
                    text: "Reload"
                    onClicked: {
                    }
                }
            }
            ShaderTreeView {
                id: shaderTreeView
                Layout.fillWidth: true
                Layout.fillHeight: true
                shaderModel: theShader.model
            }
        }
//        NumberAnimation {
//            target: testParameter
//            property: "value"
//            duration: 1000
//            running: true
//            loops: Animation.Infinite
//            from: 0
//            to: 1
//        }
        Timer {
            interval: 1
            repeat: true
            running: true
            onTriggered: cameraController.update(0.001)
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
                    yScale: -1 // qml items are mirrored
                    origin.y: eventSource.height*0.5
                }

                anchors.fill: parent
                id: scene3d
                filename: ":/models/bunny.obj"
                shader: theShader
                camera: Camera {
                    id: theCamera
//                    position: Qt.vector3d(-90, 190, 100)
//                    viewCenter: Qt.vector3d(-20, 100, 0)
                }

//                ShaderParameter {
//                    id: testParameter
//                    name: "theColorMulti"
//                    value: 0
//                }
            }
        }
    }
}
