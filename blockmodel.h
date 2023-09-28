#ifndef BLOCKMODEL_H
#define BLOCKMODEL_H

#include <QString>
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
    // Centroid coordinates of the mining area.
    double x;
    double y;

    // elevation of the centroid of the top block
    double z;

    std::vector<std::shared_ptr<Block>> blocks;
    std::vector<std::shared_ptr<MiningArea>> adjacent_areas;
    unsigned char excavated;

public:
    MiningArea();
    ~MiningArea();

    bool canBeExcavated() const;
};


class BlockModel
{
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

    // Container for all mining areas
    std::vector<std::shared_ptr<MiningArea>> mining_areas;

    // Densities per rock type in tons per cubic meter
    std::vector<double> dry_densities;

    // Required annual mining production per rock type in wet metric tons
    std::vector<double> minimum_production;

    // Assumed insity moisture content of each rock type
    std::vector<double> moisture_content;

public:
    BlockModel();
    BlockModel(QString blocks, QString centroids, QString rocks, double x, double y, double z);
    ~BlockModel();
};

#endif // BLOCKMODEL_H
