#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

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

void MainWindow::on_pushButtonInitialize_clicked()
{
    QString block = this->ui->fileBlock->text();
    QString collar = this->ui->fileCollar->text();
    QString rock = this->ui->fileRock->text();

    if (block.isEmpty())
    {
        QMessageBox::information(this, "Message", QString("No selected block model file."));
        return;
    }
    if (collar.isEmpty())
    {
        QMessageBox::information(this, "Message", QString("No selected collar data file."));
        return;
    }
    if (rock.isEmpty())
    {
        QMessageBox::information(this, "Message", QString("No selected rock data file."));
        return;
    }

    QMessageBox::information(this, "Message", QString(block + "\n" + collar + "\n" + rock));
}

