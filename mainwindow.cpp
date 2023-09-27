#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonBlockData_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", QString(), "CSV Files (*.csv)");
    ui->fileBlock->setText(filePath);
}

void MainWindow::on_pushButtonCollarCentroids_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", QString(), "CSV Files (*.csv)");
    ui->fileCollar->setText(filePath);
}

void MainWindow::on_pushButtonRockInfo_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", QString(), "CSV Files (*.csv)");
    ui->fileRock->setText(filePath);
}

