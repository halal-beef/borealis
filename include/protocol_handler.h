#ifndef PROTOCOL_HANDLER_H
#define PROTOCOL_HANDLER_H

#include <QObject>
#include <QByteArray>
#include <QList>
#include <QBluetoothAddress>
#include <QBluetoothDeviceInfo>

class ProtocolHandler : public QObject
{
    Q_OBJECT

public:
    explicit ProtocolHandler(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~ProtocolHandler() = default;

    virtual quint16 psm() const = 0;

    virtual QList<QByteArray> initPackets() const = 0;

    virtual bool supportsAddress(const QBluetoothAddress &address) const { (void)address; return false; }

    virtual bool supportsDevice(const QBluetoothDeviceInfo &info) const = 0;

    virtual void handlePacket(const QByteArray &data) = 0;

signals:
    void event(const QString &description);
    void batteryUpdated(int component, int status, int level);
    void sendPacket(const QByteArray &packet);
};

#endif // PROTOCOL_HANDLER_H