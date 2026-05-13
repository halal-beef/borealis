#include "protocol/device_information.h"

namespace Protocol {
    DeviceInformationApi::DeviceInformationApi(QObject *parent)
        : QObject(parent)
    {}

    DeviceInformationApi *DeviceInformationApi::instance()
    {
        static DeviceInformationApi api;
        return &api;
    }

    void DeviceInformationApi::updateDeviceInformation(const QString &deviceName, const QStringList &serialNumbers, const QString &firmwareVersion)
    {
        emit deviceInformationUpdated(deviceName, serialNumbers, firmwareVersion);
    }
}
