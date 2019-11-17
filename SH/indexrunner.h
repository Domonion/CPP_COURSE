#ifndef INDEXRUNNER_H
#define INDEXRUNNER_H

#include <QObject>
#include <QRunnable>
#include <QVector>
#include <QString>
#include <QThread>
#include <Trigram.h>

class IndexRunner : public QObject {
Q_OBJECT

private:
    QVector<QString> qsl;
    int l, r;

public:
    IndexRunner(QVector<QString> _qsl, int _l, int _r);

signals:

    void progress();

    void getTrigrams(Trigram);

public slots:
    void run();
};

#endif // INDEXRUNNER_H
