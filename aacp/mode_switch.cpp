#include <aacp/aacp_handler.h>

#include <protocol/mode_switch.h>
#include <aacp/control_command.h>

namespace AACP
{
    void AACPHandler::modeSwitchUpdated(const int mode)
    {
        QByteArray value;
        value.append(static_cast<quint8>(mode));
        value.append(static_cast<quint8>(0x00));
        value.append(static_cast<quint8>(0x00));
        value.append(static_cast<quint8>(0x00));

        const QByteArray cmd = makeControlCommand(ControlType::ListeningMode, value);
        emit sendPacket(cmd);
    }

    void AACPHandler::advertiseModeSwitches() const
    {
        // We have to make this device specific at some point, but i only have one device so rip
        Protocol::ModeSwitchApi::ModeTupleList modes = {
            {1, QStringLiteral("Off")},
            {2, QStringLiteral("Noise Cancellation")},
            {3, QStringLiteral("Transparency")},
            {4, QStringLiteral("Adaptive")},
        };

        QObject::connect(Protocol::ModeSwitchApi::instance(), &Protocol::ModeSwitchApi::modeUpdated, this, &AACPHandler::modeSwitchUpdated);

        Protocol::ModeSwitchApi::instance()->advertiseModes(modes);
    }
}