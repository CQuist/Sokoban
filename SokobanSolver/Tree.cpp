//
// Created by quist on 11/5/18.
//

#include <iostream>
#include <algorithm>
#include <vector>
#include "Tree.h"

Tree::Tree() {}

Tree::Tree(vector<vector<int>> &map)
{
    Node initialState;
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
                initialState.goalPositions.emplace_back(Point(j,i));
            else if  (map[i][j] == 4 )
                initialState.holePositions.emplace_back(Point(j,i));
        }
    }

    this->height = (int)map.size();
    this->width = (int)map[1].size();

    bubbleSort(initialState.goalPositions);

    this->initialState = initialState;

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

string Tree::aStar()
{
    string output;

    int i = 0;
    openSet.push(initialState);

    while (!openSet.empty())
    {
        Node node = openSet.top();
        openSet.pop();
        if (isGoal(node))
            return node.path;
        else
        {
            filterAndMerge(node);
            cout << "Index: " << i << " h: " <<  node.h << " g: " << node.g << " Total Cost: " << node.h+node.g << endl;
            if (get_mem_total() < 1000000)
                return "OUT OF MEMORY";
        }
        i++;
    }

    return "FAILED! NO PATH FOUND";
}


bool Tree::isGoal(Node &node)
{
    if (node.goalPositions.size() == 1 && node.goalPositions[0] == node.canPositions[0])
    {
        vector<Point> canPos = node.goalsWithCans;
        canPos.push_back(node.canPositions[0]);
        vector<Point> goalPos = initialState.goalPositions;
        if (canPos.size() != goalPos.size())
            return false;

        bubbleSort(canPos);
        bubbleSort(goalPos);

        for (int i = 0; i < canPos.size(); ++i) {
            if (canPos[i] != goalPos[i])
                return false;
        }
        return true;
    }
    return false;
}

bool Tree::isDeadLock(Node &node)
{
    for (int i = 0; i < node.canPositions.size(); ++i)
    {
        bool canOnGoal = false;
        for (int j = 0; j < node.goalPositions.size(); ++j) {
            if (node.canPositions[i] == node.goalPositions[j])
                canOnGoal = true;
        }

        if (deadLockedPoint(node.canPositions[i], node.canPositions[i], node.canPositions[i], node) && !canOnGoal) {
            //cout << "DEADLOCK ON CAN AT: " << node.canPositions[i].x << node.canPositions[i].y << endl;
            return true;
        }
    }

    return false;
}

bool Tree::deadLockedPoint(Point &point, Point &prevPoint, Point &originalPoint, Node &node)
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


        for (int k = 0; k < initialState.holePositions.size(); ++k)
        {
            if (neighbours[j] == initialState.holePositions[k])
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

bool Tree::outOfBounds(Point point)
{
    return (point.x >= width || point.x < 0 || point.y < 0 || point.y >= height);
};

bool Tree::stateVisited(Node &node)
{
    return !(closedSet.find(node) == closedSet.end());
}

void Tree::filterAndMerge(Node &node)
{
    closedSet.insert(node);
    node.makeSuccessors();

    if (node.goUp != nullptr && isLegalMove(*node.goUp) && !isDeadLock(*node.goUp) && !stateVisited(*node.goUp))
        openSet.push(*node.goUp);
    if (node.goDown != nullptr && isLegalMove(*node.goDown) && !isDeadLock(*node.goDown) && !stateVisited(*node.goDown))
        openSet.push(*node.goDown);
    if (node.goLeft != nullptr && isLegalMove(*node.goLeft) && !isDeadLock(*node.goLeft) && !stateVisited(*node.goLeft))
        openSet.push(*node.goLeft);
    if (node.goRight != nullptr && isLegalMove(*node.goRight) && !isDeadLock(*node.goRight) && !stateVisited(*node.goRight))
        openSet.push(*node.goRight);
};

bool Tree::isLegalMove(Node &node)
{
    if (outOfBounds(node.robotPosition))
        return false;

    for (int i = 0; i < node.holePositions.size(); ++i)
        if (node.robotPosition == node.holePositions[i])
            return false;

    int k = 0;
    for (int j = 0; j < node.canPositions.size(); ++j)
    {
        if (node.robotPosition == node.canPositions[j])
            k++;
        if (k > 1)
            return false;
    }


    bool tempBool = false;
    if ((node.canIndex != -1 && (*(node.path.end()-1) == *(node.path.end()-2) || *(node.path.end()-1) == backWards(*(node.path.end()-2)) || node.path.size() < 2)) || (node.canIndex == -1))
        tempBool = true;

    return tempBool;
}

char Tree::backWards(char &step)
{
    if (step == 'u')
        return 'd';
    if (step == 'd')
        return 'u';
    if (step == 'r')
        return 'l';
    if (step == 'l')
        return 'r';
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