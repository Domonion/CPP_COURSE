#include "mymainwindow.h"
#include "window.h"

MyMainWindow::MyMainWindow(QWidget *parent) : QMainWindow(parent)
{
}

void MyMainWindow::createAddressWindow(QString address){
    AddressWindow * ptr = new AddressWindow(address);
    ptr->show();
}
