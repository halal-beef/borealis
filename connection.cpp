#include "connection.h"

#include <QDebug>

#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

Connection::Connection(ProtocolHandler *handler, QObject *parent)
    : QObject(parent)
    , m_handler(handler)
{
    m_handler->setParent(this);

    m_discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &Connection::deviceDiscovered);
    connect(m_handler, &ProtocolHandler::event, this, &Connection::protocolEvent);
    connect(m_handler, &ProtocolHandler::quirkPacketNeeded, this, &Connection::writeData);
}

Connection::~Connection()
{
    onDisconnected();
}

void Connection::scan()
{
    QBluetoothLocalDevice localDev;
    const QList<QBluetoothAddress> connected = localDev.connectedDevices();

    if (!connected.isEmpty()) {
        qDebug() << "Found" << connected.size() << "connected devices";
        for (const QBluetoothAddress &addr : connected) {
            qDebug() << "  Trying" << addr.toString();
            if (tryConnect(addr))
                return;
        }
    }

    qDebug() << "Waiting for devices";
    m_discoveryAgent->start();
}

void Connection::writeData(const QByteArray &data)
{
    if (m_rawSocket < 0) {
        qWarning() << "Not connected to device";
        return;
    }
    ::send(m_rawSocket, data.constData(), data.size(), 0);
}

void Connection::deviceDiscovered(const QBluetoothDeviceInfo &info)
{
    if (!m_handler->supportsDevice(info))
        return;

    qDebug() << "Discovered supported device:" << info.name();
    m_discoveryAgent->stop();
    tryConnect(info.address());
}

void Connection::dataAvailable()
{
    char buf[1024];
    const ssize_t len = ::recv(m_rawSocket, buf, sizeof(buf), 0);

    if (len <= 0) {
        qWarning() << "Socket closed or error:" << strerror(errno);
        onDisconnected();
        return;
    }

    m_handler->handlePacket(QByteArray(buf, static_cast<int>(len)));
}

void Connection::onDisconnected()
{
    if (m_notifier) {
        m_notifier->deleteLater();
        m_notifier = nullptr;
    }
    if (m_rawSocket >= 0) {
        ::close(m_rawSocket);
        m_rawSocket = -1;
    }
    emit disconnected();
}

bool Connection::tryConnect(const QBluetoothAddress &addr)
{
    const int sock = ::socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
    if (sock < 0) {
        qWarning() << "Failed to create L2CAP socket:" << strerror(errno);
        return false;
    }

    struct sockaddr_l2 sa = {};
    sa.l2_family = AF_BLUETOOTH;
    sa.l2_psm    = htobs(m_handler->psm());
    str2ba(addr.toString().toUtf8().constData(), &sa.l2_bdaddr);

    if (::connect(sock, reinterpret_cast<struct sockaddr *>(&sa), sizeof(sa)) < 0) {

        qDebug() << "L2CAP connect failed for" << addr.toString() << strerror(errno);
        ::close(sock);
        return false;
    }

    m_rawSocket = sock;
    qDebug() << "Connected to" << addr.toString()
             << "on PSM" << Qt::hex << m_handler->psm();

    for (const QByteArray &pkt : m_handler->initPackets())
        ::send(m_rawSocket, pkt.constData(), pkt.size(), 0);

    m_notifier = new QSocketNotifier(m_rawSocket, QSocketNotifier::Read, this);
    connect(m_notifier, &QSocketNotifier::activated, this, &Connection::dataAvailable);

    emit connected();
    return true;
}