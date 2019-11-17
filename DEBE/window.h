#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QJsonObject>
#include <QNetworkReply>
#include <QProgressDialog>
#include <QLabel>
#include <QJsonArray>
#include <QScrollArea>
#include <QStringBuilder>

class AddressWindow : public QWidget
{
    Q_OBJECT
public:
    explicit AddressWindow(QString, QWidget *parent = nullptr);
private:
    QString address;
    const QString APIkey = QString("WT579N2EYHCKKE36BMA3282FUIY4STM96D");
    QString ether;
    QString wei;
    QJsonArray arr;

    QNetworkAccessManager internet;
    QNetworkReply * replyBalance;
    QNetworkReply * replyTxList;
    QLabel * addressLabel;
    QScrollArea * scroll;
    QLabel * txsLabel;
signals:

public slots:
    void refreshData();
    void refreshBalance();
    void refreshTxList();
};

#endif // WINDOW_H
