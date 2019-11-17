#include "worker.h"

#include <QCryptographicHash>
#include <QDir>
#include <QDirIterator>
#include <QMap>
#include <QVector>
#include <QException>
#include <QThread>
#include <QDebug>

Worker::Worker(QObject *parent) : QObject(parent) {

}

void Worker::check() {
    if (QThread::currentThread()->isInterruptionRequested()) {
        emit searchEnded();
        throw QException();
    }
}

void Worker::search(QString dir) {
    QDirIterator it(dir, QDir::NoSymLinks | QDir::Hidden | QDir::Files, QDirIterator::Subdirectories);
    QMap<qint64, QVector<QFileInfo>> sameSize;
    int cnt = 0;
    while (it.hasNext()) {
        check();
        QFileInfo info(it.next());
        sameSize[info.size()].push_back(info);
        cnt++;
    }
    emit searchStarted(cnt);
    for (auto sameSizeFiles : sameSize) {
        QCryptographicHash hasher(QCryptographicHash::Md5);
        QMap<QByteArray, QVector<QFileInfo>> duplicates;
        for (auto file : sameSizeFiles) {
            check();
            QFile f(file.filePath());
            if (f.open(QIODevice::ReadOnly)) {
                hasher.reset();
                hasher.addData(&f);
                duplicates[hasher.result()].push_back(file);
            }
            emit progress();
        }
        for (auto dups : duplicates) {
            check();
            if (dups.size() > 1) {
                QVector<QString> files;
                for (auto f : dups) {
                    check();
                    files.push_back(QDir(dir).relativeFilePath(f.filePath()));
                }
                emit duplicatesFound(files);
            }
        }
    }
    emit searchEnded();
}

void Worker::doSearch(QString dir) {
    try {
        search(dir);
    }
    catch (...) {
        return;
    }
}
