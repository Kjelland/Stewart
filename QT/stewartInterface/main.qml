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
        onGetData: {
            switch (variable)
            {
            case BackEnd.Ex:
            dialX.value = input
            break;

            case BackEnd.Ey:
            dialY.value = input
            break;

            case BackEnd.Ez:
            dialZ.value = input
            break;
            case BackEnd.Eroll:
            dialRoll.value = input
            break;
            case BackEnd.Epitch:
            dialPitch.value = input
            break;
            case BackEnd.Eyaw:
            dialYaw.value = input
            break;
            }
        }

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
        width: 640
        height: 480



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


        Row {
            id: row1
            width: 200
            height: 40

            Switch {
                id: element
                y: 0
                text: qsTr("Gyro Compensation")
                onClicked: backEnd.gyroConnectButtonClicked(!(backEnd.sta & (1<<3)))
                checked: backEnd.sta & (1<<3)
            }
            Switch {
                id: useYawButton
                y: 0
                text: qsTr("Enable Yaw Compentation")
                onClicked: backEnd.enableYawButtonClicked(!(backEnd.sta & (1<<2)))
                checked: backEnd.sta & (1<<2)
            }
            Switch {
                id: imuInitDone
                y: 0
                text: qsTr("Gyro Init done")
                checked: backEnd.sta & (1<<4)
                checkable: false
            }

        }

        Row {
            id: row
            width: 400
            height: 100
            MyDial{
                id:dialX
                variable: BackEnd.Ex
                label: "X"
                from: -2
                to:2

            }
            MyDial{
                id:dialY
                variable: BackEnd.Ey
                label: "Y"
                from: -2
                to:2

            }
            MyDial{
                id:dialZ
                variable: BackEnd.Ez
                label: "Z"
                from: -2
                to:2

            }
            MyDial{
                id:dialRoll
                variable: BackEnd.Eroll
                label: "R"
                from: -30
                to:30

            }
            MyDial{
                id:dialPitch
                variable: BackEnd.Epitch
                label: "P"
                from: -30
                to:30
            }
            MyDial{
                id:dialYaw
                variable: BackEnd.Eyaw
                label: "W"
                from: -30
                to:30
            }

//            Dial
//            {
//                id:setX
//                from: -180
//                to:180
//                value: 0
//                onPressedChanged:
//                {

//                    if (!pressed)
//                    {
//                        backEnd.setData(value,BackEnd.Ex)
//                    }

//                }
//            }
        }

        Column {
            id: column
            width: 200
            height: 400

            Label {
                id: labelPitch
                text: qsTr("Pitch: ") + backEnd.pitch.toFixed(2) + qsTr(" degrees")
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 15
            }

            Label {
                id: labelRoll
                text: qsTr("Roll: ") + backEnd.roll.toFixed(2) + qsTr(" degrees")
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 15
            }

            Label {
                id: labelYaw
                text: qsTr("Yaw: ") + backEnd.yaw.toFixed(2) + qsTr(" degrees")
                font.pointSize: 15
            }
        }

    }


}
