#include "window.h"
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QByteArray>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFont>
#include <QDateTime>
#include <QFontDatabase>

AddressWindow::AddressWindow(QString _address, QWidget *parent) :
    QWidget(parent),
    address(_address)
{
    this->setMinimumHeight(400);
    this->setMinimumWidth(525);
    this->setMaximumHeight(400);
    this->setMaximumWidth(525);
    setAttribute(Qt::WA_DeleteOnClose);
    refreshData();
    addressLabel = new QLabel(this);
    addressLabel -> setText(QString("address: ") + address + QString("\n") +
                            QString("balance: "));
    addressLabel -> setGeometry(10, 10, 500, 50);
    addressLabel ->setStyleSheet("QLabel { color : black; }");
    txsLabel = new QLabel(this);
    txsLabel -> setGeometry(0, 0, 484, 1000);
    txsLabel -> setText("Here will be placed transactions\n\n");
    txsLabel -> setAlignment(Qt::AlignTop | Qt::AlignLeft);
    txsLabel -> setStyleSheet("QLabel { background-color : red; color : blue; }");
    txsLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    txsLabel ->setFont(fixedFont);
    addressLabel -> setFont(fixedFont);
    scroll = new QScrollArea(this);
    scroll ->setGeometry(10, 60, 500, 300);
    scroll->setWidget(txsLabel);
}

void AddressWindow::refreshData(){
    ether = wei = "0";
    QString urlBalance = QString( "http://api.etherscan.io/api?module=account&action=balance&address=" +
                                  address + "&tag=latest&apikey=" + APIkey);
    QString urlTx = QString("http://api.etherscan.io/api?module=account&action=txlist&address=" +
                            address + "&startblock=0&endblock=99999999&sort=asc&apikey=" + APIkey);
    replyBalance = internet.get(QNetworkRequest(QUrl(urlBalance)));
    replyTxList = internet.get(QNetworkRequest(QUrl(urlTx)));
    QObject::connect(replyBalance, SIGNAL(finished()), this, SLOT(refreshBalance()));
    QObject::connect(replyTxList, SIGNAL(finished()), this, SLOT(refreshTxList()));
}
//0x1111111111111111111111111111111111111111
void AddressWindow::refreshBalance(){
    QByteArray buffer = replyBalance->readAll();
    replyBalance -> deleteLater();
    replyBalance = 0;
    qDebug() << buffer.size();
    QJsonDocument doc = QJsonDocument::fromJson(buffer);
    qDebug() << doc.isNull();
    QJsonObject obj = doc.object();
    qDebug() << obj.empty();
    QJsonValue val = obj.value(QString("result"));
    qDebug() << val.toString();
    QString str = val.toString();
    wei = str.right(18);
    ether = str.left(str.size() - 18);
    if(wei.size() == 0)
        wei = "0";
    if(ether.size() == 0)
        ether = "0";
    addressLabel -> setText(QString("address: ") + address + QString("\n") +
                            QString("balance: ") + ether + QString(".") + wei + QString("\n") + QString("Transactions window"));
}

QString weiToEth(QString str){
    while(str.size() <= 18){
        str.push_front("0");
    }
    str.insert(str.size() - 18, ".");
    int i = str.size() - 1;
    while(str[i] == "0")
        i--;
    str.chop(str.size() - i - 1);
    if(str[str.size() - 1] == ".")
        str.push_back("0");
    return str;
}

QString getStatus(QString code){
    if(code == "0"){
        return "Completed";
    } else{
        return "Error";
    }
}

void AddressWindow::refreshTxList(){
    QByteArray buffer = replyTxList->readAll();
    replyTxList -> deleteLater();
    replyTxList = 0;
    qDebug() << buffer.size();
    QJsonDocument doc = QJsonDocument::fromJson(buffer);
    QJsonObject obj = doc.object();
    QJsonValue val = obj.value(QString("result"));
    arr = val.toArray();
    qDebug() << arr.size();
    QString str;
    QJsonObject elem;
    for(int i = arr.size() -1; i >= 0; i--){
        elem = arr.at(i).toObject();
        str = str +
                + "Date      :" + QDateTime::fromSecsSinceEpoch(elem.value("timeStamp").toString().toLongLong()).toString(Qt::SystemLocaleShortDate) +QString("\n") +
                + "From      :" + elem.value("from").toString() + QString("\n") +
                + "To        :" + elem.value("to").toString() + QString("\n") +
                + "Value     :" + weiToEth(elem.value("value").toString()) + " eth" + QString("\n")+
                + "Status    :" + getStatus(elem.value("isError").toString()) + QString("\n\n");
    }
    txsLabel -> setText(str);
    txsLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    txsLabel -> setGeometry(0, 0, 484, 1000);
    txsLabel ->adjustSize();
}
