#include "client.h"
#include "ui_client.h"
client::client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::client)
{
    socket = -1;
    ui->setupUi(this);
    ui->portNumber->setValidator(new QIntValidator(1024,65535,this));

    ui->nickEdit->setMaxLength(POORIRC_NICKNAME_MAX_LEN);
    ui->sendMessage->setMaxLength(POORIRC_MSG_MAX_LEN-1);

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
#ifdef _WIN32
        closesocket(socket);
#elif __linux__
        close(socket);
#endif
    delete ui;
}

void client::on_connectButton_clicked()
{
    //Validate nick
    if(ui->nickEdit->text().length() == 0){
        ui->connectErrorLabel->setText("Set nick!");
        return;
    }

    //Validate port and host
    if(!ui->portNumber->hasAcceptableInput()
            && !ui->ipEdit->hasAcceptableInput()){
        ui->connectErrorLabel->setText("Invalid port!");
        return;
    }
    //just to be readible
    char *host = ui->ipEdit->text().toLocal8Bit().data();
    char *port = ui->portNumber->text().toLocal8Bit().data();

    nick = ui->nickEdit->text().toLocal8Bit().data();
    strcat(nick, "\0");

    if(-1 == (socket = get_tcp_socket(port, host, SOCKET_CONN))){
        ui->connectErrorLabel->setText("Wrong IP");
        return;
    }

    char *set_nick = (char *)calloc(7+POORIRC_MSG_MAX_LEN, sizeof(char));
    strcat(set_nick, "/nick ");
    strcat(set_nick, nick);

    if(-1 == poor_send(socket, set_nick)){
        free(set_nick);
        return;
    }
    else {
        ui->connectErrorLabel->setText("");
        ui->sendButton->setEnabled(true);
        ui->sendMessage->setEnabled(true);
        ui->connectButton->setText("CONNECTED!");

        ui->nickEdit->setEnabled(false);
        ui->connectButton->setEnabled(false);
        ui->portNumber->setEnabled(false);
        ui->ipEdit->setEnabled(false);
    }
    free(set_nick);

    printf("Socket obtained!: %d\n", socket);

    //QSocketNotifier *SocketMonitor;
    //SocketMonitor = new QSocketNotifier(socket, QSocketNotifier::Read, parent);
    //QObject::connect(SocketMonitor,SIGNAL(activated(int)), client ,SLOT(dataReceived()) );

}

void client::on_sendButton_clicked()
{  
    char *message = ui->sendMessage->text().toLocal8Bit().data();
    strcat(message,"\0");

    if(0 == poor_send(socket, message))
        ui->sendMessage->clear();


}
