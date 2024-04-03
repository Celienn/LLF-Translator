#include "udpworker.h"

UDPWorker::UDPWorker(LLFTranslator *llf)
{
    socket = new QUdpSocket();
    socket->bind(QHostAddress::Any, 49000);
    parent = llf;
}

UDPWorker::~UDPWorker()
{
    socket->close();
    delete socket;
}

void UDPWorker::init()
{
    if (parent == nullptr) return;

    connect(socket, &QUdpSocket::readyRead, [=]() {

        while (socket->hasPendingDatagrams()) {
            QByteArray datagram;
            datagram.resize(socket->pendingDatagramSize());
            if(!datagram.size() && !socket->pendingDatagramSize()) break;
            
            socket->readDatagram(datagram.data(), datagram.size(), &dstAddr, &dstPort);

            int id, freq;
            char rref[400];
            if (!parseRREFRequest(datagram, &freq, &id, rref)) continue;
            qDebug() << "Received RREF request for " << rref << " with id " << id << " and frequency " << freq << "Hz";
            
            datagramIdMap[rref] = id;

            emit datagramReceived(rref,freq,id);
        }
    });
}

void UDPWorker::sendFrame(QString dataref, float value)
{
    QByteArray frame = generateDatagram(datagramIdMap[dataref], value);
    socket->writeDatagram(frame, dstAddr, dstPort);
}

void UDPWorker::sendFrame(QList<QPair<QString, float>> datagrams)
{
    QByteArray frame;

    frame.append("RREF,");

    for (const auto& datagram : datagrams) {
        QByteArray datagramFrame = generateDatagram(datagramIdMap[datagram.first], datagram.second, false);
        frame.append(datagramFrame);
    }

    socket->writeDatagram(frame, dstAddr, dstPort);
}

QByteArray UDPWorker::generateDatagram(int id, float value, bool header)
{
    QByteArray frame;

    if (header) frame.append("RREF,");
    frame.append(reinterpret_cast<const char*>(&id), sizeof(int));
    frame.append(reinterpret_cast<const char*>(&value), sizeof(float));
    return frame;
}


bool UDPWorker::parseRREFRequest(QByteArray datagram, int *frequency, int *id, char *rref)
{
    if (datagram.size() < 13 || datagram.size() > 413){
        qDebug() << "Invalid datagram size : " << datagram.size() << " bytes. Expected between 13 and 412 bytes.";
        return false;
    }

    if (!datagram.startsWith("RREF")) {
        qDebug() << "Invalid datagram : does not start with 'RREF'";
        return false;
    }

    memcpy(frequency,   datagram.data() + 5, 4);
    memcpy(id,          datagram.data() + 9, 4);
    memcpy(rref,        datagram.data() + 13, datagram.size() - 13);
    
    rref[datagram.size() - 13] = '\0';
    return true;
};