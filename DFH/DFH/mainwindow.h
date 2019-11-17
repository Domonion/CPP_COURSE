#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QTime>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

signals:

    void startSearch(QString const dir);

public slots:

    void searchStarted(int files_cnt);

    void addProgress();

    void getDuplicates(QVector<QString> files);

    void searchEnded();

    void cancelButtonClicked();

    void startButtonClicked();

    void deleteButtonClicked();

    void browseButtonClicked();

private:

    void showTime();

    Ui::MainWindow *ui;
    QThread thread;
    QString dir;
    QTime timer;
};

#endif // MAINWINDOW_H
