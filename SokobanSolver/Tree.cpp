//
// Created by quist on 11/5/18.
//

#include <iostream>
#include <algorithm>
#include <vector>
#include "Tree.h"
#include <iomanip>

Tree::Tree() {}

Tree::Tree(vector<vector<int>> &map)
{
    NodeV2 initialState;
    for (int i = 0; i < map.size(); ++i)
    {
        for (int j = 0; j < map[i].size(); ++j) {
            if (map[i][j] == 1)
            {
                initialState.robotPosition = Point(j,i);
            }
            else if (map[i][j] == 2 )
                initialState.canPositions.emplace_back(Point(j,i));
            else if  (map[i][j] == 3 )
                goalPositions.emplace_back(Point(j,i));
            else if  (map[i][j] == 4 )
                holePositions.emplace_back(Point(j,i));
        }
    }

    this->height = (int)map.size();
    this->width = (int)map[1].size();

    bubbleSort(goalPositions);

    this->initialState = initialState;

}

string Tree::aStar()
{
    int i = 0;
    openSet.push(initialState);

    while (!openSet.empty())
    {
        NodeV2 node = openSet.top();
        openSet.pop();

        cout << "Index: " << setw(8) << i << "  |  h: " << setw(3) << node.h << "  |  g: " << setw(3) <<  node.path.size() << "  |  Cost: " << setw(3) <<  node.h+node.path.size() << endl;

        if (isGoal(node))
            return node.path;
        else
        {
            closedSet.insert(node);
            makeSuccessors(node);
            if (get_mem_total() < 1000000)
                return "OUT OF MEMORY";
        }
        i++;
    }

    return "FAILED! NO PATH FOUND";
}


