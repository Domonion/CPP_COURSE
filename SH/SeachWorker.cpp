#include "SearchWorker.h"
#include <QThread>
#include <QException>
#include <QFile>
#include <QVector>
#include <QTextStream>
#include <string>
#include <algorithm>
#include <search.h>
#include <QDebug>
#include <experimental/algorithm>
#include <experimental/functional>
#include <iostream>
#include <functional>

SearchWorker::SearchWorker(QObject *parent) : QObject(parent) {

}

void SearchWorker::check() {
    if (QThread::currentThread()->isInterruptionRequested()) {
        emit searchFinished();
        throw QException();
    }
}

void SearchWorker::search(QString s, QVector<Trigram> data) {
    emit searchStarted(data.size());
    Trigram tr;
    tr.fromString(s);
    for (int i = 0; i < data.size(); i++) {
        check();
        if (tr.isIn(data[i])) {
            QFile file(data[i].path);
            if (!file.open(QFile::ReadOnly)) {
                emit progress();
                continue;
            }
            QVector<QPair<int, int> > ans;
            QTextStream in(&file);
            in.setCodec("UTF-8");
            QString line;
            int x = 0;
            while ((line = in.readLine()).size() > 0) {
                check();
                x++;
                std::string what = s.toStdString();
                std::string where = line.toStdString();
                auto it = std::search(where.begin(), where.end(), std::boyer_moore_searcher(what.begin(), what.end()));
                while (it != where.end()) {
                    check();
                    int y = it - where.begin();
                    it = std::search(it + 1, where.end(), std::boyer_moore_searcher(what.begin(), what.end()));
                    ans.push_back(QPair<int, int>(x, y));
                }
            }
            if(ans.size() > 0)
              emit getOccurencies(data[i].path, ans);
        }
        emit progress();
    }
    emit searchFinished();
}

void SearchWorker::doSearch(QString s, QVector<Trigram> data) {
    try {
        search(s, data);
    }
    catch (...) {
        return;
    }
}
