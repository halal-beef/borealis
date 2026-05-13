#ifndef PROTOCOL_BATTERY_H
#define PROTOCOL_BATTERY_H

#include <QObject>

// Generic battery component IDs for the app-level protocol.
enum class BatteryComponentGeneric {
    Left = 0,
    Right = 1,
    Case = 2,
};

// Generic charging status values the app understands.
// Optimised charging is Apple-specific and intentionally omitted.
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

} // namespace Protocol

#endif // PROTOCOL_BATTERY_H
