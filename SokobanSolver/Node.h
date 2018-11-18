//
// Created by quist on 11/18/18.
//

#pragma once

#include "Point.h"
#include "hungarianMethod/hungarian.hpp"
#include <vector>
#include <boost/functional/hash.hpp>

class Node
{
public:
    vector <Point> canPositions;
    vector <Point> goalPositions;
    vector <Point> holePositions;
    Point robotPosition;
    int canIndex = -1;
    int goalIndex = -1;

    vector <Point> goalsWithCans;

    Node* goUp = nullptr;
    Node* goRight = nullptr;
    Node* goLeft = nullptr;
    Node* goDown = nullptr;

    int h = INT16_MAX;
    int g = 0;

    string path = "";

    Node();
    Node(Point robotPosition, vector <Point> canPositions, vector <Point> goalPositions, vector <Point> holePositions, vector<Point> goalsWithCans ,int &g, string currentPath, string nextStep);

    int robotGotCan();
    int robotOnGoal();
    vector<Point> updateCanPos(string dir, int index);
    vector<Point> updateGoals();
    vector<Point> updateGoalsWithCans();
    vector<Point> updateHoles();
    vector<Point> updateCans();
    void makeSuccessors();
    int getCost();
    int h_func();
    int g_func();

    bool operator==(const Node node) const;
};

struct nodeCostCompare
{
public:
    int operator() (Node& n1, Node& n2)
    {
        return (n1.getCost() > n2.getCost());
    }
};

struct KeyHasher
{
    std::size_t operator()(const Node& key) const
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

        std::size_t seed3 = 0;
        for (int j = 0; j < key.goalPositions.size(); ++j)
        {
            seed2 ^= key.goalPositions[j].x + key.goalPositions[j].y + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        hash_combine(seed,seed3);

        std::size_t seed4 = 0;
        for (int j = 0; j < key.holePositions.size(); ++j)
        {
            seed2 ^= key.holePositions[j].x + key.holePositions[j].y + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        hash_combine(seed,seed4);

        std::size_t seed5 = 0;
        for (int j = 0; j < key.goalsWithCans.size(); ++j)
        {
            seed2 ^= key.goalsWithCans[j].x + key.goalsWithCans[j].y + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        hash_combine(seed,seed5);

        hash_combine(seed,hash_value(*(key.path.end()-1)));

        // Return the result.
        return seed;
    }
};

