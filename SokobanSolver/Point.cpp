//
// Created by quist on 11/18/18.
//

#include "Point.h"

Point::Point()
{
    x = 0;
    y = 0;
}
Point::Point(int x, int y)
{
    this->x = x;
    this->y = y;
}

Point Point::move(string dir)
{
    if (dir == "up")
        return Point(x,y-1);
    else if (dir == "left")
        return Point(x-1, y);
    else if (dir == "right")
        return Point(x+1, y);
    else if (dir == "down")
        return Point(x, y+1);
}

bool Point::operator==(const Point p) const
{
    return (x == p.x && y == p.y);
}

bool Point::operator<(Point p)
{
    return (x < p.x) || (x == p.x && y < p.y);
}

bool Point::operator>(Point p)
{
    return (x > p.x) || (x == p.x && y > p.y);
}

bool Point::operator!=(Point p)
{
    return (x != p.x || y != p.y);
}