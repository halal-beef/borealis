#ifndef AACP_CONTROL_COMMAND_H
#define AACP_CONTROL_COMMAND_H

#include <QByteArray>

#include <aacp/aacp_protocol.h>

namespace AACP {
    QByteArray makeControlCommand(const ControlType identifier, const QByteArray &value);
}

#endif // AACP_CONTROL_COMMAND_H
