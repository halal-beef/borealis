#include <aacp/aacp_handler.h>
#include <protocol/device_information.h>

#include <QDebug>
#include <QtGlobal>

namespace AACP {
    AACPHandler::AACPHandler(QObject *parent)
        : ProtocolHandler(parent)
    {}

    QList<QByteArray> AACPHandler::initPackets() const
    {
        return { HANDSHAKE, REQ_NOTIFS, FLAGS };
    }

    bool AACPHandler::supportsDevice(const QBluetoothDeviceInfo &info) const
    {
    #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if (info.manufacturerIds().contains(0x004C))
            return true;
    #endif

        return !info.manufacturerData(0x004C).isEmpty();
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


    void AACPHandler::parseEarDetect(const QByteArray &data)
    {
        emit event(QStringLiteral("Ear detection: ") + QString(data.mid(6).toHex()));
    }

    void AACPHandler::parseControl(const QByteArray &data)
    {
        if (data.size() < 8)
            return;

        if (static_cast<quint8>(data[6]) != static_cast<quint8>(ControlType::ListeningMode)) {
            return;
        }

        const auto mode = static_cast<ListeningMode>(static_cast<quint8>(data[7]));
        QString modeStr;
        switch (mode) {
            case ListeningMode::Off: modeStr = QStringLiteral("Off"); break;
            case ListeningMode::NoiseCancelling: modeStr = QStringLiteral("Noise Cancellation"); break;
            case ListeningMode::Transparency: modeStr = QStringLiteral("Transparency"); break;
            case ListeningMode::Adaptive: modeStr = QStringLiteral("Adaptive"); break;
        }
        emit event(QStringLiteral("Listening mode: ") + modeStr);
    }
}