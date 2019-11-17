#include "addresschecker.h"

AddressChecker::AddressChecker(QLineEdit * _line, QObject *parent) :
    QObject(parent),
    line(_line),
    hexMatcher("^0x[0-9A-Z]{40}$", QRegularExpression::CaseInsensitiveOption)
{
}

void AddressChecker::checkAddress(){
    QString address = line -> text();
    if(address.size() != 42){
        emit wrongAddress();
    }
    else {
        if (hexMatcher.match(address).hasMatch()){
            emit rightAddress(address);
        } else{
            emit wrongAddress();
        }
    }

}
