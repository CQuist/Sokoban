//
// Created by quist on 11/5/18.
//

#pragma once

#include <iostream>
#include "Node.h"
#include <math.h>
#include <bits/stdc++.h>

using namespace std;

class Tree
{
public:
    Tree();
    Tree(vector<vector<int>> &map);

    string aStar();

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

    bool deadLockedPoint(Point point, Node &node);


    static const int CUTOFF = 3;
    static void bubbleSort(deque<Node> &array, int left, int right);
    static void quickSort(deque<Node> &array);
    static void quickSort(deque<Node> &array, int left, int right);
    static int median(deque<Node> &array, int left, int right);
    inline static void swap(deque<Node> &array, int i, int j);

    static void bubbleSort(vector<Point> &array);
    static void bubbleSort(vector<Point> &array, int left, int right);
    inline static void swap(vector<Point> &array, int i, int j);

};