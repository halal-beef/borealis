#ifndef PROTOCOL_DEVICE_INFORMATION_H
#define PROTOCOL_DEVICE_INFORMATION_H

#include <QObject>
#include <QString>
#include <QStringList>

namespace Protocol {
    class DeviceInformationApi : public QObject
    {
        Q_OBJECT

    public:
        static DeviceInformationApi *instance();

        void updateDeviceInformation(const QString &deviceName, const QStringList &serialNumbers, const QString &firmwareVersion);

    signals:
        void deviceInformationUpdated(const QString &deviceName, const QStringList &serialNumbers, const QString &firmwareVersion);

    private:
        explicit DeviceInformationApi(QObject *parent = nullptr);
    };
}

#endif // PROTOCOL_DEVICE_INFORMATION_H
