import QtQuick 2.0
import QtQuick.Controls 2.12

Item {
    id:item1
    property int sizeH: 100
    property int sizeW: 100
    property string label: "-"
    property int variable: 0
    property var value: 0
    implicitHeight: sizeH
    implicitWidth: sizeW
    Dial{
        from: -180
        to:180
        value: item1.value

        implicitHeight: sizeH
        implicitWidth: sizeW

        onPressedChanged:
        {

            if (!pressed)
            {
                backEnd.setData(value,variable)
            }

        }

        Text {
            id: name
            text: qsTr(label)
            anchors.centerIn: parent
            font.pointSize: 16
        }

    }

}
