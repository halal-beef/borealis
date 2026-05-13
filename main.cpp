#include <QCoreApplication>
#include <QDebug>
#include <connection.h>
#include <aacp/aacp_handler.h>
#include <protocol/battery.h>
#include <protocol/device_information.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Connection connection(new AACP::AACPHandler);

    QObject::connect(&connection, &Connection::connected, []()
    {
        qDebug() << "Headphones connected";
    });

    QObject::connect(&connection, &Connection::disconnected, []()
    {
        qDebug() << "Headphones disconnected.";
    });

    QObject::connect(&connection, &Connection::protocolEvent, [](const QString &description)
    {
        qDebug().noquote() << description;
    });

    QObject::connect(Protocol::BatteryApi::instance(), &Protocol::BatteryApi::batteryUpdated, [](BatteryComponentGeneric comp, ChargingStatusGeneric status, int level)
    {
        const char *compStr = "Unknown component";
        const char *statusStr = "Unknown";

        switch (comp)
        {
            case BatteryComponentGeneric::Left: compStr = "Left"; break;
            case BatteryComponentGeneric::Right: compStr = "Right"; break;
            case BatteryComponentGeneric::Case: compStr = "Case"; break;
        }
    
        switch (status)
        {
            case ChargingStatusGeneric::Charging: statusStr = "Charging"; break;
            case ChargingStatusGeneric::NotCharging: statusStr = "Not Charging"; break;
            case ChargingStatusGeneric::Disconnected: statusStr = "Disconnected"; break;
        }
        qDebug() << "Battery update:" << compStr << statusStr << level << "%";
    });

    QObject::connect(Protocol::DeviceInformationApi::instance(),
                     &Protocol::DeviceInformationApi::deviceInformationUpdated,
                     [](const QString &deviceName, const QStringList &serialNumbers,
                        const QString &firmwareVersion) {
        qDebug() << "Device:" << deviceName;
        qDebug() << "  Serial Numbers:" << serialNumbers;
        qDebug() << "  Firmware:" << firmwareVersion;
    });

    qDebug() << "Scanning for headphones...";
    connection.scan();

    return a.exec();

}