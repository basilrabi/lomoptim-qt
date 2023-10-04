#include "blockmodel.h"
#include <QFile>
#include <QMessageBox>

MiningArea::MiningArea(const double& x_coordinates,
                       const double& y_coordinates,
                       const double& z_coordinates,
                       const size_t& id,
                       QTextBrowser *log)
{
    this->id = id;
    this->log = log;
    this->x = x_coordinates;
    this->y = y_coordinates;
    this->z = z_coordinates;
}

MiningArea::~MiningArea(){}

box_2d MiningArea::asBox2D(const double& size_x, const double& size_y) const
{
    box_2d area(point_2d(this->x - size_x - (size_x / 2), this->y - size_y - (size_y / 2)),
                point_2d(this->x + size_x + (size_x / 2), this->y + size_y + (size_y / 2)));
    return area;
}

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

void MiningArea::setAdjacentArea(std::vector<std::shared_ptr<MiningArea>> adjacent_areas)
{
    this->adjacent_areas = adjacent_areas;
}

BlockModel::BlockModel(const QString& blocks,
                       const QString& centroids,
                       const QString& rocks,
                       const double& x,
                       const double& y,
                       const double& z,
                       QTextBrowser *log)
{
    this->log = log;
    this->size_x = x;
    this->size_y = y;
    this->size_z = z;

    unsigned char valid_centroid = 4;

    QFile centroid_csv(centroids);
    if (!centroid_csv.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        log->append(QString("Error: Could not open centroid file."));
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
                while (!in_centroid.atEnd())
                {
                    line = in_centroid.readLine();
                    csv_entry = line.split(",");
                    this->mining_areas.push_back(std::make_shared<MiningArea>(csv_entry.at(1).toDouble(),
                                                                              csv_entry.at(2).toDouble(),
                                                                              csv_entry.at(3).toDouble(),
                                                                              idx,
                                                                              log));
                    this->insertIndex(this->mining_areas[idx]->asPointValue());
                    idx++;
                }
            }
        }
    }
    centroid_csv.close();

    if (valid_centroid == 0)
    {
        this->populateAdjacentMiningArea();
        this->initialized = 1;
    }
}

BlockModel::~BlockModel(){}

bool BlockModel::isInitialized() const
{
    return this->initialized;
}

std::vector<std::shared_ptr<MiningArea>> BlockModel::getAdjacentAreas(std::shared_ptr<MiningArea> mining_area)
{
    std::vector<std::shared_ptr<MiningArea>> area_list;
    std::vector<point_value> adjacent_values;
    this->index.query(boost::geometry::index::intersects(mining_area->asBox2D(this->size_x, this->size_y)), std::back_inserter(adjacent_values));
    for (auto& adjacent_value : adjacent_values)
    {
        if (mining_area->getId() != std::get<1>(adjacent_value))
        {
            area_list.push_back(this->mining_areas[std::get<1>(adjacent_value)]);
        }
    }
    return area_list;
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

void BlockModel::populateAdjacentMiningArea()
{
    for (auto& mining_area : this->mining_areas)
    {
        mining_area->setAdjacentArea(this->getAdjacentAreas(mining_area));
    }
}
