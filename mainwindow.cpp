#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sizeX->setValue(10);
    ui->sizeY->setValue(10);
    ui->sizeZ->setValue(3);
    ui->log->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_clearButton_clicked()
{
    ui->log->clear();
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
    if (this->ui->fileBlock->text().isEmpty())
    {
        QMessageBox::information(this, "Message", QString("No selected block model file."));
        return;
    }
    if (this->ui->fileCollar->text().isEmpty())
    {
        QMessageBox::information(this, "Message", QString("No selected collar data file."));
        return;
    }
    if (this->ui->fileRock->text().isEmpty())
    {
        QMessageBox::information(this, "Message", QString("No selected rock data file."));
        return;
    }
    if (this->ui->sizeX->text().toDouble() < 0.01)
    {
        QMessageBox::information(this, "Message", QString("Size along X-axis should be greater than 0.00."));
        return;
    }
    if (this->ui->sizeY->text().toDouble() < 0.01)
    {
        QMessageBox::information(this, "Message", QString("Size along Y-axis should be greater than 0.00."));
        return;
    }
    if (this->ui->sizeZ->text().toDouble() < 0.01)
    {
        QMessageBox::information(this, "Message", QString("Size along Z-axis should be greater than 0.00."));
        return;
    }
    this->blockmodel = std::make_shared<BlockModel>(this->ui->fileBlock->text(),
                                                    this->ui->fileCollar->text(),
                                                    this->ui->fileRock->text(),
                                                    this->ui->sizeX->text().toDouble(),
                                                    this->ui->sizeY->text().toDouble(),
                                                    this->ui->sizeZ->text().toDouble(),
                                                    this->ui->log);
    if (this->blockmodel->isInitialized())
    {
        this->ui->log->append("Block Model initialized.");
        if (this->blockmodel->areaCount() > 0)
        {
            this->ui->log->append(
                        QString("Area Count: ")
                        + QString::number(this->blockmodel->areaCount())
                        + QString(". Block Count: ")
                        + QString::number(this->blockmodel->blockCount())
                        );
        }
    }
    else
    {
        this->ui->log->append("Block Model failed to initialized.");
    }
}
