#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>

extern "C" {
#include "../include/sock_help.h"
#include "../include/poorIRC_proto.h"
}


namespace Ui {
class client;
}

class client : public QMainWindow
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = 0);
    int socket;
    char *nick;
    struct poorIRC_message msg;
    struct poorIRC_response res;
    ~client();

private slots:
    void on_connectButton_clicked();

    void on_sendButton_clicked();

private:
    Ui::client *ui;
};

#endif // CLIENT_H
