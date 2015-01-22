#include "client.h"
#include "ui_client.h"

client::client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);
    ui->portNumber->setValidator(new QIntValidator(1024,65535,this));
}


client::~client()
{
    delete ui;
}

void client::on_connectButton_clicked()
{
    if(ui->portNumber->hasAcceptableInput()){
        ui->portLabelInvalid->setText("");
        ui->sendButton->setEnabled(true);
    }
    else
        ui->portLabelInvalid->setText("INVALID PORT!");


}
