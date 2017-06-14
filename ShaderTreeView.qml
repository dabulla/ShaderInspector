import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import Qt.labs.settings 1.0 as Labs
import fhac 1.0

Item {
    property alias parameters: delegateManager.parameters
    property alias shaderModel: treeViewShaderVariables.model
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
            width: 60
            resizable: true
            delegate: Label {
                text: styleData.value
                font.capitalization: Font.Capitalize
                clip: true
            }
        }
        TableViewColumn {
            id: colType
            role: "type"
            title: "type"
            width: 50
            resizable: true

            delegate: Label {
                text: styleData.value.toString()
                clip: true
            }
        }
        TableViewColumn {
            id: colDatatype
            role: "datatype"
            title: "datatype"
            width: 60
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
                   - colType.width
                   - colDatatype.width
            delegate: delegateManager.autoSelectComponent
        }
    }
    ShaderUniformDelegateManager {
        id: delegateManager
    }
}
