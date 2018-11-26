//
// Created by quist on 11/5/18.
//

#pragma once

#include <iostream>
#include "Node.h"
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

    unsigned long get_mem_total();

    Node initialState;

    priority_queue <Node, vector<Node>, nodeCostCompare> openSet;

    //vector<Node> closedSet;
    unordered_set<Node, KeyHasher> closedSet;

    int width = 0;
    int height = 0;

    bool stateVisited(Node &node);
    bool isGoal(Node &node);
    bool isDeadLock(Node &node);
    bool isLegalMove(Node &node);
    vector<Point> getNeighbours(Point point);
    void filterAndMerge(Node &node);
    char backWards(char &step);

    bool deadLockedPoint(Point &point, Point &prevPoint, Point &originalPoint, Node &node);
    bool outOfBounds(Point poinjt);

    static void bubbleSort(vector<Point> &array);
    static void bubbleSort(vector<Point> &array, int left, int right);
    inline static void swap(vector<Point> &array, int i, int j);

};