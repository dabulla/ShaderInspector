import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQml 2.2
import fhac 1.0

Item {
    id: root
    // For usage in a TableView. This needs styleData
    property alias autoSelectComponent: autoSelectComponent
    readonly property var parameters: []
    property var target: root
    property string parametersProperty: "parameters"

    function heightOfType(datatype, isSubroutine) {
        if(!datatype) return
        var singleHeight = 60;
        if( isSubroutine ) {
            return singleHeight;
        } else {
            switch(datatype.valueOf()) {
                case ShaderParameterInfo.FLOAT:
                case ShaderParameterInfo.DOUBLE:
                    return singleHeight;
                case ShaderParameterInfo.FLOAT_VEC2:
                    return singleHeight+singleHeight*0.5;
                case ShaderParameterInfo.FLOAT_VEC3:
                    return singleHeight+singleHeight*0.5*2;
                case ShaderParameterInfo.FLOAT_VEC4:
                    return singleHeight+singleHeight*0.5*3;
                case ShaderParameterInfo.INT:
                    return singleHeight;
                case ShaderParameterInfo.BOOL:
                    return singleHeight*0.5;
                case ShaderParameterInfo.FLOAT_MAT4:
                case ShaderParameterInfo.SAMPLER_2D:
                default:
                    return singleHeight*0.6;
            }
        }
    }

    function sync() {
        for (var i in root.target[root.parametersProperty]) {
            root.parameterChange(root.target[root.parametersProperty][i].name, root.target[root.parametersProperty][i].value)
        }
    }

    signal parameterAddedOrRemoved()
    signal parameterChange(string name, var value)
    Item {
        id: priv
        function setParameter(name, value, typename) {
            if( typeof value === "number" && isNaN(value)) return
            if( value === undefined) return
            var found = false
            for (var i in root.target[root.parametersProperty]) {
                if ( root.target[root.parametersProperty][i].name === name ) {
                    root.target[root.parametersProperty][i].value = value
                    found = true
                    break;
                }
            }
            if (!found) {
                root.target[root.parametersProperty].push({"name": name, "value": value })
                root.parameterAddedOrRemoved()
            } else {
                root.parameterChange(name, value)
            }
        }
    }

    Component {
        id: autoSelectComponent
        Item {
            anchors.fill: parent
            anchors.margins: 3
            id: item
            clip: true
            Component.onCompleted: {
                //styleData comes from TableView
                if( styleData.value.isSubroutine ) {
                    subroutineChooser.createObject(item, styleData.value)
                } else {
                    switch(styleData.value.datatype.valueOf()) {
                        case ShaderParameterInfo.FLOAT:
                        case ShaderParameterInfo.DOUBLE:
                            defaultFloatSlider.createObject(item, styleData.value)
                            break;
                        case ShaderParameterInfo.FLOAT_VEC2:
                            defaultVec2Control.createObject(item, styleData.value)
                            break;
                        case ShaderParameterInfo.FLOAT_VEC3:
                            defaultVec3Control.createObject(item, styleData.value)
                            break;
                        case ShaderParameterInfo.FLOAT_VEC4:
                            defaultVec4Control.createObject(item, styleData.value)
                            break;
                        case ShaderParameterInfo.INT:
                            var obj = defaultFloatSlider.createObject(item, styleData.value)
                            obj.isInt = true
                            break;
                        case ShaderParameterInfo.BOOL:
                            defaultBoolComponent.createObject(item, styleData.value)
                            break;
                        case ShaderParameterInfo.FLOAT_MAT4:
                            notAvaliableComponent.createObject(item, styleData.value)
                            break;
                        case ShaderParameterInfo.SAMPLER_2D:
                            notAvaliableComponent.createObject(item, styleData.value)
                            break;
                        default:
                            notAvaliableComponent.createObject(item, styleData.value)
                    }
                }
            }
        }
    }

    Component {
        id: subroutineChooser
        ColumnLayout {
            id: comp
            property string name
            property string qmlTypename
            property var subroutineValues
            Label {
                text: name + ":"
                font.capitalization: Font.Capitalize
            }
            ComboBox {
                id: subroutineCb
                model: subroutineValues
                onCurrentTextChanged: {
                    console.log("DBG: set:" + currentIndex)
                    priv.setParameter(name, currentIndex, qmlTypename);
                }
                textRole: ""
            }
        }
    }

    Component {
        id: defaultFloatSlider
        MinMaxSlider {
            anchors.left: parent.left
            anchors.right: parent.right
            property string name
            property string qmlTypename
            Component.onCompleted: priv.setParameter(name, value, qmlTypename);
            onValueChanged: priv.setParameter(name, value, qmlTypename);
        }
    }

    Component {
        id: defaultVec2Control
        ColumnLayout {
            anchors.left: parent.left
            anchors.right: parent.right
            id: comp
            property string name
            property bool isInt
            property string qmlTypename
            Component.onCompleted: updateUniform();
            function updateUniform() {
                priv.setParameter(name, Qt.vector2d(valueSliderx.value,
                                                    valueSlidery.value), qmlTypename);
            }
            MinMaxSlider {
                Layout.fillWidth: true
                id: valueSliderx
                isInt: comp.isInt
                minMaxEditable: false
                min: valueSlidery.min
                max: valueSlidery.max
                onValueChanged: updateUniform()
            }
            MinMaxSlider {
                Layout.fillWidth: true
                id: valueSlidery
                isInt: comp.isInt
                onValueChanged: updateUniform()
            }
        }
    }

    Component {
        id:defaultVec3Control
        ColumnLayout {
            id: comp
            anchors.left: parent.left
            anchors.right: parent.right
            property string name
            property bool isInt
            property string qmlTypename
            Component.onCompleted: updateUniform();
            function updateUniform() {
                priv.setParameter(name, Qt.vector3d(valueSliderx.value,
                                                    valueSlidery.value,
                                                    valueSliderz.value), qmlTypename);
            }
            MinMaxSlider {
                Layout.fillWidth: true
                id:valueSliderx
                isInt: comp.isInt
                minMaxEditable: false
                min: valueSliderz.min
                max: valueSliderz.max
                onValueChanged: updateUniform()
            }
            MinMaxSlider {
                Layout.fillWidth: true
                id:valueSlidery
                isInt: comp.isInt
                minMaxEditable: false
                min: valueSliderz.min
                max: valueSliderz.max
                onValueChanged: updateUniform()
            }
            MinMaxSlider {
                Layout.fillWidth: true
                id:valueSliderz
                isInt: comp.isInt
                onValueChanged: updateUniform()
            }
        }
    }
    Component {
        id:defaultVec4Control
        ColumnLayout {
            anchors.left: parent.left
            anchors.right: parent.right
            id: comp
            property string name
            property bool isInt
            property string qmlTypename
            Component.onCompleted: updateUniform();
            function updateUniform() {
                priv.setParameter(name, Qt.vector4d(valueSliderx.value,
                                                    valueSlidery.value,
                                                    valueSliderz.value,
                                                    valueSliderw.value), qmlTypename);
            }
            MinMaxSlider {
                Layout.fillWidth: true
                id:valueSliderx
                isInt: comp.isInt
                minMaxEditable: false
                min: valueSliderw.min
                max: valueSliderw.max
                onValueChanged: updateUniform()
            }
            MinMaxSlider {
                Layout.fillWidth: true
                id:valueSlidery
                isInt: comp.isInt
                minMaxEditable: false
                min: valueSliderw.min
                max: valueSliderw.max
                onValueChanged: updateUniform()
            }
            MinMaxSlider {
                Layout.fillWidth: true
                id:valueSliderz
                isInt: comp.isInt
                minMaxEditable: false
                min: valueSliderw.min
                max: valueSliderw.max
                onValueChanged: updateUniform()
            }
            MinMaxSlider {
                Layout.fillWidth: true
                id:valueSliderw
                isInt: comp.isInt
                onValueChanged: updateUniform()
            }
        }
    }

    Component {
        id: defaultBoolComponent
        CheckBox {
            id: comp
            property string name
            property string qmlTypename
            Component.onCompleted: priv.setParameter(name, checked, qmlTypename);
            onCheckedChanged: priv.setParameter(name, checked, qmlTypename);
        }
    }

    Component {
        id: notAvaliableComponent
        GroupBox {
            anchors.left: parent.left
            anchors.right: parent.right
            property var datatype
            Text {
                text: "(unsupported in Gui)"
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
            }
        }
    }
}
