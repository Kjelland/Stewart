#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QtDebug>
#include "lynxstructure.h"
#include "lynxuartqt.h"
#include "stewartcontrol.h"
// #include "teststruct.h"


class BackEnd : public QObject
{
    Q_OBJECT


    Q_PROPERTY(float roll READ roll NOTIFY rollChanged)
    Q_PROPERTY(float pitch READ pitch  NOTIFY pitchChanged)
    Q_PROPERTY(float yaw READ yaw  NOTIFY yawChanged)





    LynxManager _lynx;
    LynxUartQt _uart;

    StewartControl _controlDatagram;
    StewartFeedback _feedbackDatagram;

    LynxInfo _receiveInfo;

    QList<QSerialPortInfo> _portList;
    QSerialPortInfo _selectedPort;

    unsigned long _baudrate = 115200;

    bool updateDial;

public:
    enum E_variable{
        Ex,
        Ey,
        Ez,
        Eroll,
        Epitch,
        Eyaw
    };
    Q_ENUM(E_variable)
    explicit BackEnd(QObject *parent = nullptr);
    ~BackEnd() { _uart.close(); }

    float roll() const { return _feedbackDatagram.imuRoll; }
    float pitch() const { return _feedbackDatagram.imuPitch; }
    float yaw() const { return _feedbackDatagram.imuYaw; }






signals:
    void clearPortList();
    void addPort(const QString & portName);

    void rollChanged();
    void pitchChanged();
    void yawChanged();

    void getData(float input,E_variable variable);

public slots:
    void setData(float input,E_variable var)
    {
        qDebug()<<input << " as "<<var;
        updateDial=true;
        switch (var)
        {
        case Ex:
            _controlDatagram.setpointX = input;
            _uart.send(_controlDatagram.setpointX.lynxId());
            break;
        case Ey:
            _controlDatagram.setpointY = input;
            _uart.send(_controlDatagram.setpointY.lynxId());
            break;
        case Ez:
            _controlDatagram.setpointZ = input;
            _uart.send(_controlDatagram.setpointZ.lynxId());
            break;
        case Eroll:
            _controlDatagram.setpointRoll = input;
            _uart.send(_controlDatagram.setpointRoll.lynxId());
            break;
        case Epitch:
            _controlDatagram.setpointPitch = input;
            _uart.send(_controlDatagram.setpointPitch.lynxId());
            break;
        case Eyaw:
            _controlDatagram.setpointYaw = input;
            _uart.send(_controlDatagram.setpointYaw.lynxId());
            break;
        }

    }
    void sendData();
    void readData();
    void refreshPortList();
    void portSelected(int index);
    void connectButtonClicked();
    bool uartConnected() { return _uart.opened(); }
    void gyroConnectButtonClicked(bool state);
};

#endif // BACKEND_H
