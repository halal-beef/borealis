#include <aacp/aacp_protocol.h>
#include <aacp/aacp_handler.h>

#include <protocol/device_information.h>
#include <protocol/mode_switch.h>

#include <QDebug>

namespace AACP {
    void AACPHandler::parseDeviceInfo(const QByteArray &data)
    {
        QString deviceName;
        QStringList serialNumbers;
        QString firmwareVersion;

        if (data.size() < 6)
        {
            qDebug() << "Invalid device info packet size:" << data.size();
            return;
        }

        const QByteArray payload = data.mid(6);
        
        if (payload.size() < 5)
        {
            qDebug() << "Device info payload too small" << payload.size();
            return;
        }
        
        const QByteArray stringsPayload = payload.mid(5);
        const QList<QByteArray> segments = stringsPayload.split('\0');

        if (segments.isEmpty())
        {
            return;
        }

        if (segments.size() < 9)
        {
            qDebug() << "Unable to identify device. " << segments.size();
        }

        for (int i = 0; i < 9; ++i)
        {
            if(segments[i].isEmpty())
            {
                qDebug() << "Some Device information is empty. Unable to identify.";
                return;
            }
        }

        // Device name
        deviceName = QString::fromUtf8(segments[0]);

        // AirPods Pro 2 have a quirk where another command has to be sent for some features to work
        if(QString::fromUtf8(segments[1]) == "A3048")
        {
            qDebug() << "AirPods Pro 2 detected";
            QByteArray quirkPayload = HEADER;
            quirkPayload.append(QByteArray::fromHex("D00FF00000000000000"));
            emit sendPacket(quirkPayload);
        }

        // First firmware version field
        firmwareVersion = QString::fromUtf8(segments[4]);

        // Serial numbers for case, left and right airpods
        serialNumbers << QString::fromUtf8(segments[3]);
        serialNumbers << QString::fromUtf8(segments[8]);
        serialNumbers << QString::fromUtf8(segments[9]);

        Protocol::DeviceInformationApi::instance()->updateDeviceInformation(deviceName, serialNumbers, firmwareVersion);
        advertiseModeSwitches(); // Kinda dirty but we ball
    }
}