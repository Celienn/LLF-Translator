#ifndef LLFTUDPWORKER_H
#define LLFTUDPWORKER_H

#include <QUdpSocket>
#include <QHostAddress>
#include <QByteArray>
#include <QObject>

class UDPWorker : public QObject
{
    Q_OBJECT

    public:   
        UDPWorker(QObject *parent = nullptr);
        ~UDPWorker();
        void init();
        void sendFrame(QString dataref, float value);
        void sendFrame(QList<QPair<QString, float>> datagrams);
    private:
        QUdpSocket *socket;
        QHostAddress dstAddr;
        quint16 dstPort;
        QObject *parent;
        QHash<QString, int> datagramIdMap;
        QByteArray generateDatagram(int id, float value, bool header = true);
        
        bool parseRREFRequest(QByteArray datagram,int *frequency, int *id, char *rref);
    signals:
        void datagramReceived(char* rref, int frequency, int id);

};

#endif // LLFTUDPWORKER_H
