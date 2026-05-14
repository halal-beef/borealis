#ifndef AACP_PROTOCOL_H
#define AACP_PROTOCOL_H

#include <QByteArray>

static constexpr quint16 AAP_PSM = 0x1001;

namespace AACP {
    inline const QByteArray HANDSHAKE  = QByteArray::fromHex("00000400010002000000000000000000");
    inline const QByteArray REQ_NOTIFS = QByteArray::fromHex("040004000F00FFFFFFFF");
    inline const QByteArray FLAGS      = QByteArray::fromHex("040004004D00D700000000000000");
    inline const QByteArray HEADER     = QByteArray::fromHex("04000400");

    enum class OpCode : quint8 {
        Battery = 0x04,
        EarDetect = 0x06,
        Control = 0x09,
        DeviceInfo = 0x1D,
    };

    enum class ControlType : quint8 {
        ListeningMode = 0x0D,
    };

    enum class ListeningMode : quint8 {
        Off = 0x01,
        NoiseCancelling = 0x02,
        Transparency = 0x03,
        Adaptive = 0x04,
    };

    enum class BatteryComponent : quint8 {
        Right = 0x02,
        Left = 0x04,
        Case = 0x08,
    };

    enum class BatteryStatus : quint8 {
        Charging = 0x01,
        NotCharging = 0x02,
        Disconnected = 0x04,
        OptimisedCharging = 0x05,
    };

    enum class EarStatus : quint8 {
        EarIn = 0x00,
        EarOut = 0x01,
        InCase = 0x02,
        Disconnected = 0x03,
    };
}

#endif // AACP_PROTOCOL_H