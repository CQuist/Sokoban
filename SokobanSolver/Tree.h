//
// Created by quist on 11/5/18.
//

#pragma once

#include <iostream>
#include "NodeV2.h"
#include <math.h>
#include <bits/stdc++.h>
#include <sys/sysinfo.h>

using namespace std;

class Tree
{
public:
    Tree();
    Tree(vector<vector<int>> &map);

    string aStar();

private:
    NodeV2 initialState;
    vector <Point> goalPositions;
    vector <Point> holePositions;

    priority_queue <NodeV2, vector<NodeV2>, nodeCostCompare> openSet;

    unordered_set<NodeV2, KeyHasher> closedSet;

    int width = 0;
    int height = 0;

    void makeSuccessors(NodeV2 &node);
    void h_func(NodeV2 &node);

    bool isLegalRobotMove(NodeV2 &node, Point &dest, string &pathToPoint);
    bool isLegalCanMove(NodeV2 &node, Point &dest);
    bool isDeadLock(NodeV2 &node);
    bool deadLockedPoint(Point &point, Point &prevPoint, Point &originalPoint, NodeV2 &node);
    bool stateVisited(NodeV2 &node);
    bool outOfBounds(Point point);
    bool isGoal(NodeV2 &node);
    bool inDeadZone(Point &point);

    int robotGotCan(NodeV2 &node);

    vector<Point> updateCans(vector<Point> &canPos, Point &newPoint, int index);
    vector<Point> getNeighbours(Point point);

    static void bubbleSort(vector<Point> &array);
    static void bubbleSort(vector<Point> &array, int left, int right);
    inline static void swap(vector<Point> &array, int i, int j);

    unsigned long get_mem_total();

};