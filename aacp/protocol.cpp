#include <aacp/aacp_handler.h>
#include <protocol/device_information.h>

#include <QDebug>
#include <QtGlobal>
#include <QBluetoothAddress>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusVariant>
#include <QDBusConnection>
#include <QRegularExpression>

namespace AACP {
    AACPHandler::AACPHandler(QObject *parent)
        : ProtocolHandler(parent)
    {}

    QList<QByteArray> AACPHandler::initPackets() const
    {
        return { HANDSHAKE, REQ_NOTIFS, FLAGS };
    }

    bool AACPHandler::supportsAddress(const QBluetoothAddress &address) const
    {
        const QString addr = address.toString().replace(':', '_');
        QString path = QStringLiteral("/org/bluez/hci0/dev_%1").arg(addr);
        QDBusInterface propsIface(QStringLiteral("org.bluez"), path, QStringLiteral("org.freedesktop.DBus.Properties"), QDBusConnection::systemBus());
 
        if (propsIface.isValid())
        {
            QDBusReply<QVariant> reply = propsIface.call(QStringLiteral("Get"), QStringLiteral("org.bluez.Device1"), QStringLiteral("Modalias"));

            if (reply.isValid())
            {
                QVariant v = reply.value();
                QString modalias;

                if (v.canConvert<QDBusVariant>())
                    modalias = v.value<QDBusVariant>().variant().toString();
                else
                    modalias = v.toString();

                if (!modalias.isEmpty())
                {
                    QRegularExpression re(QStringLiteral("v([0-9A-Fa-f]{4})"));
                    QRegularExpressionMatch m = re.match(modalias);
                    if (m.hasMatch())
                    {
                        bool ok = false;
                        const quint16 vendor = static_cast<quint16>(m.captured(1).toUShort(&ok, 16));
                        if (ok && vendor == 0x004C)
                            return true;
                    }
                }
            }
        }
        return false;
    }

    bool AACPHandler::supportsDevice(const QBluetoothDeviceInfo &info) const
    {
        return supportsAddress(info.address());
    }

    void AACPHandler::handlePacket(const QByteArray &data)
    {
        if (data.size() < 6 || data.left(4) != HEADER)
            return;

        const auto opcode = static_cast<OpCode>(static_cast<quint8>(data[4]));
        switch (opcode)
        {
            case OpCode::Battery: parseBattery(data); break;
            case OpCode::EarDetect: parseEarDetect(data); break;
            case OpCode::Control: parseControl(data); break;
            case OpCode::DeviceInfo: parseDeviceInfo(data); break;
        }
    }
}