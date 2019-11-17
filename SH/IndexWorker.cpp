#include "IndexWorker.h"
#include <QThread>
#include <QException>
#include <QThreadPool>
#include <QDirIterator>
#include <indexrunner.h>
#include <MainWindow.h>
#include <QDir>
#include <algorithm>
#include <Trigram.h>
#include <QDebug>

bool IndexWorker::check() {
    return QThread::currentThread()->isInterruptionRequested();
}

void IndexWorker::doIndex(QString dir) {
    QDirIterator it(dir, QDir::NoSymLinks | QDir::Hidden | QDir::Files, QDirIterator::Subdirectories);
    QVector<QString> qsl;
    int cnt = 0;
    while (it.hasNext()) {
        if(check()) {
            emit indexFinished();
            return;
        }
        qsl.push_back(it.next());
        cnt++;
    }
    emit indexStarted(cnt);
    threadsCnt = std::max(1, std::min(cnt, QThread::idealThreadCount()));
    amount = cnt / threadsCnt;
    remain = cnt % threadsCnt;
    threads.resize(threadsCnt);
    for (int i = 0; i < threadsCnt; i++) {
        threads[i] = new QThread();
        auto ir = new IndexRunner(qsl, amount * i, amount * (i + 1) + (i == threadsCnt - 1 ? remain : 0));
        ir->moveToThread(threads[i]);
        connect(ir, &IndexRunner::progress, ptr, &MainWindow::addProgress);
        connect(ir, &IndexRunner::getTrigrams, ptr, &MainWindow::getTrigrams);
        connect(this, &IndexWorker::startThreads, ir, &IndexRunner::run);
        threads[i]->start();
    }
    emit startThreads();
    deleted = 0;
    for(int i = 0; i < threadsCnt; i++){
        while(!threads[i]->wait(50)){
            if(check()){
                for(int j = 0; j < threadsCnt; j++){
                    threads[j]->requestInterruption();
                    threads[j]->wait();
                    delete threads[j];
                }
                emit indexFinished();
                return;
            }
        }
    }
    qDebug() << "alpha thread finished";
    for(int j = 0; j < threadsCnt; j++){
        threads[j]->quit();
        threads[j]->wait();
        delete threads[j];
    }
    emit indexFinished();
}

IndexWorker::IndexWorker(QObject *parent) : QObject(nullptr) {
    ptr = qobject_cast<MainWindow*>(parent);
}
