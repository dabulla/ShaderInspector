import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import Qt.labs.settings 1.0 as Labs
import fhac 1.0

Item {
    id: root
    property alias shaderModel: treeViewShaderVariables.model
    property alias parameterTarget: delegateManager.target
    property alias parametersProperty: delegateManager.parametersProperty
    TreeView {
        anchors.fill: parent
        id: treeViewShaderVariables
        horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff

        rowDelegate: Rectangle {
            property var row: styleData.row
            height: {
                var datatype = treeViewShaderVariables.model.data (treeViewShaderVariables.model.index(row, 0), ShaderModel.ParameterDatatype);
                var isSubroutine = treeViewShaderVariables.model.data (treeViewShaderVariables.model.index(row, 0), ShaderModel.ParameterIsSubroutine);
                var h = delegateManager.heightOfType(datatype, isSubroutine)
                return h
            }
        }
        TableViewColumn {
            id: colName
            role: "name"
            title: "name"
            width: 110
            resizable: true
            delegate: Label {
                // this is a hack to force the whole expression to reevaluate when styleData changed. Moreover the model sends dataCHanged events altough the name stays the same
                property bool found: treeViewShaderVariables.model.data (treeViewShaderVariables.model.index(styleData.row, 0), ShaderModel.ParameterFound) * (styleData.value !== "");
                color: found ? "black" : "grey"
                text: styleData.value
                font.capitalization: Font.Capitalize
                clip: true
            }
        }
//        TableViewColumn {
//            id: colType
//            role: "type"
//            title: "type"
//            width: 50
//            resizable: true
//            delegate: Label {
//                text: styleData.value.toString()
//                clip: true
//            }
//        }
        TableViewColumn {
            id: colDatatype
            role: "datatype"
            title: "datatype"
            width: 77
            resizable: true
            delegate: Label {
                text: styleData.value.toString()
                clip: true
            }
        }
        TableViewColumn {
            role: "data"
            title: "data"
            resizable: false
            width: treeViewShaderVariables.contentItem.width
                   - colName.width
                   //- colType.width
                   - colDatatype.width
            delegate: delegateManager.autoSelectComponent
        }
    }
    ShaderUniformDelegateManager {
        id: delegateManager
    }
}
