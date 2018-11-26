//
// Created by quist on 11/26/18.
//

#include "NodeV2.h"

NodeV2::NodeV2()
{

}

NodeV2::NodeV2(Point &robotPos, vector <Point> &canPos, string path, string nextStep)
{
    this->robotPosition = robotPos;
    this->canPositions = canPos;
    this->path = path.append(nextStep);

}

NodeV2::NodeV2(Point &robotPos, vector <Point> &canPos, string path)
{
    this->robotPosition = robotPos;
    this->canPositions = canPos;
    this->path = path;

}

int NodeV2::getCost()
{
    return path.size()+h;
}

bool NodeV2::operator==(const NodeV2 node) const
{
    bool returnVal = false;

    bool robotPos = (robotPosition == node.robotPosition);

    bool canPos = (canPositions.size() == node.canPositions.size());
    if (canPos)
        canPos = equal(canPositions.begin(), canPositions.end(), node.canPositions.begin());

    returnVal = (robotPos && canPos);

    return returnVal;
}

NodeV2::~NodeV2()
{

}
