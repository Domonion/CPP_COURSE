#include "addresschecker.h"
#include "mymainwindow.h"
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QtDebug>
#include <QSpinBox>
#include <QDial>
#include <QDir>
#include <QScrollBar>
#include <QImage>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(QDir::currentPath() + QString("/../DEBE/trayIcon.jpg")));


    MyMainWindow * window = new MyMainWindow;
    window -> setMinimumHeight(600);
    window -> setMinimumWidth(1000);
    window -> setMaximumHeight(600);
    window -> setMaximumWidth(1000);

    QLabel * progIcon = new QLabel(window);
    progIcon -> setPixmap(QPixmap(QDir::currentPath() + QString("/../DEBE/mainIcon.jpg")));
    progIcon -> setScaledContents(true);
    progIcon -> setGeometry(1, 1, 1000, 500);

    QLineEdit * line = new QLineEdit(window);
    line -> setGeometry(10, 510,500, 50);
    line -> setPlaceholderText("Place here address");

    QPushButton * button = new QPushButton(window);
    button -> setGeometry(550, 510, 400, 50);
    button -> setText("OK");

    AddressChecker * addressChecker = new AddressChecker(line);

    QLabel * error = new QLabel(window);
    error -> setText("Wrong addresss");
    error -> setGeometry(10, 550, 125, 50);
    error -> hide();

    QObject::connect(button, SIGNAL(released()), addressChecker, SLOT(checkAddress()));
    QObject::connect(addressChecker, SIGNAL(wrongAddress()), error, SLOT(show()));
    QObject::connect(addressChecker, SIGNAL(rightAddress(QString)), error, SLOT(hide()));
    QObject::connect(addressChecker, SIGNAL(rightAddress(QString)), window, SLOT(createAddressWindow(QString)));

    window -> show();

    return a.exec();
}
