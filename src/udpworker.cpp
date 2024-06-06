#include "udpworker.h"

UDPWorker::UDPWorker(QObject *parent)
: QObject(parent)
{
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::Any, 49000);
}

UDPWorker::~UDPWorker()
{
    socket->close();
    delete socket;
}

/**
 * @brief Initialise l'UDPWorker.
 * 
 * Cette méthode initialise le UDPWorker en connectant le socket à l'événement readyRead.
 * Lorsque des données sont disponibles pour être lues à partir du socket, elle lit le datagramme,
 * le parse avec parseRREFRequest et émet un signal datagramReceived afin que les données soit ensuite réceptioné par la classe principale.
 */
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

/**
 * @brief Envoie une trame de réponse RREF.
 * 
 * @param dataref Le dataref dont la valeur doit être envoyée.
 * @param value La valeur du dataref.
 * 
 * Permet d'envoyer un seul dataref par trame.
 */
void UDPWorker::sendFrame(QString dataref, float value)
{
    QByteArray frame = generateDatagram(datagramIdMap[dataref], value);
    socket->writeDatagram(frame, dstAddr, dstPort);
}

/**
 * @brief Envoie une trame de réponse RREF.
 * 
 * @param datagrams La liste des datarefs et valeurs à envoyer.
 * 
 * Permet d'envoyer plusieurs datarefs et valeurs en une seule trame.
 */
void UDPWorker::sendFrame(QList<QPair<QString, float>> datagrams)
{
    if (datagrams.isEmpty()) return;
    QByteArray frame;

    frame.append("RREF,");

    for (const auto& datagram : datagrams) {
        QByteArray datagramFrame = generateDatagram(datagramIdMap[datagram.first], datagram.second, false);
        frame.append(datagramFrame);
    }

    socket->writeDatagram(frame, dstAddr, dstPort);
}

/**
 * @brief Génère un datagramme de réponse RREF.
 * 
 * @param id L'identifiant de la requête.
 * @param value La valeur du dataref.
 * @param header Si le datagramme generé doit contenir l'en-tête "RREF,".
 * @return QByteArray Le datagramme généré.
 */
QByteArray UDPWorker::generateDatagram(int id, float value, bool header)
{
    QByteArray frame;

    if (header) frame.append("RREF,");
    frame.append(reinterpret_cast<const char*>(&id), sizeof(int));
    frame.append(reinterpret_cast<const char*>(&value), sizeof(float));
    return frame;
}

/**
 * @brief Parse une requête RREF.
 * 
 * @param datagram Le datagramme à parser.
 * @param frequency Un pointer où sera stocker la fréquence lue.
 * @param id Un pointer où sera stocker l'identifiant lue.
 * @param rref Un pointer où sera stocker le dataref lue.
 * 
 * @return true Si le datagramme est valide.
 * @return false Si le datagramme est invalide.
 */
bool UDPWorker::parseRREFRequest(QByteArray datagram, int *frequency, int *id, char *rref)
{
    if (datagram.size() < 13 || datagram.size() > 413){
        qWarning() << "Invalid datagram size : " << datagram.size() << " bytes. Expected between 13 and 412 bytes.";
        return false;
    }

    if (!datagram.startsWith("RREF")) {
        qWarning() << "Invalid datagram : does not start with 'RREF'";
        return false;
    }

    memcpy(frequency,   datagram.data() + 5, 4);
    memcpy(id,          datagram.data() + 9, 4);
    memcpy(rref,        datagram.data() + 13, datagram.size() - 13);
    
    rref[datagram.size() - 13] = '\0';
    return true;
};
