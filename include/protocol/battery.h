#ifndef PROTOCOL_BATTERY_H
#define PROTOCOL_BATTERY_H

#include <QObject>

enum class BatteryComponentGeneric {
    Left = 0,
    Right = 1,
    Case = 2,
};

enum class ChargingStatusGeneric {
    Charging = 0,
    NotCharging = 1,
    Disconnected = 2,
};

namespace Protocol {
    class BatteryApi : public QObject
    {
        Q_OBJECT

    public:
        static BatteryApi *instance();

        void updateBattery(BatteryComponentGeneric component,
                        ChargingStatusGeneric status,
                        int level);

    signals:
        void batteryUpdated(BatteryComponentGeneric component, ChargingStatusGeneric status, int level);

    private:
        explicit BatteryApi(QObject *parent = nullptr);
    };
}

#endif // PROTOCOL_BATTERY_H
