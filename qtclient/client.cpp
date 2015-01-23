#include "client.h"
#include "ui_client.h"
extern "C" {
#include "../include/sock_help.h"
}
client::client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);
    ui->portNumber->setValidator(new QIntValidator(1024,65535,this));
    //IPv4 validation
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->ipEdit->setValidator(ipValidator);
}


client::~client()
{
    delete ui;
}

void client::on_connectButton_clicked()
{
    if(ui->portNumber->hasAcceptableInput()
            && ui->ipEdit->hasAcceptableInput()){
        ui->portLabelInvalid->setText("");
        ui->sendButton->setEnabled(true);
        ui->sendMessage->setEnabled(true);
        ui->connectButton->setText("CONNECTED!");
        ui->connectButton->setEnabled(false);
        ui->portNumber->setEnabled(false);
        ui->ipEdit->setEnabled(false);
    }
    else
        ui->portLabelInvalid->setText("INVALID PORT!");

    char flag = SOCKET_CONN;
    //just to be readible
    char *host = ui->ipEdit->text().toLocal8Bit().data();
    char *port = ui->portNumber->text().toLocal8Bit().data();
    if(-1 == (socket = get_tcp_socket(port, host, flag)))
        return;


    printf("Socket obtained!: %d\n", socket);
}
