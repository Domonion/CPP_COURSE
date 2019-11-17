#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QDesktopWidget>
#include <worker.h>
#include <QDebug>
#include <QTreeWidgetItem>
#include <QFileDialog>
#include <QVector>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startButtonClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &MainWindow::cancelButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::deleteButtonClicked);
    connect(ui->browseButton, &QPushButton::clicked, this, &MainWindow::browseButtonClicked);
    qRegisterMetaType<QVector<QString>>("QVector<QString>");
}

MainWindow::~MainWindow() {
    thread.requestInterruption();
    thread.quit();
    thread.wait();
    delete ui;
}

void MainWindow::searchStarted(int files_cnt) {
    timer.start();
    ui->progressBar->setRange(0, files_cnt);
    ui->progressBar->setValue(0);
}

void MainWindow::addProgress() {
    ui->progressBar->setValue(ui->progressBar->value() + 1);
}

void MainWindow::getDuplicates(QVector<QString> files) {
    auto *item = new QTreeWidgetItem(ui->treeWidget);
    ui->treeWidget->addTopLevelItem(item);
    item->setText(0, QString::number(files.size()) + " files");
    for (auto i : files) {
        auto *child = new QTreeWidgetItem(item);
        child->setText(0, i);
        item->addChild(child);
    }
}

void MainWindow::searchEnded() {
    showTime();
    ui->startButton->setEnabled(true);
    thread.quit();
    thread.wait();
}

void MainWindow::startButtonClicked() {
    dir = ui->textEdit->toPlainText();
    if (!QDir::isAbsolutePath(dir) || !QDir(dir).exists()) {
        ui->statusBar->showMessage(QString("Invalid path"));
        return;
    }
    ui->startButton->setEnabled(false);
    ui->treeWidget->clear();
    auto *worker = new Worker();
    worker->moveToThread(&thread);
    connect(this, &MainWindow::startSearch, worker, &Worker::doSearch);
    connect(worker, &Worker::searchStarted, this, &MainWindow::searchStarted);
    connect(worker, &Worker::progress, this, &MainWindow::addProgress);
    connect(worker, &Worker::duplicatesFound, this, &MainWindow::getDuplicates);
    connect(worker, &Worker::searchEnded, this, &MainWindow::searchEnded);
    connect(worker, &Worker::searchEnded, worker, &QObject::deleteLater);
    thread.start();
    emit startSearch(dir);
}

void MainWindow::cancelButtonClicked() {
    thread.requestInterruption();
}

void MainWindow::deleteButtonClicked() {
    for (auto it : ui->treeWidget->selectedItems()) {
        if (QFile::remove(QDir(dir).absoluteFilePath(it->text(0))))
            delete it;
    }
}

void MainWindow::browseButtonClicked() {
    ui->textEdit->setText(QFileDialog::getExistingDirectory());
}

void MainWindow::showTime()
{
    ui->statusBar->showMessage(QString("Time elapsed: ") + QString::number(timer.elapsed() / 1000.0) + QString(" s"));
}
