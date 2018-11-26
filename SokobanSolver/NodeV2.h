//
// Created by quist on 11/26/18.
//

#pragma once

#include "Point.h"
#include "hungarianMethod/hungarian.hpp"
#include <vector>
#include <boost/functional/hash.hpp>


class NodeV2
{
public:
    vector <Point> canPositions;
    Point robotPosition;

    int h = INT16_MAX;

    string path = "";

    NodeV2();
    NodeV2(Point &robotPos, vector <Point> &canPos, string path, string nextStep);
    NodeV2(Point &robotPos, vector <Point> &canPos, string path);
    ~NodeV2();

    bool operator==(const NodeV2 node) const;

    int getCost();
};

struct nodeCostCompare
{
public:
    int operator() (NodeV2& n1, NodeV2& n2)
    {
        if (n1.getCost() == n2.getCost())
            return (n1.h > n2.h);

        return (n1.getCost() > n2.getCost());
    }
};

struct KeyHasher
{
    std::size_t operator()(const NodeV2& key) const
    {
        //sudo apt-get install libboost-all-dev
        using boost::hash_value;
        using boost::hash_combine;

        // Start with a hash value of 0    .
        std::size_t seed = 0;

        // Modify 'seed' by XORing and bit-shifting in
        // one member of 'Key' after the other:
        hash_combine(seed,hash_value(key.robotPosition.x));
        hash_combine(seed,hash_value(key.robotPosition.y));

        std::size_t seed2 = 0;
        for (int j = 0; j < key.canPositions.size(); ++j)
        {
            seed2 ^= key.canPositions[j].x + key.canPositions[j].y + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        hash_combine(seed,seed2);

        // Return the result.
        return seed;
    }
};