#include "mainwindow.h"

MainWindow *g_mainWindow = nullptr;

void DebugOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context)

    // Choisissez la couleur en fonction du type de message
    QString color;
    switch (type) {
    case QtDebugMsg:
        color = "black";
        break;
    case QtWarningMsg:
        color = "orange";
        break;
    case QtCriticalMsg:
        color = "red";
        break;
    case QtFatalMsg:
        color = "red";
        break;
    case QtInfoMsg:
        color = "green";
        break;
    default:
        color = "black";
        break;
    }

    g_mainWindow->ui->console->append(QString("<span style='color: %1'>%2</span>").arg(color, msg));
}



void MainWindow::init()
{
    ui->setupUi(this);
    ui->Table->setRowCount(23);
    ui->Table->setColumnCount(0);
    ui->Table->setColumnWidth(0,143);

    QTableWidgetItem *pCell = ui->Table->item(0, 0);
    pCell = new QTableWidgetItem;
    ui->Table->setItem(0, 0, pCell);

    for (const QHostAddress &address : QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
        {
            ui->label->setText(address.toString());
        }

    }

    if (m_LLFTranslator->isConnected()){
        ui->connectButton->setText("Disconnect");
        ui->ledlabel->setState(Green);
    }

    g_mainWindow = this;

    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::onConnectButtonClicked);

    qInstallMessageHandler(DebugOutput);
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    init();
    Data = new GlobalData(ui);
}

MainWindow::MainWindow(LLFTranslator *translator, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) 
{
    m_LLFTranslator = translator;
    init();
    Data = new GlobalData(ui);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString itemname = item->text();

    double* ptr = Data->getPointers()[itemname];
    if (ptr == nullptr) return;
    
    graphicalview *GraphicalView = new graphicalview(this,itemname,ptr);
    GraphicalView->show();
}

void MainWindow::onConnectButtonClicked()
{
    bool connected = m_LLFTranslator->isConnected();
    bool success = connected ? m_LLFTranslator->disconnect() : m_LLFTranslator->connect();
    if (success) {
        ui->connectButton->setText(connected ? "Connect" : "Disconnect");
        ui->ledlabel->setState(connected ? Red : Green);
    }
}