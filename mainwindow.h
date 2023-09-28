#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "blockmodel.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonCollarCentroids_clicked();
    void on_pushButtonInitialize_clicked();
    void on_pushButtonBlockData_clicked();
    void on_pushButtonRockInfo_clicked();

private:
    Ui::MainWindow *ui;
    BlockModel *blockmodel;
};
#endif // MAINWINDOW_H
