#include <protocol/battery.h>

#include <QtGlobal>

namespace Protocol {
    BatteryApi::BatteryApi(QObject *parent)
        : QObject(parent)
    {}

    BatteryApi *BatteryApi::instance()
    {
        static BatteryApi api;
        return &api;
    }

    void BatteryApi::updateBattery(BatteryComponentGeneric component, ChargingStatusGeneric status, int level)
    {
        emit batteryUpdated(component, status, qBound(0, level, 100));
    }
}
