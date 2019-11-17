#ifndef TRIGRAMWORKER_H
#define TRIGRAMWORKER_H

#include <QObject>
#include <QVector>
#include <Trigram.h>
#include <MainWindow.h>
#include <QThread>

class IndexWorker : public QObject {
Q_OBJECT
private:
    bool check();

    MainWindow * ptr;
    int threadsCnt;
    int amount;
    int remain;
    int deleted;
    QVector<QThread*> threads;

public:
    explicit IndexWorker(QObject *parent = nullptr);

public slots:

    void doIndex(QString dir);
signals:

    void indexStarted(int files_cnt);

    void indexFinished();

    void startThreads();
};

#endif // TRIGRAMWORKER_H
