#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QStringBuilder>

class MyMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MyMainWindow(QWidget *parent = nullptr);
signals:

public slots:
    void createAddressWindow(QString);
};

#endif // MYMAINWINDOW_H
