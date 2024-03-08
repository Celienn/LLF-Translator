#include "mainwindow.h"
#include "llftranslator.h"
#include <QApplication>
#include <QUdpSocket>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    LLFTranslator llf;
    llf.connect();
    // llf.readVar<double>(llf.translateXPlaneToMFS("sim/cockpit2/gauges/indicators/altitude_ft_pilot"), "meter", SIMCONNECT_DATATYPE_FLOAT64,[](double altitude){
    //     qDebug() << altitude;
    // },5);

    QUdpSocket *receiver = new QUdpSocket();
    QUdpSocket *sender = new QUdpSocket();

    // Écouter sur le port 49001
    receiver->bind(QHostAddress::Any, 49000);
    
    QObject::connect(receiver, &QUdpSocket::readyRead, [&]() {
        while (receiver->hasPendingDatagrams()) {
            QByteArray datagram;
            datagram.resize(receiver->pendingDatagramSize());
            QHostAddress senderAddress;
            quint16 senderPort;

            receiver->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);

            // Analyser le datagramme pour extraire la demande RREF
            // (Cela suppose que vous avez une méthode parseRREFRequest qui fait cela)
            int id;
            char rref[400];
            llf.parseRREFRequest(datagram, &id, rref);
            qDebug() << "Received RREF request for " << rref << " with id " << id;
            Sleep(7500);
            // Générer une réponse RREF et l'envoyer au client
            QByteArray response = llf.generateFrame(2, 5000.0);
            sender->writeDatagram(response, senderAddress, 49001);
        }
    });

    return a.exec();
}
