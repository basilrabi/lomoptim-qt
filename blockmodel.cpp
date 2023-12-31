#include "blockmodel.h"
#include <QDateTime>
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

size_t MiningArea::blockCount() const
{
    return this->blocks.size();
}

size_t MiningArea::getId() const
{
    return this->id;
}

void MiningArea::appendLog(const QString& message)
{
    this->log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ") + message);
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

    QString guide;
    QString line;
    QStringList csv_entry;

    unsigned char valid_centroid = 4;

    QFile centroid_csv(centroids);
    if (!centroid_csv.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        this->appendLog("Error: Could not open centroid file.");
    }
    else
    {
        QTextStream in_centroid(&centroid_csv);
        line = in_centroid.readLine();
        csv_entry = line.split(",");
        guide = "Acceptable Collar CSV should only contain the following headers in order:\nid, x, y, z";
        if (csv_entry.size() != 4)
        {
            QMessageBox::information(NULL, "Message", guide);
        }
        else
        {
            if (csv_entry.at(0) != QString("id"))
            {
                guide += QString("\n1st header should be 'id'.");
                QMessageBox::information(NULL, "Message", guide);
            }
            else
            {
                valid_centroid--;
            }
            if (csv_entry.at(1) != QString("x"))
            {
                QMessageBox::information(NULL, "Message", QString("2nd header shoud be 'x'."));
            }
            else
            {
                valid_centroid--;
            }
            if (csv_entry.at(2) != QString("y"))
            {
                QMessageBox::information(NULL, "Message", QString("2nd header shoud be 'y'."));
            }
            else
            {
                valid_centroid--;
            }
            if (csv_entry.at(3) != QString("z"))
            {
                QMessageBox::information(NULL, "Message", QString("2nd header shoud be 'z'."));
            }
            else
            {
                valid_centroid--;
            }
            if (valid_centroid == 0)
            {
                long idx = -1;
                while (!in_centroid.atEnd())
                {
                    line = in_centroid.readLine();
                    csv_entry = line.split(",");
                    if (csv_entry.at(0).toLong() > (idx + 1))
                    {
                        if (csv_entry.at(0).toLong() - (idx + 1) > 1)
                        {
                            this->appendLog("Error: id column skipped at "
                                            + csv_entry.at(0)
                                            + ".");
                            valid_centroid++;
                            break;
                        }
                        idx = csv_entry.at(0).toLong() - 1;
                        this->mining_areas.push_back(std::make_shared<MiningArea>(csv_entry.at(1).toDouble(),
                                                                                  csv_entry.at(2).toDouble(),
                                                                                  csv_entry.at(3).toDouble(),
                                                                                  (size_t) idx,
                                                                                  log));
                        this->insertIndex(this->mining_areas[(size_t) idx]->asPointValue());
                    }
                    else if ((csv_entry.at(0).toLong() == (idx + 1)))
                    {
                        this->appendLog("Error: id column duplicated at "
                                        + csv_entry.at(0)
                                        + ".");
                        valid_centroid++;
                        break;
                    }
                    else
                    {
                        this->appendLog("Error: id column in collar csv is not sorted in ascending order.");
                        valid_centroid++;
                        break;
                    }
                }
            }
        }
    }
    centroid_csv.close();

    unsigned char valid_rock = 4;

    QFile rock_csv(rocks);
    if (!rock_csv.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        this->appendLog("Error: Could not open rock information file.");
    }
    else
    {
        QTextStream in_rock(&rock_csv);
        line = in_rock.readLine();
        csv_entry = line.split(",");
        guide = "Acceptable Rock Info CSV should only contain the following headers in order:\nid, name, dry_density, moisture";
        if (csv_entry.size() != 4)
        {
            QMessageBox::information(NULL, "Message", guide);
        }
        else
        {
            if (csv_entry.at(0) != QString("id"))
            {
                guide += QString("\n1st header should be 'id'.");
                QMessageBox::information(NULL, "Message", guide);
            }
            else
            {
                valid_rock--;
            }
            if (csv_entry.at(1) != QString("name"))
            {
                QMessageBox::information(NULL, "Message", QString("2nd header shoud be 'name'."));
            }
            else
            {
                valid_rock--;
            }
            if (csv_entry.at(2) != QString("dry_density"))
            {
                QMessageBox::information(NULL, "Message", QString("2nd header shoud be 'dry_density'."));
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
                long idx = -1;
                while (!in_centroid.atEnd())
                {
                    line = in_centroid.readLine();
                    csv_entry = line.split(",");
                    if (csv_entry.at(0).toLong() > (idx + 1))
                    {
                        if (csv_entry.at(0).toLong() - (idx + 1) > 1)
                        {
                            this->appendLog("Error: id column skipped at "
                                            + csv_entry.at(0)
                                            + ".");
                            valid_centroid++;
                            break;
                        }
                        idx = csv_entry.at(0).toLong() - 1;
                        this->mining_areas.push_back(std::make_shared<MiningArea>(csv_entry.at(1).toDouble(),
                                                                                  csv_entry.at(2).toDouble(),
                                                                                  csv_entry.at(3).toDouble(),
                                                                                  (size_t) idx,
                                                                                  log));
                        this->insertIndex(this->mining_areas[(size_t) idx]->asPointValue());
                    }
                    else if ((csv_entry.at(0).toLong() == (idx + 1)))
                    {
                        this->appendLog("Error: id column duplicated at "
                                        + csv_entry.at(0)
                                        + ".");
                        valid_centroid++;
                        break;
                    }
                    else
                    {
                        this->appendLog("Error: id column in collar csv is not sorted in ascending order.");
                        valid_centroid++;
                        break;
                    }
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

size_t BlockModel::areaCount() const
{
    return this->mining_areas.size();
}

size_t BlockModel::blockCount() const
{
    size_t block_count = 0;
    for (auto& mining_area : this->mining_areas)
    {
        block_count += mining_area->blockCount();
    }
    return block_count;
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

void BlockModel::appendLog(const QString& message)
{
    this->log->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ") + message);
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
