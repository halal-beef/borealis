#include <aacp/aacp_protocol.h>
#include <aacp/aacp_handler.h>

#include <protocol/battery.h>

#include <QDebug>

namespace AACP {
    BatteryComponentGeneric mapComponent(AACP::BatteryComponent component)
    {
        switch (component) {
            case AACP::BatteryComponent::Left: return BatteryComponentGeneric::Left;
            case AACP::BatteryComponent::Right: return BatteryComponentGeneric::Right;
            case AACP::BatteryComponent::Case: return BatteryComponentGeneric::Case;
        }
        return BatteryComponentGeneric::Left; // Should not reach here
    }

    ChargingStatusGeneric mapStatus(AACP::BatteryStatus status)
    {
        switch (status) {
            case AACP::BatteryStatus::Charging:
            case AACP::BatteryStatus::OptimisedCharging:
                return ChargingStatusGeneric::Charging;
            case AACP::BatteryStatus::Disconnected:
                return ChargingStatusGeneric::Disconnected;
            case AACP::BatteryStatus::NotCharging:
                return ChargingStatusGeneric::NotCharging;
        }
        return ChargingStatusGeneric::NotCharging; // Should not reach here
    }

    struct BatteryFieldSet
    {
        int componentOffset;
        int levelOffset;
        int statusOffset;
    };

    constexpr BatteryFieldSet kBatteryFields[] = { // Idx of Component, Level, Status
        {7, 9, 10},
        {12, 14, 15},
        {17, 19, 20},
    };

    void AACPHandler::parseBattery(const QByteArray &data)
    {
        if (data.size() != 22) {
            qDebug() << "Invalid battery packet size:" << data.size();
            return;
        }

        for (const BatteryFieldSet &fieldSet : kBatteryFields)
        {
            if (fieldSet.statusOffset >= data.size() ||
                fieldSet.levelOffset >= data.size() ||
                fieldSet.componentOffset >= data.size()) {
                continue;
            }

            const auto component = mapComponent(static_cast<AACP::BatteryComponent>(data[fieldSet.componentOffset]));
            const auto status = mapStatus(static_cast<AACP::BatteryStatus>(data[fieldSet.statusOffset]));
            const int level = static_cast<int>(static_cast<quint8>(data[fieldSet.levelOffset]));

            Protocol::BatteryApi::instance()->updateBattery(component, status, level);
        }
    }
}