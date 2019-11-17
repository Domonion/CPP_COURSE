#include "indexrunner.h"
#include <Trigram.h>
#include <QDebug>

IndexRunner::IndexRunner(QVector<QString> _qsl, int _l, int _r) : qsl(_qsl), l(_l), r(_r) {

}

void IndexRunner::run() {
    for (int i = l; i < r; i++) {
        if (QThread::currentThread()->isInterruptionRequested()) {
            break;
        }
        Trigram tr;
        tr.fromFile(qsl[i]);
        if (QThread::currentThread()->isInterruptionRequested()) {
            break;
        }
        if (tr.isText()) {
            emit getTrigrams(tr);
        }
        emit progress();
    }
    QThread::currentThread()->quit();
}
