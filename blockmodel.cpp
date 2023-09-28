#include "blockmodel.h"
#include <QMessageBox>

BlockModel::BlockModel(QString blocks, QString centroids, QString rocks, double x, double y, double z)
{
    this->size_x = x;
    this->size_y = y;
    this->size_z = z;
    QMessageBox::information(NULL, "Message", QString(blocks + "\n" + centroids + "\n" + rocks));
}

BlockModel::~BlockModel(){}
