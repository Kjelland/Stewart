import QtQuick 2.7
import QtQuick.Window 2.2
import backend 1.1
import QtQuick.Controls 2.12


Window
{
    visible: true
    width: 640
    height: 480
    title: qsTr("Stewie Control")

    BackEnd
    {
        id: backEnd
        onClearPortList:
        {
            portListModel.clear()
            portListModel.append({ text: qsTr("Select port") })
        }
        onAddPort: portListModel.append({ text: portName })
    }



    Column
    {
        spacing: 10
        padding: 20
        width: 200

        Row
        {
            width: parent.width
            spacing: 10
        }

        Dial
        {
            id:setX
            from: -180
            to:180
            value: 0
            onPressedChanged:
            {

                if (!pressed)
                {
                    backEnd.setData(value,BackEnd.Ex)
                }

            }
        }

        Button
        {
            width: parent.width
            id: button
            text: qsTr("Send all")
            onClicked: backEnd.sendData()
        }


        Row
        {
            spacing: 5

            ComboBox
            {
                width: 200
                id: portComboBox
                // currentIndex: -1
                model:
                    ListModel
                    {
                        id: portListModel
                        ListElement{text: qsTr("Select port")}
                    }

                onActivated:
                {
                    // console.log("Activated")
                    if (currentIndex > 0)
                    {
                        backEnd.portSelected(portComboBox.currentIndex - 1)
                        connectButton.enabled = true
                    }
                    else
                        connectButton.enabled = false
                }

                onPressedChanged:
                {
                    if (pressed)
                    {
                        backEnd.refreshPortList()
                        if (portListModel.count < 2)
                            connectButton.enabled = false
                        // console.log("Pressed")
                    }
                }
            }

            Button
            {
                width: 100
                id: connectButton
                enabled: false
                text: qsTr("Connect")
                onClicked:
                {
                    backEnd.connectButtonClicked()
                    if (backEnd.uartConnected())
                    {
                        text = qsTr("Disconnect")
                        portComboBox.enabled = false
                    }
                    else
                    {
                        text = qsTr("Connect")
                        portComboBox.enabled = true
                    }
                }
            }
        }

        Label {
            id: labelPitch
            text: qsTr("Pitch: ") + backEnd.pitch + qsTr(" degrees")
        }
        Label {
            id: labelRoll
            text: qsTr("Roll: ") + backEnd.roll + qsTr(" degrees")
        }

    }


}
