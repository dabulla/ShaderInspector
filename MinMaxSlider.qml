import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4

ColumnLayout {
    id:comp
    property bool minMaxEditable: true
    property alias min: minVal.text
    property alias max: maxVal.text
    property bool isInt
    property alias value: valueSlider.value
    property bool enabled: true
    Slider {
        Layout.fillWidth: true
        id: valueSlider
        minimumValue: comp.isInt ? parseInt(minVal.text) : parseFloat(minVal.text)
        maximumValue: comp.isInt ? parseInt(maxVal.text) : parseFloat(maxVal.text)
        stepSize: isInt ? 1.0 : 0.0
        enabled: comp.enabled
    }
    RowLayout {
        Layout.fillWidth: true
        visible: minMaxEditable
        Label {
            enabled: comp.enabled
            text: "Min:"
        }
        IntValidator { id: intValidatorMin }
        DoubleValidator { id: doubleValidatorMin }
        IntValidator { id: intValidatorMax }
        DoubleValidator { id: doubleValidatorMax }
        TextField {
            enabled: comp.enabled
            id: minVal
            text: "0"
            validator: comp.isInt ? intValidatorMin : doubleValidatorMin
            Layout.fillWidth: true
            Layout.maximumWidth: 60
            Layout.minimumWidth: 10
        }
        Label {
            enabled: comp.enabled
            text: "Max:"
        }
        TextField {
            enabled: comp.enabled
            id: maxVal
            text: "1"
            validator: comp.isInt ? intValidatorMax : doubleValidatorMax
            Layout.fillWidth: true
            Layout.maximumWidth: 60
            Layout.minimumWidth: 10
        }
    }
}
