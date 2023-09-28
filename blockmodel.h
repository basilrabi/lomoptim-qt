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

    std::vector<Block*> blocks;
    std::vector<MiningArea*> adjacent_areas;
    unsigned char excavated;

public:
    MiningArea();
    ~MiningArea();

    bool canBeExcavated() const;
};


class BlockModel
{
    // Container for all mining areas
    std::vector<MiningArea*> mining_areas;

    // Densities per rock type in tons per cubic meter
    std::vector<double> dry_densities;

    // Assumed insity moisture content of each rock type
    std::vector<double> moisture_content;

public:
    BlockModel();
    BlockModel(QString blocks, QString centroids, QString rocks);
    ~BlockModel();
};

#endif // BLOCKMODEL_H
