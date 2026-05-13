#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothLocalDevice>
#include <QBluetoothAddress>
#include <QSocketNotifier>
#include "protocol_handler.h"


class Connection : public QObject
{
    Q_OBJECT

public:
    explicit Connection(ProtocolHandler *handler, QObject *parent = nullptr);
    ~Connection();

    void scan();

    void writeData(const QByteArray &data);

signals:
    void connected();
    void disconnected();
    void protocolEvent(const QString &description);

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &info);
    void dataAvailable();
    void onDisconnected();

private:
    bool tryConnect(const QBluetoothAddress &addr);

    ProtocolHandler *m_handler;
    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent;
    QSocketNotifier *m_notifier  = nullptr;
    int m_rawSocket = -1;
};

#endif // CONNECTION_H