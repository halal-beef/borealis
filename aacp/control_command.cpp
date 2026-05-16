#include <aacp/aacp_handler.h>

#include <aacp/control_command.h>

namespace AACP
{
    void AACPHandler::parseControl(const QByteArray &data)
    {
        if (data.size() < 8)
            return;

        if (static_cast<quint8>(data[6]) != static_cast<quint8>(ControlType::ListeningMode)) {
            return;
        }

        const auto mode = static_cast<ListeningMode>(static_cast<quint8>(data[7]));
        QString modeStr;
        switch (mode) {
            case ListeningMode::Off: modeStr = QStringLiteral("Off"); break;
            case ListeningMode::NoiseCancelling: modeStr = QStringLiteral("Noise Cancellation"); break;
            case ListeningMode::Transparency: modeStr = QStringLiteral("Transparency"); break;
            case ListeningMode::Adaptive: modeStr = QStringLiteral("Adaptive"); break;
        }
        emit event(QStringLiteral("Listening mode: ") + modeStr);
    }

    QByteArray makeControlCommand(const ControlType identifier, const QByteArray &value)
    {
        QByteArray cmd = HEADER;
        cmd.append(static_cast<quint8>(OpCode::Control));
        cmd.append(static_cast<quint8>(0x00));
        cmd.append(static_cast<quint8>(identifier));
        cmd.append(value);
        return cmd;
    }
}