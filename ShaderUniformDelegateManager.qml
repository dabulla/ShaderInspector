import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQml 2.2
import fhac 1.0

Item {
    id: root
    // For usage in a TableView. This needs styleData
    property alias autoSelectComponent: autoSelectComponent
    property var target
    property string parametersProperty: "parameters"
    property var treeView //TODO: there should be no dependecy on treeview

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

    Component {
        id: shaderParameterComponent
        ShaderParameter {}
    }
    Item {
        id: priv
        function setParameter(name, value, typename) {
            if( typeof value === "number" && isNaN(value)) return
            if( value === undefined) return
            if( root.target === undefined) {
                console.error("no target for uniform delegate manager")
                return
            }
            var found = false
            for (var i in root.target[root.parametersProperty]) {
                if ( root.target[root.parametersProperty][i].name === name ) {
                    root.target[root.parametersProperty][i].value = value
                    found = true
                    break;
                }
            }
            if (!found) {
                var param = shaderParameterComponent.createObject(root.target, {"name": name, "value": value })
                // This is a pre Qt5.9 version, sinde Qt5.8 does not like push() with QmlListProperty
                //root.target[root.parametersProperty].push(param)
                root.target.appendParameter(param)
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
                    var obj
                    switch(styleData.value.datatype.valueOf()) {
                        case ShaderParameterInfo.FLOAT:
                        case ShaderParameterInfo.DOUBLE:
                            obj = defaultFloatSlider.createObject(item, styleData.value)
                            break;
                        case ShaderParameterInfo.FLOAT_VEC2:
                            obj = defaultVec2Control.createObject(item, styleData.value)
                            break;
                        case ShaderParameterInfo.FLOAT_VEC3:
                            obj = defaultVec3Control.createObject(item, styleData.value)
                            break;
                        case ShaderParameterInfo.FLOAT_VEC4:
                            obj = defaultVec4Control.createObject(item, styleData.value)
                            break;
                        case ShaderParameterInfo.INT:
                            obj = defaultFloatSlider.createObject(item, styleData.value)
                            obj.isInt = true
                            break;
                        case ShaderParameterInfo.BOOL:
                            obj = defaultBoolComponent.createObject(item, styleData.value)
                            break;
                        case ShaderParameterInfo.FLOAT_MAT4:
                            obj = notAvaliableComponent.createObject(item, styleData.value)
                            break;
                        case ShaderParameterInfo.SAMPLER_2D:
                            obj = notAvaliableComponent.createObject(item, styleData.value)
                            break;
                        default:
                            obj = notAvaliableComponent.createObject(item, styleData.value)
                    }
                    // A bit hacky here. dynamic object creation destroys signal/slots
                    styleData.valueChanged.connect(function() {
                        if((typeof obj["found"] !== "undefined") && (typeof styleData !== "undefined")) {
                            obj.found = styleData.value.found
                        }
                    })
                }
            }
        }
    }

    Component {
        id: subroutineChooser
        ColumnLayout {
            anchors.left: parent.left
            anchors.right: parent.right
            id: comp
            property string name
            property string qmlTypename
            property var subroutineValues
            property bool found
            Label {
                text: name + ":"
                font.capitalization: Font.Capitalize
            }
            ComboBox {
                enabled: comp.found
                Layout.fillWidth: true
                id: subroutineCb
                model: subroutineValues
                onCurrentTextChanged: {
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
            property bool found
            enabled: found
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
            property bool found
            Component.onCompleted: updateUniform();
            function updateUniform() {
                priv.setParameter(name, Qt.vector2d(valueSliderx.value,
                                                    valueSlidery.value), qmlTypename);
            }
            MinMaxSlider {
                Layout.fillWidth: true
                id: valueSliderx
                isInt: comp.isInt
                enabled: comp.found
                minMaxEditable: false
                min: valueSlidery.min
                max: valueSlidery.max
                onValueChanged: updateUniform()
            }
            MinMaxSlider {
                Layout.fillWidth: true
                id: valueSlidery
                isInt: comp.isInt
                enabled: comp.found
                onValueChanged: updateUniform()
            }
        }
    }

    Component {
        id: defaultVec3Control
        ColumnLayout {
            id: comp
            anchors.left: parent.left
            anchors.right: parent.right
            property string name
            property bool isInt
            property string qmlTypename
            property bool found
            Component.onCompleted: updateUniform();
            function updateUniform() {
                priv.setParameter(name, Qt.vector3d(valueSliderx.value,
                                                    valueSlidery.value,
                                                    valueSliderz.value), qmlTypename);
            }
            MinMaxSlider {
                Layout.fillWidth: true
                id: valueSliderx
                isInt: comp.isInt
                enabled: comp.found
                minMaxEditable: false
                min: valueSliderz.min
                max: valueSliderz.max
                onValueChanged: updateUniform()
            }
            MinMaxSlider {
                Layout.fillWidth: true
                id: valueSlidery
                isInt: comp.isInt
                enabled: comp.found
                minMaxEditable: false
                min: valueSliderz.min
                max: valueSliderz.max
                onValueChanged: updateUniform()
            }
            MinMaxSlider {
                Layout.fillWidth: true
                id: valueSliderz
                isInt: comp.isInt
                enabled: comp.found
                onValueChanged: updateUniform()
            }
        }
    }
    Component {
        id: defaultVec4Control
        ColumnLayout {
            anchors.left: parent.left
            anchors.right: parent.right
            id: comp
            property string name
            property bool isInt
            property bool found
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
                id: valueSliderx
                isInt: comp.isInt
                enabled: comp.found
                minMaxEditable: false
                min: valueSliderw.min
                max: valueSliderw.max
                onValueChanged: updateUniform()
            }
            MinMaxSlider {
                Layout.fillWidth: true
                id: valueSlidery
                isInt: comp.isInt
                enabled: comp.found
                minMaxEditable: false
                min: valueSliderw.min
                max: valueSliderw.max
                onValueChanged: updateUniform()
            }
            MinMaxSlider {
                Layout.fillWidth: true
                id: valueSliderz
                isInt: comp.isInt
                enabled: comp.found
                minMaxEditable: false
                min: valueSliderw.min
                max: valueSliderw.max
                onValueChanged: updateUniform()
            }
            MinMaxSlider {
                Layout.fillWidth: true
                id: valueSliderw
                isInt: comp.isInt
                enabled: comp.found
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
            property bool found
            enabled: found
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
