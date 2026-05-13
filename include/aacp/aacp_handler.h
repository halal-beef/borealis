#ifndef AACP_HANDLER_H
#define AACP_HANDLER_H

#include "protocol_handler.h"
#include "aacp/aacp_protocol.h"

namespace AACP {
    class AACPHandler : public ProtocolHandler
    {
        Q_OBJECT

    public:
        explicit AACPHandler(QObject *parent = nullptr);

        quint16 psm() const override { return AAP_PSM; }
        QList<QByteArray> initPackets() const override;
        bool supportsDevice(const QBluetoothDeviceInfo &info) const override;
        void handlePacket(const QByteArray &data) override;

    private:
        void parseBattery(const QByteArray &data);
        void parseEarDetect(const QByteArray &data);
        void parseControl(const QByteArray &data);
        void parseDeviceInfo(const QByteArray &data);
    };
}

#endif // AACP_HANDLER_H