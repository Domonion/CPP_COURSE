#ifndef TRIGRAM_H
#define TRIGRAM_H

#include <QString>
#include <QSet>
#include <QtCore/QVector>

struct Trigram {
    static const int TRIGRAMS = 20000;
    QString path;
    QVector<quint32> trigrams;
    mutable bool sorted;

    bool isText();

    bool isIn(Trigram const &other);

    void fromFile(QString const &str);

    void fromString(QString const &s);

};

#endif // TRIGRAM_H