void Tree::makeSuccessors(NodeV2 &node)
{
    int canIndex = robotGotCan(node);
    if (canIndex == -1)
    {
        for (int i = 0; i < node.canPositions.size(); ++i)
        {
            Point leftNeighbour = node.canPositions[i].neighbour("left");
            Point rightNeighbour = node.canPositions[i].neighbour("right");
            Point upNeighbour = node.canPositions[i].neighbour("up");
            Point downNeighbour = node.canPositions[i].neighbour("down");

            string path = "";

            //move can right
            if (isLegalCanMove(node, rightNeighbour) && isLegalRobotMove(node, leftNeighbour, path))
            {
                path.append("rr");
                vector<Point> newCanPos = updateCans(node.canPositions, rightNeighbour, i);
                NodeV2 tempNode(rightNeighbour, newCanPos, path);
                if (!isDeadLock(tempNode) && !stateVisited(tempNode)){
                    h_func(tempNode);
                    openSet.emplace(tempNode);
                }
            }

            //move can left
            if (isLegalCanMove(node, leftNeighbour) && isLegalRobotMove(node, rightNeighbour, path))
            {
                path.append("ll");
                vector<Point> newCanPos = updateCans(node.canPositions, leftNeighbour, i);
                NodeV2 tempNode(leftNeighbour, newCanPos, path);
                if (!isDeadLock(tempNode) && !stateVisited(tempNode)){
                    h_func(tempNode);
                    openSet.emplace(tempNode);
                }
            }

            //move can up
            if (isLegalCanMove(node, upNeighbour) && isLegalRobotMove(node, downNeighbour, path))
            {
                path.append("uu");
                vector<Point> newCanPos = updateCans(node.canPositions, upNeighbour, i);
                NodeV2 tempNode(upNeighbour, newCanPos, path);
                if (!isDeadLock(tempNode) && !stateVisited(tempNode)){
                    h_func(tempNode);
                    openSet.emplace(tempNode);
                }
            }

            //move can down
            if (isLegalCanMove(node, downNeighbour) && isLegalRobotMove(node, upNeighbour, path))
            {
                path.append("dd");
                vector<Point> newCanPos = updateCans(node.canPositions, downNeighbour, i);
                NodeV2 tempNode(downNeighbour, newCanPos, path);
                if (!isDeadLock(tempNode) && !stateVisited(tempNode)){
                    h_func(tempNode);
                    openSet.emplace(tempNode);
                }
            }
        }
    }
    else
    {
        Point leftNeighbour = node.canPositions[canIndex].neighbour("left");
        Point rightNeighbour = node.canPositions[canIndex].neighbour("right");
        Point upNeighbour = node.canPositions[canIndex].neighbour("up");
        Point downNeighbour = node.canPositions[canIndex].neighbour("down");
        string path = "";

        // Last move with can was right
        if (*(node.path.end()-1) == 'r')
        {
            if (isLegalCanMove(node, rightNeighbour))
            {
                path.append("r");
                vector<Point> newCanPos = updateCans(node.canPositions, rightNeighbour, canIndex);
                NodeV2 tempNode(rightNeighbour, newCanPos, node.path, path);
                if (!isDeadLock(tempNode) && !stateVisited(tempNode)){
                    h_func(tempNode);
                    openSet.emplace(tempNode);
                }
            }

            path.clear();
            path.append("l");
            NodeV2 tempNode(leftNeighbour, node.canPositions, node.path, path);
            tempNode.h = node.h;
            openSet.emplace(tempNode);
        }// Last move with can was left
        else if (*(node.path.end()-1) == 'l')
        {
            if (isLegalCanMove(node, leftNeighbour))
            {
                path.append("l");
                vector<Point> newCanPos = updateCans(node.canPositions, leftNeighbour, canIndex);
                NodeV2 tempNode(leftNeighbour, newCanPos, node.path, path);
                if (!isDeadLock(tempNode) && !stateVisited(tempNode)){
                    h_func(tempNode);
                    openSet.emplace(tempNode);
                }
            }

            path.clear();
            path.append("r");
            NodeV2 tempNode(rightNeighbour, node.canPositions, node.path, path);
            tempNode.h = node.h;
            openSet.emplace(tempNode);
        }// Last move with can was up
        else if (*(node.path.end()-1) == 'u')
        {
            if (isLegalCanMove(node, upNeighbour))
            {
                path.append("u");
                vector<Point> newCanPos = updateCans(node.canPositions, upNeighbour, canIndex);
                NodeV2 tempNode(upNeighbour, newCanPos, node.path, path);
                if (!isDeadLock(tempNode) && !stateVisited(tempNode)){
                    h_func(tempNode);
                    openSet.emplace(tempNode);
                }
            }

            path.clear();
            path.append("d");
            NodeV2 tempNode(downNeighbour, node.canPositions, node.path, path);
            tempNode.h = node.h;
            openSet.emplace(tempNode);
        }// Last move with can was down
        else if (*(node.path.end()-1) == 'd')
        {
            if (isLegalCanMove(node, downNeighbour))
            {
                path.append("d");
                vector<Point> newCanPos = updateCans(node.canPositions, downNeighbour, canIndex);
                NodeV2 tempNode(downNeighbour, newCanPos, node.path, path);
                if (!isDeadLock(tempNode) && !stateVisited(tempNode)){
                    h_func(tempNode);
                    openSet.emplace(tempNode);
                }
            }

            path.clear();
            path.append("u");
            NodeV2 tempNode(upNeighbour, node.canPositions, node.path, path);
            tempNode.h = node.h;
            openSet.emplace(tempNode);
        }
    }
}

vector<Point> Tree::updateCans(vector<Point> &canPos, Point &newPoint, int index)
{
    vector<Point> tempCanPos = canPos;
    tempCanPos[index] = newPoint;
    return tempCanPos;
}

