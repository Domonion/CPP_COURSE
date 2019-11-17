#include "Trigram.h"

#include <QFile>
#include <QDataStream>
#include <QtCore/QThread>

bool Trigram::isText() {
    return trigrams.size() <= TRIGRAMS;
}

bool Trigram::isIn(Trigram const &other) {
    for (auto i : trigrams) {
        if (qBinaryFind(other.trigrams.begin(), other.trigrams.end(), i) == other.trigrams.end()) {
            return false;
        }
    }
    return true;
}

void Trigram::fromFile(QString const &str) {
    trigrams.clear();

    sorted = false;
    path = str;
    QFile file(str);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    QDataStream in(&file);
    char buffer[1024 * 4];
    int len = 1024 * 4;
    int nowlen;
    quint32 nowtrigram = 0;
    while ((nowlen = in.readRawData(buffer, len)) > 0) {
        for (int i = 0; i < nowlen; i++) {
            if (QThread::currentThread()->isInterruptionRequested()) {
                break;
            }
            nowtrigram <<= 16;
            nowtrigram >>= 8;
            nowtrigram += static_cast<quint8>(buffer[i]);
            trigrams.push_back(nowtrigram);
        }
        if (trigrams.size() > TRIGRAMS) {
            trigrams.clear();
            return;
        }
    }
    if(!sorted){
        qSort(trigrams);
        sorted = true;
    }
}

void Trigram::fromString(const QString &s) {
    trigrams.clear();
    quint32 nowtrigram = 0;
    auto arr = s.toUtf8();
    if (arr.size() < 3)
        return;
    nowtrigram += static_cast<quint8>(arr[0]);
    nowtrigram <<= 8;
    nowtrigram += static_cast<quint8>(arr[1]);
    for (int i = 2; i < arr.size(); i++) {
        nowtrigram <<= 16;
        nowtrigram >>= 8;
        nowtrigram += static_cast<quint8>(arr[i]);
        trigrams.push_back(nowtrigram);
        if (trigrams.size() > TRIGRAMS) {
            trigrams.clear();
            return;
        }
    }
    if(!sorted){
        qSort(trigrams);
        sorted = true;
    }
}
