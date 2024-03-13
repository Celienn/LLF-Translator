#ifndef LLFTUDPWORKER_H
#define LLFTUDPWORKER_H

#include <QUdpSocket>
#include <QHostAddress>
#include <QByteArray>
#include <QObject>

class LLFTranslator;

class UDPWorker : public QObject
{
    public:   
        UDPWorker(LLFTranslator *llf = nullptr);
        ~UDPWorker();
        void init();
    private:
        QUdpSocket *socket;
        QHostAddress dstAddr;
        quint16 dstPort;
        LLFTranslator *parent;
        QByteArray generateFrame(int id, float value);
        void parseRREFRequest(QByteArray datagram, int *id, char *rref);
    signals:
        void datagramReceived(char* rref, float value);

};

#endif // LLFTUDPWORKER_H