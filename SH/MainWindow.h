#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "Trigram.h"
#include <QVector>
#include <QFileSystemWatcher>
#include <QDir>
#include <QString>
#include <QTime>
#include <QMap>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

public slots:

    void browseButtonClicked();

    //search
    void startSearchButtonClicked();

    void getOccurencies(QString file, QVector<QPair<int, int> > places);

    //index
    void startIndexButtonClicked();

    void getTrigrams(Trigram);

    //both
    void threadStarted(int files_cnt);

    void addProgress();

    void threadFinished();

    void cancelButtonsClicked();

    void dirChanged();
signals:

    void startSearch(QString str, QVector<Trigram> data);

    void startIndex(QString dir);

private:
    void changeButtons(bool state);

    Ui::MainWindow *ui;
    QThread thread;
    QString dir;
    QVector<Trigram> data;
    QTime timer;
    QFileSystemWatcher watcher;
    int changed;
};

#endif // MAINWINDOW_H
