#ifndef LLFTUDPWORKER_H
#define LLFTUDPWORKER_H

#include <QUdpSocket>
#include <QHostAddress>
#include <QByteArray>
#include <QObject>

class LLFTranslator;

class UDPWorker : public QObject
{
    Q_OBJECT

    public:   
        UDPWorker(LLFTranslator *llf = nullptr);
        ~UDPWorker();
        void init();
        void sendDatagram(QString datagram, float value);
    private:
        QUdpSocket *socket;
        QHostAddress dstAddr;
        quint16 dstPort;
        LLFTranslator *parent;
        QHash<QString, int> datagramIdMap;
        QByteArray generateFrame(int id, float value);
        
        void parseRREFRequest(QByteArray datagram,int *frequency, int *id, char *rref);
    signals:
        void datagramReceived(char* rref, int frequency);

};

#endif // LLFTUDPWORKER_H