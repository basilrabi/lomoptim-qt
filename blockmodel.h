#ifndef BLOCKMODEL_H
#define BLOCKMODEL_H

#include "boost.h"
#include <QString>
#include <QTextBrowser>
#include <vector>

class Block
{
    // volume fractions per rock type component
    std::vector<double> component_fractions;

    // Cost in PhP per WMT if mined as product
    std::vector<double> mining_costs;

    // Cost in PhP per WMT if mined as waste
    std::vector<double> waste_costs;

    // Revenue in PhP per WMT
    std::vector<double> revenues;

public:
    Block();
    ~Block();

    double profit() const;
    std::vector<double> wmt() const;
};


class MiningArea
{
    QTextBrowser *log;

    // Centroid coordinates of the mining area.
    double x;
    double y;

    // elevation of the centroid of the top block
    double z;

    std::vector<std::shared_ptr<Block>> blocks;
    std::vector<std::shared_ptr<MiningArea>> adjacent_areas;
    size_t id;
    unsigned char excavated;

public:
    MiningArea(const double& x_coordinates,
               const double& y_coordinates,
               const double& z_coordinates,
               const size_t& id,
               QTextBrowser *log);
    ~MiningArea();

    // Checks whether the exposed area can be excavated by applying the design parameters constraints.
    bool canBeExcavated() const;

    // Returns the bounding box of the area which also covers nearby mining areas.
    // The dimensions of the bounding box will be X = 2 * size_x and Y = 2 * size_y.
    // The centroid of the bounding is the centroid of the mining area.
    box_2d asBox2D(const double& size_x, const double& size_y) const;

    // Returns the centroid of the mining area.
    point_2d asPoint2D() const;

    // Returns the centroid along with the id of the mining area.
    point_value asPointValue() const;

    // Returns the number of blocks within the mining area.
    unsigned char blockCount() const;

    // Returns the id of the mining area.
    unsigned long getId() const;

    // Sets adjacent_areas;
    void setAdjacentArea(std::vector<std::shared_ptr<MiningArea>> adjacent_areas);
};


class BlockModel
{
    QTextBrowser *log;

    bool initialized = 0;

    // Maximum allowable height of bench
    double bench_height;

    // Angle of the bench
    double bench_slope;

    // Annual mining limit in wet metric tons
    double mining_limit;

    // Block dimensions in meters.
    double size_x;
    double size_y;
    double size_z;

    // An R-tree index to be used for querying adjacent areas.
    rtree_t index;

    std::vector<QString> rock_types;

    // Container for all mining areas.
    std::vector<std::shared_ptr<MiningArea>> mining_areas;

    // Densities per rock type in tons per cubic meter.
    std::vector<double> dry_densities;

    // Required annual mining production per rock type in wet metric tons.
    std::vector<double> minimum_production;

    // Assumed insity moisture content of each rock type.
    std::vector<double> moisture_content;

public:
    BlockModel(const QString& blocks,
               const QString& centroids,
               const QString& rocks,
               const double& x,
               const double& y,
               const double& z,
               QTextBrowser *log);
    ~BlockModel();

    // Checks the BlockModel if properly initialized.
    bool isInitialized() const;

    // Returns the vector of MiningAreas adjacent to the input MiningArea.
    std::vector<std::shared_ptr<MiningArea>> getAdjacentAreas(std::shared_ptr<MiningArea> mining_area);

    // Returns the number of areas in the BlockModel.
    unsigned long areaCount() const;

    // Returns the number of blocks in the BlockModel.
    unsigned long long blockCount() const;

    // Inserts a point_value to the R-Tree index.
    void insertIndex(const point_value& pv);

    // Builds the vector of adjacent areas per mining area.
    void populateAdjacentMiningArea();
};

#endif // BLOCKMODEL_H
