#ifndef ADDRESSCHECKER_H
#define ADDRESSCHECKER_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QRegularExpression>
#include <QStringBuilder>

class AddressChecker : public QObject
{
    Q_OBJECT
public:
    explicit AddressChecker(QLineEdit * line, QObject * parent = nullptr);
private:
    QLineEdit * line;
    QRegularExpression hexMatcher;
signals:
    void wrongAddress();
    void rightAddress(QString);
public slots:
    void checkAddress();
};

#endif // ADDRESSCHECKER_H
