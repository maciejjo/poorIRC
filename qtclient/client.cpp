#include "client.h"
#include "ui_client.h"
client::client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::client)
{
    socket = -1;
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
    if(socket != -1)
        closesocket(socket);
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
        ui->portLabelInvalid->setText("Invalid data!");

    char flag = SOCKET_CONN;

    //just to be readible
    char *host = ui->ipEdit->text().toLocal8Bit().data();
    char *port = ui->portNumber->text().toLocal8Bit().data();



    if(-1 == (socket = get_tcp_socket(port, host, flag)))
        return;


    printf("Socket obtained!: %d\n", socket);
}

void client::on_sendButton_clicked()
{  

    msg.len = ui->sendMessage->text().length() + 1;
    if(0 == msg.len-1)
        return; // you have to actually send something
    else if (127 <= msg.len){
        return; // too much
    }
    else{
        if (-1 == send(socket, (char *)&(msg.len), sizeof(msg.len), 0)){
            return; //error with sending
        }
        strncpy(msg.body,ui->sendMessage->text().toLocal8Bit().data(), msg.len);
    }

    if(-1 == (send(socket, (char *)&(msg.body), msg.len, 0))) {
        return; //error with sending actual message
    }

    if(-1 == (recv(socket, (char *)&res, sizeof(res), 0))) {
        return;
    }

    if(res.status == POORIRC_STATUS_OK) {
                printf("Status OK received\n");

    }

    ui->sendMessage->clear();

}
