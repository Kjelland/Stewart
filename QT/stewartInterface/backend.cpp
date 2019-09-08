#include "backend.h"

BackEnd::BackEnd(QObject *parent) :
    QObject(parent),
    _lynx(0x25),
    _uart(_lynx),
    _controlDatagram(_lynx, 0x22),
    _feedbackDatagram(_lynx, 0x23)
{
   //  _uart.open(4, 115200);

    connect(_uart.portPointer(), SIGNAL(readyRead()), this, SLOT(readData()));
}

void BackEnd::sendData()
{
    _uart.send(_controlDatagram.lynxId());
}

void BackEnd::readData()
{
    _receiveInfo = _uart.update();


    if(_receiveInfo.state != eNoChange)
    {


        if(_receiveInfo.state == eNewDataReceived)
        {

            emit rollChanged();
            emit pitchChanged();
        }
    }
}

void BackEnd::refreshPortList()
{
    this->clearPortList();
    _portList = QSerialPortInfo::availablePorts();

    QString tempName;
    for (int i = 0; i < _portList.count(); i++)
    {
        tempName = _portList.at(i).portName();
        tempName += " - ";
        tempName += _portList.at(i).description();

        if (!_portList.at(i).isNull())
            this->addPort(tempName);
    }
}

void BackEnd::portSelected(int index)
{
    qDebug() << "Port selsected";
    qDebug() << "Index: " << index;
    if ((index < 0) || (index >= _portList.count()))
        return;

    _selectedPort = _portList.at(index);
}

void BackEnd::connectButtonClicked()
{
    // qDebug() << "Button clicked";
    if (_uart.opened())
    {
        qDebug() << "Closing port";
        _uart.close();
    }
    else
    {
        qDebug() << "Attempting to open";
        qDebug() << _selectedPort.portName();
        qDebug() << _selectedPort.description();
        if (_uart.open(_selectedPort, _baudrate))
            qDebug() << "Opened successfully";
        else
            qDebug() << "Open failed";
    }

}
