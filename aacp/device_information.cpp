#include <aacp/aacp_protocol.h>
#include <aacp/aacp_handler.h>

#include <protocol/device_information.h>

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

        // Device name
        if (segments.size() > 0 && !segments[0].isEmpty())
        {
            deviceName = QString::fromUtf8(segments[0]);
        }

        // AirPods Pro 2 have a quirk where another command has to be sent for some features to work
        if (segments.size() > 1 && !segments[1].isEmpty())
        {
            if(QString::fromUtf8(segments[1]) == "A3048")
            {
                qDebug() << "AirPods Pro 2 detected";
                const QByteArray quirkPkt = QByteArray::fromHex("040004004D00FF00000000000000");
                emit quirkPacketNeeded(quirkPkt);
            }
        }

        // First firmware version field
        if (segments.size() > 4 && !segments[4].isEmpty())
        {
            firmwareVersion = QString::fromUtf8(segments[4]);
        }

        // Serial numbers for case, left and right airpods
        if (segments.size() > 3 && !segments[3].isEmpty())
        {
            serialNumbers << QString::fromUtf8(segments[3]);
        }
        if (segments.size() > 8 && !segments[8].isEmpty())
        {
            serialNumbers << QString::fromUtf8(segments[8]);
        }
        if (segments.size() > 9 && !segments[9].isEmpty())
        {
            serialNumbers << QString::fromUtf8(segments[9]);
        }

        Protocol::DeviceInformationApi::instance()->updateDeviceInformation(deviceName, serialNumbers, firmwareVersion);
    }
}