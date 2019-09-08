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



    LynxManager _lynx;
    LynxUartQt _uart;

    StewartControl _controlDatagram;
    StewartFeedback _feedbackDatagram;

    LynxInfo _receiveInfo;

    QList<QSerialPortInfo> _portList;
    QSerialPortInfo _selectedPort;

    unsigned long _baudrate = 19200;

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






signals:
    void clearPortList();
    void addPort(const QString & portName);

    void rollChanged();
    void pitchChanged();

public slots:
    void setData(float input,E_variable var)
    {
        qDebug()<<input << " as "<<var;
       _controlDatagram.setpointX = input;
        _uart.send(_controlDatagram.setpointX.lynxId());
    }
    void sendData();
    void readData();
    void refreshPortList();
    void portSelected(int index);
    void connectButtonClicked();
    bool uartConnected() { return _uart.opened(); }
};

#endif // BACKEND_H
