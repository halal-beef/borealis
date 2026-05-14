#include <aacp/aacp_protocol.h>
#include <aacp/aacp_handler.h>

#include <protocol/ear_detection.h>

#include <QDebug>

namespace AACP {
    EarStatusGeneric mapStatus(AACP::EarStatus status)
    {
        switch (status) {
            case AACP::EarStatus::EarIn:
                return EarStatusGeneric::EarIn;
            case AACP::EarStatus::EarOut:
                return EarStatusGeneric::EarOut;
            case AACP::EarStatus::InCase:
                return EarStatusGeneric::InCase;
            case AACP::EarStatus::Disconnected:
                return EarStatusGeneric::Disconnected;
        }
        return EarStatusGeneric::Disconnected; // Should not reach here
    }

    void AACPHandler::parseEarDetect(const QByteArray &data)
    {
        if (data.size() != 8) {
            qDebug() << "Invalid ear detection packet size:" << data.size();
            return;
        }

        const auto status_r = mapStatus(static_cast<AACP::EarStatus>(static_cast<uint8_t>(data[6])));
        const auto status_l = mapStatus(static_cast<AACP::EarStatus>(static_cast<uint8_t>(data[7])));

        Protocol::EarDetectionApi::instance()->updateEarDetection(EarComponentGeneric::Left, status_l);
        Protocol::EarDetectionApi::instance()->updateEarDetection(EarComponentGeneric::Right, status_r);
    }
}