#ifndef SEARCHWORKER_H
#define SEARCHWORKER_H

#include <Trigram.h>
#include <QFileSystemWatcher>
#include <QObject>

class SearchWorker : public QObject {
Q_OBJECT

public:
    explicit SearchWorker(QObject *parent = nullptr);

private:
    void check();

    void search(QString s, QVector<Trigram> data);

signals:

    void searchStarted(int files_cnt);

    void progress();

    void getOccurencies(QString file, QVector<QPair<int, int>> places);

    void searchFinished();

public slots:

    void doSearch(QString s, QVector<Trigram> data);
};

#endif // SEARCHWORKER_H
