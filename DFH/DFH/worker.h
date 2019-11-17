#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class Worker : public QObject {
Q_OBJECT

private:
    void check();

    void search(QString dir);

public:
    explicit Worker(QObject *parent = nullptr);

signals:

    void searchStarted(int files_cnt);

    void progress();

    void duplicatesFound(QVector<QString> files);

    void searchEnded();

public slots:

    void doSearch(QString dir);
};

#endif // WORKER_H
