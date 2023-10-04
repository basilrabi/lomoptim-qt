#include "blockmodel.h"
#include <QFile>
#include <QMessageBox>

MiningArea::MiningArea(const double& x_coordinates,
                       const double& y_coordinates,
                       const double& z_coordinates,
                       const unsigned long& id)
{
    this->id = id;
    this->x = x_coordinates;
    this->y = y_coordinates;
    this->z = z_coordinates;
}

MiningArea::~MiningArea(){}

point_value MiningArea::asPointValue() const
{
    return std::make_pair(this->asPoint2D(), this->getId());
}

point_2d MiningArea::asPoint2D() const
{
    return point_2d(this->x, this->y);
}

unsigned char MiningArea::blockCount() const
{
    return this->blocks.size();
}

unsigned long MiningArea::getId() const
{
    return this->id;
}

BlockModel::BlockModel(const QString& blocks,
                       const QString& centroids,
                       const QString& rocks,
                       const double& x,
                       const double& y,
                       const double& z)
{
    this->size_x = x;
    this->size_y = y;
    this->size_z = z;

    unsigned char valid_centroid = 4;

    QFile centroid_csv(centroids);
    if (!centroid_csv.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Error: Could not open the file";
    }
    else
    {
        QString line;
        QStringList csv_entry;
        QTextStream in_centroid(&centroid_csv);
        line = in_centroid.readLine();
        csv_entry = line.split(",");
        QString collar_guide = "Acceptable Collar CSV should only contain the following headers in order:\nid, x, y, z";
        if (csv_entry.size() != 4)
        {
            QMessageBox::information(NULL, "Message", collar_guide);
        }
        else
        {
            if (csv_entry.at(0) != QString("id"))
            {
                collar_guide += QString("\n1st header should be 'id'");
                QMessageBox::information(NULL, "Message", collar_guide);
            }
            else
            {
                valid_centroid--;
            }
            if (csv_entry.at(1) != QString("x"))
            {
                QMessageBox::information(NULL, "Message", QString("2nd header shoud be 'x'"));
            }
            else
            {
                valid_centroid--;
            }
            if (csv_entry.at(2) != QString("y"))
            {
                QMessageBox::information(NULL, "Message", QString("2nd header shoud be 'y'"));
            }
            else
            {
                valid_centroid--;
            }
            if (csv_entry.at(3) != QString("z"))
            {
                QMessageBox::information(NULL, "Message", QString("2nd header shoud be 'z'"));
            }
            else
            {
                valid_centroid--;
            }
            if (valid_centroid == 0)
            {
                size_t idx = 0;
                while (!in_centroid.atEnd()) {
                    line = in_centroid.readLine();
                    csv_entry = line.split(",");
                    this->mining_areas.push_back(std::make_shared<MiningArea>(csv_entry.at(1).toDouble(),
                                                                              csv_entry.at(2).toDouble(),
                                                                              csv_entry.at(3).toDouble(),
                                                                              idx));
                    this->insertIndex(this->mining_areas[idx]->asPointValue());
                    idx++;
                }
            }
        }
    }
    centroid_csv.close();
    if (valid_centroid == 0)
    {
        this->initialized = 1;
    }
}

BlockModel::~BlockModel(){}

bool BlockModel::isInitialized() const
{
    return this->initialized;
}

unsigned long BlockModel::areaCount() const
{
    return this->mining_areas.size();
}

unsigned long long BlockModel::blockCount() const
{
    unsigned long long block_count = 0;
    for (auto& mining_area : this->mining_areas)
    {
        block_count += mining_area->blockCount();
    }
    return block_count;
}

void BlockModel::insertIndex(const point_value& pv)
{
    this->index.insert(pv);
}
