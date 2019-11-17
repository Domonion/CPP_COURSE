#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QVector>
#include <QDebug>
#include <QPair>
#include <QFileSystemWatcher>
#include <QDebug>
#include <QThreadPool>
#include <IndexWorker.h>
#include <SearchWorker.h>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    qDebug() << QThread::idealThreadCount();
    qDebug() << QThreadPool::globalInstance()->maxThreadCount();
    qDebug() << QThreadPool::globalInstance()->activeThreadCount();
    ui->setupUi(this);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::startSearchButtonClicked);
    connect(ui->indexButton, &QPushButton::clicked, this, &MainWindow::startIndexButtonClicked);
    connect(ui->browseButton, &QPushButton::clicked, this, &MainWindow::browseButtonClicked);
    connect(ui->cancelIndexButton, &QPushButton::clicked, this, &MainWindow::cancelButtonsClicked);
    connect(ui->cancelSearchButton, &QPushButton::clicked, this, &MainWindow::cancelButtonsClicked);
    changed = 0;
    ui->number->setText(QString::number(changed));
    connect(&watcher, &QFileSystemWatcher::fileChanged, this, &MainWindow::dirChanged);
    qRegisterMetaType<Trigram>("Trigram");
    qRegisterMetaType<QVector<Trigram>>("QVector<Trigram>");
    qRegisterMetaType<QVector<QPair<int, int>>>("QVector<QPair<int,int>>");
}

MainWindow::~MainWindow() {
    thread.requestInterruption();
    thread.quit();
    thread.wait();
    delete ui;
}

void MainWindow::browseButtonClicked() {
    ui->dirEdit->setText(QFileDialog::getExistingDirectory());
}

void MainWindow::startSearchButtonClicked() {
    if (ui->stringEdit->text().trimmed().size() == 0) {
        ui->statusBar->showMessage(QString("Empty string"));
        return;
    }
    if (data.empty()) {
        ui->statusBar->showMessage(QString("Index not finished"));
        return;
    }
    if (!QDir(dir).exists()) {
        ui->statusBar->showMessage("Directory not found");
        return;
    }
    qDebug() << "button clicked";
    ui->searchWidget->clear();
    changeButtons(false);
    timer.start();
    auto *worker = new SearchWorker();
    worker->moveToThread(&thread);
    connect(this, &MainWindow::startSearch, worker, &SearchWorker::doSearch);
    connect(worker, &SearchWorker::searchStarted, this, &MainWindow::threadStarted);
    connect(worker, &SearchWorker::progress, this, &MainWindow::addProgress);
    connect(worker, &SearchWorker::getOccurencies, this, &MainWindow::getOccurencies);
    connect(worker, &SearchWorker::searchFinished, this, &MainWindow::threadFinished);
    connect(worker, &SearchWorker::searchFinished, worker, &QObject::deleteLater);
    thread.start();
    qDebug() << ui->stringEdit->text().trimmed();
    emit startSearch(ui->stringEdit->text().trimmed(), data);
}

void MainWindow::getOccurencies(QString file, QVector<QPair<int, int> > places) {
    auto *item = new QTreeWidgetItem(ui->searchWidget);
    ui->searchWidget->addTopLevelItem(item);
    item->setText(0, "Found " + QString::number(places.size()) + " occurencies in " + QDir(dir).relativeFilePath(file));
    for (auto i : places) {
        auto *child = new QTreeWidgetItem(item);
        child->setText(0, "Line: " + QString::number(i.first) + ", character: " + QString::number(i.second));
        item->addChild(child);
    }
}

void MainWindow::startIndexButtonClicked() {
    if (!QDir::isAbsolutePath(ui->dirEdit->text()) || !QDir(ui->dirEdit->text()).exists()) {
        ui->statusBar->showMessage(QString("Invalid path"));
        return;
    }
    dir = ui->dirEdit->text();
    ui->searchWidget->clear();
    if (!watcher.files().isEmpty()) watcher.removePaths(watcher.files());
    timer.start();
    changed = 0;
    ui->number->setText(QString::number(changed));
    changeButtons(false);
    auto *worker = new IndexWorker(this);
    worker->moveToThread(&thread);
    connect(this, &MainWindow::startIndex, worker, &IndexWorker::doIndex);
    connect(worker, &IndexWorker::indexStarted, this, &MainWindow::threadStarted);
    //connect(worker, &IndexWorker::progress, this, &MainWindow::addProgress);
    //connect(worker, &IndexWorker::getTrigrams, this, &MainWindow::getTrigrams);
    connect(worker, &IndexWorker::indexFinished, this, &MainWindow::threadFinished);
    connect(worker, &IndexWorker::indexFinished, worker, &QObject::deleteLater);
    data.clear();
    data.resize(0);
    data.squeeze();
    thread.start();
    emit startIndex(dir);
}

void MainWindow::getTrigrams(Trigram tr) {
    data.push_back(tr);
    watcher.addPath(tr.path);
}

void MainWindow::threadStarted(int files_cnt) {
    ui->progressBar->setRange(0, files_cnt);
    ui->progressBar->setValue(0);
}

void MainWindow::addProgress() {
    ui->progressBar->setValue(ui->progressBar->value() + 1);
}

void MainWindow::threadFinished() {
    ui->statusBar->showMessage("Time elapsed: " + QString::number(timer.elapsed() / 1000.0) + QString(" sec."));
    changeButtons(true);
    thread.quit();
    thread.wait();
}

void MainWindow::cancelButtonsClicked() {
    thread.requestInterruption();
}

void MainWindow::dirChanged() {
    changed++;
    ui->number->setText(QString::number(changed));
}

void MainWindow::changeButtons(bool state) {
    ui->searchButton->setEnabled(state);
    ui->indexButton->setEnabled(state);
}