bool Tree::isLegalRobotMove(NodeV2 &passedNode, Point &dest, string &pathToPoint)
{
    pathToPoint.clear();
    deque<NodeV2> openList;
    openList.push_back(passedNode);
    unordered_set<NodeV2, KeyHasher> closedList;

    while (!openList.empty())
    {
        NodeV2 node = openList.front();
        openList.pop_front();
        if (node.robotPosition == dest)
        {
            pathToPoint = node.path;
            return true;
        }
        else if ((closedList.find(node) == closedList.end()))
        {
            closedList.insert(node);

            Point leftNeighbour = node.robotPosition.neighbour("left");
            Point rightNeighbour = node.robotPosition.neighbour("right");
            Point upNeighbour = node.robotPosition.neighbour("up");
            Point downNeighbour = node.robotPosition.neighbour("down");

            if (isLegalCanMove(node, leftNeighbour))
            {
                openList.emplace_back(NodeV2(leftNeighbour, node.canPositions, node.path, "l"));
            }
            if (isLegalCanMove(node, rightNeighbour))
            {
                openList.emplace_back(NodeV2(rightNeighbour, node.canPositions, node.path, "r"));
            }
            if (isLegalCanMove(node, upNeighbour))
            {
                openList.emplace_back(NodeV2(upNeighbour, node.canPositions, node.path, "u"));
            }
            if (isLegalCanMove(node, downNeighbour))
            {
                openList.emplace_back(NodeV2(downNeighbour, node.canPositions, node.path, "d"));
            }
        }
    }


    return false;
}

bool Tree::isLegalCanMove(NodeV2 &node, Point &dest)
{
    if (outOfBounds(dest))
        return false;

    for (int i = 0; i < holePositions.size(); ++i)
        if (dest == holePositions[i])
            return false;

    for (int j = 0; j < node.canPositions.size(); ++j)
    {
        if (dest == node.canPositions[j])
            return false;
    }

    return true;
}

bool Tree::outOfBounds(Point point)
{
    return (point.x >= width || point.x < 0 || point.y < 0 || point.y >= height);
};

bool Tree::isGoal(NodeV2 &node)
{
    vector<Point> canPos = node.canPositions;
    bubbleSort(canPos);

    for (int j = 0; j < canPos.size(); ++j)
    {
        if(canPos[j] != goalPositions[j])
            return false;
    }

    return true;
}

bool Tree::isDeadLock(NodeV2 &node)
{
    for (int i = 0; i < node.canPositions.size(); ++i)
    {
        bool canOnGoal = false;
        for (int j = 0; j < goalPositions.size(); ++j) {
            if (node.canPositions[i] == goalPositions[j])
                canOnGoal = true;
        }

        if (deadLockedPoint(node.canPositions[i], node.canPositions[i], node.canPositions[i], node) && !canOnGoal) {
            //cout << "DEADLOCK ON CAN AT: " << node.canPositions[i].x << node.canPositions[i].y << endl;
            return true;
        }
    }

    return false;
}

bool Tree::deadLockedPoint(Point &point, Point &prevPoint, Point &originalPoint, NodeV2 &node)
{

    vector<int> obstacles;
    vector<Point> neighbours = getNeighbours(point);
    for (int j = 0; j < neighbours.size(); ++j)
    {
        if (outOfBounds(neighbours[j]))
        {
            obstacles.push_back(j);
            obstacles.push_back(0);
        }


        for (int k = 0; k < holePositions.size(); ++k)
        {
            if (neighbours[j] == holePositions[k])
            {
                obstacles.push_back(j);
                obstacles.push_back(0);
            }
        }
        for (int l = 0; l < node.canPositions.size(); ++l)
        {
            if (neighbours[j] == node.canPositions[l])
            {
                obstacles.push_back(j);
                obstacles.push_back(1);
            }
        }
    }
    for (int m = 2; m < obstacles.size(); m = m+2)
    {
        if ((obstacles[m-1] == 0 && obstacles[m+1] == 0) && (abs(obstacles[m-2] - obstacles[m]) == 1 || abs(obstacles[obstacles.size()-2] - obstacles[0]) == 3))
        {
            return true;
        }
        else if ((obstacles[m-1] == 1 && obstacles[m+1] == 0) && (abs(obstacles[m-2] - obstacles[m]) == 1 || abs(obstacles[obstacles.size()-2] - obstacles[0]) == 3))
        {
            if (neighbours[obstacles[m-2]] == prevPoint || neighbours[obstacles[m-2]] == originalPoint)
                return true;
            else if (deadLockedPoint(neighbours[obstacles[m-2]], point, originalPoint, node))
                return true;
        }
        else if ((obstacles[m-1] == 0 && obstacles[m+1] == 1) && (abs(obstacles[m-2] - obstacles[m]) == 1 || abs(obstacles[obstacles.size()-2] - obstacles[0]) == 3))
        {
            if (neighbours[obstacles[m]] == prevPoint || neighbours[obstacles[m]] == originalPoint)
                return true;
            else if (deadLockedPoint(neighbours[obstacles[m]], point, originalPoint, node))
                return true;
        }
        else if ((obstacles[m-1] == 1 && obstacles[m+1] == 1) && (abs(obstacles[m-2] - obstacles[m]) == 1 || abs(obstacles[obstacles.size()-2] - obstacles[0]) == 3))
        {
            bool tempBool = false;
            if (neighbours[obstacles[m]] == prevPoint || neighbours[obstacles[m]] == originalPoint)
                tempBool = true;
            else if (deadLockedPoint(neighbours[obstacles[m]], point, originalPoint, node))
                tempBool = true;

            bool tempBool2 = false;
            if (neighbours[obstacles[m-2]] == prevPoint || neighbours[obstacles[m-2]] == originalPoint)
                tempBool2 = true;
            else if (deadLockedPoint(neighbours[obstacles[m-2]], point, originalPoint, node))
                tempBool2 = true;

            return  (tempBool && tempBool2);
        }
    }
    return false;
}

