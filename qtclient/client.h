#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>



namespace Ui {
class client;
}

class client : public QMainWindow
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = 0);
    int socket;
    ~client();

private slots:
    void on_connectButton_clicked();

private:
    Ui::client *ui;
};

#endif // CLIENT_H
