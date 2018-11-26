//
// Created by quist on 11/18/18.
//
#pragma once

#include <string>

using namespace std;

class Point
{
public:
    Point();
    Point(int x, int y);

    Point neighbour(string dir);
    bool operator==(const Point p) const;
    bool operator<(Point p);
    bool operator>(Point p);
    bool operator!=(Point p);

    int x;
    int y;
};