vector<Point> Tree::getNeighbours(Point point)
{
    vector<Point> neighbours;

    neighbours.emplace_back(Point(point.x,point.y-1));
    neighbours.emplace_back(Point(point.x+1,point.y));
    neighbours.emplace_back(Point(point.x,point.y+1));
    neighbours.emplace_back(Point(point.x-1,point.y));

    return neighbours;
}

bool Tree::stateVisited(NodeV2 &node)
{
    return !(closedSet.find(node) == closedSet.end());
}

int Tree::robotGotCan(NodeV2 &node)
{
    for (int i = 0; i < node.canPositions.size(); ++i)
    {
        if (node.robotPosition == node.canPositions[i])
            return i;
    }
    return -1;
}

void Tree::h_func(NodeV2 &node)
{
    vector<vector<int>> outerVector;
    //int playerToCanManhattenValue = 0;

    for (int i = 0; i < node.canPositions.size(); ++i)
    {
        vector<int> innerVector;

        for (int j = 0; j < goalPositions.size(); ++j)
        {
            innerVector.push_back(abs(node.canPositions[i].x - goalPositions[j].x) + abs(node.canPositions[i].y - goalPositions[j].y));

        }
        outerVector.push_back(innerVector);

        //playerToCanManhattenValue += abs(node.robotPosition.x - node.canPositions[i].x) + abs(node.robotPosition.y - node.canPositions[i].y);
    }

    Hungarian::Matrix costMatrix = outerVector;

    Hungarian::Result minimumCost = Solve(costMatrix, Hungarian::MODE_MINIMIZE_COST);

    node.h = minimumCost.totalCost;// + playerToCanManhattenValue;
}

void Tree::bubbleSort(vector<Point> &array)
{
    bubbleSort(array, 0, array.size()-1);
}

void Tree::bubbleSort(vector<Point> &array, int left, int right)
{
    bool swapped = false;
    for (int i = left; i <= right; ++i)
    {
        for (int j = left; j <= right - i - 1 + left; ++j)
        {
            if (array[j] > array[j + 1])
            {
                swap(array, j, j + 1);
                swapped = true;
            }
        }
        if(!swapped)
            return;
    }
}

inline void Tree::swap(vector<Point> &array, int i, int j)
{
    Point indexI = array[i];
    array[i] = array[j];
    array[j] = indexI;
}

unsigned long Tree::get_mem_total() {
    std::string token;
    std::ifstream file("/proc/meminfo");
    while(file >> token) {
        if(token == "MemAvailable:") {
            unsigned long mem;
            if(file >> mem) {
                return mem;
            } else {
                return 0;
            }
        }
        // ignore rest of the line
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return 0; // nothing found
}