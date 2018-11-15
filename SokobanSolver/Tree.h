//
// Created by quist on 11/5/18.
//

#pragma once

#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <deque>

using namespace std;

struct Point
{
    int x;
    int y;

    Point()
    {
        x = 0;
        y = 0;
    }
    Point(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    static double getDist(Point p1, Point p2)
    {
        int dx = p1.x - p2.x;
        int dy = p1.y - p2.y;

        return sqrt(dx*dx + dy*dy);
    }

    Point move(string dir)
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

    bool operator==(Point p)
    {
        return (x == p.x && y == p.y);
    }

    bool operator<(Point p)
    {
        return (x < p.x) || (x == p.x && y < p.y);
    }

    bool operator>(Point p)
    {
        return (x > p.x) || (x == p.x && y > p.y);
    }

    bool operator!=(Point p)
    {
        return (x != p.x || y != p.y);
    }

};


struct Node
{
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


    Node(){}

    Node(Point robotPosition, vector <Point> canPositions, vector <Point> goalPositions, vector <Point> holePositions, vector<Point> goalsWithCans ,int &g, string currentPath, string nextStep)
    {
        this->robotPosition = robotPosition;
        this->canPositions = canPositions;
        this->goalPositions = goalPositions;
        this->holePositions = holePositions;
        this->g = g;
        this->h = h_func();
        this->path = currentPath.append(nextStep);
        this->goalIndex = robotOnGoal();
        this->goalsWithCans = goalsWithCans;

    }

    int robotGotCan()
    {
        for (int i = 0; i < canPositions.size(); ++i)
        {
            if (robotPosition == canPositions[i])
                return i;
        }
        return -1;
    }

    int robotOnGoal()
    {
        for (int i = 0; i < goalPositions.size(); ++i)
        {
            if (robotPosition == goalPositions[i])
                return i;
        }
        return -1;
    }

    vector <Point> updateCanPos(string dir, int index)
    {
        vector<Point> newCanPos = canPositions;
        newCanPos[index] = canPositions[index].move(dir);
        return newCanPos;
    }

    vector <Point> updateGoals()
    {

        vector<Point> newGoalPos = goalPositions;
        newGoalPos.erase(newGoalPos.begin()+goalIndex);
        return newGoalPos;
    }

    vector <Point> updateGoalsWithCans()
    {
        vector <Point> newGoalsWithCans = goalsWithCans;
        newGoalsWithCans.push_back(goalPositions[goalIndex]);
        return newGoalsWithCans;
    }

    vector <Point> updateHoles()
    {
        vector<Point> newHolePos = holePositions;
        newHolePos.push_back(canPositions[canIndex]);
        return newHolePos;
    }

    vector <Point> updateCans()
    {
        vector<Point> newCanPos = canPositions;
        newCanPos.erase(newCanPos.begin()+canIndex);
        return newCanPos;
    }

    void makeSuccessors()
    {
        canIndex = robotGotCan();
        int g_value = g_func();
        if (canIndex != -1)
        {
            if (goalIndex != -1)
            {
                if (*(path.end()-1) == 'd') {
                    goUp = new Node(robotPosition.move("up"), updateCans(), updateGoals(), updateHoles(), updateGoalsWithCans(), g_value, path, "u");
                    goDown = new Node(robotPosition.move("down"), updateCanPos("down", canIndex), goalPositions, holePositions, goalsWithCans, g_value, path, "d");

                    goRight = new Node(robotPosition.move("up"), canPositions, goalPositions, holePositions, goalsWithCans,g_value, path, "u");
                }
                else if (*(path.end()-1) == 'l') {
                    goRight = new Node(robotPosition.move("right"), updateCans(), updateGoals(), updateHoles(), updateGoalsWithCans(), g_value, path, "r");
                    goLeft = new Node(robotPosition.move("left"), updateCanPos("left", canIndex), goalPositions, goalsWithCans, holePositions, g_value, path, "l");

                    goUp = new Node(robotPosition.move("right"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "r");
                }
                else if (*(path.end()-1) == 'r') {
                    goLeft = new Node(robotPosition.move("left"), updateCans(), updateGoals(), updateHoles(), updateGoalsWithCans(), g_value, path, "l");
                    goRight = new Node(robotPosition.move("right"), updateCanPos("right", canIndex), goalPositions, goalsWithCans, holePositions, g_value, path, "r");

                    goDown = new Node(robotPosition.move("left"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "l");
                }
                else if (*(path.end()-1) == 'u') {
                    goDown = new Node(robotPosition.move("down"), updateCans(), updateGoals(), updateHoles(), updateGoalsWithCans(), g_value, path, "d");
                    goUp = new Node(robotPosition.move("up"), updateCanPos("up", canIndex), goalPositions, holePositions, goalsWithCans, g_value, path, "u");

                    goLeft = new Node(robotPosition.move("down"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "d");
                }
            } else {
                if (*(path.end()-1) == 'd')
                {
                    goUp = new Node(robotPosition.move("up"), canPositions, goalPositions, holePositions, goalsWithCans,g_value, path, "u");
                    goDown = new Node(robotPosition.move("down"), updateCanPos("down", canIndex), goalPositions, holePositions, goalsWithCans, g_value, path, "d");
                }
                else if (*(path.end()-1) == 'l')
                {
                    goRight = new Node(robotPosition.move("right"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "r");
                    goLeft = new Node(robotPosition.move("left"), updateCanPos("left", canIndex), goalPositions, goalsWithCans, holePositions, g_value, path, "l");
                }
                else if (*(path.end()-1) == 'r')
                {
                    goLeft = new Node(robotPosition.move("left"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "l");
                    goRight = new Node(robotPosition.move("right"), updateCanPos("right", canIndex), goalPositions, goalsWithCans, holePositions, g_value, path, "r");
                }
                else if (*(path.end()-1) == 'u')
                {
                    goDown = new Node(robotPosition.move("down"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "d");
                    goUp = new Node(robotPosition.move("up"), updateCanPos("up", canIndex), goalPositions, holePositions, goalsWithCans, g_value, path, "u");
                }
            }
        }
        else
        {
            if (*(path.end()-1) == 'd')
            {
                goRight = new Node(robotPosition.move("right"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "r");
                goLeft = new Node(robotPosition.move("left"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "l");
                goDown = new Node(robotPosition.move("down"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "d");
            }
            else if (*(path.end()-1) == 'l')
            {
                goUp = new Node(robotPosition.move("up"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "u");
                goLeft = new Node(robotPosition.move("left"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "l");
                goDown = new Node(robotPosition.move("down"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "d");
            }
            else if (*(path.end()-1) == 'r')
            {
                goUp = new Node(robotPosition.move("up"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "u");
                goRight = new Node(robotPosition.move("right"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "r");
                goDown = new Node(robotPosition.move("down"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "d");
            }
            else if (*(path.end()-1) == 'u')
            {
                goUp = new Node(robotPosition.move("up"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "u");
                goRight = new Node(robotPosition.move("right"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "r");
                goLeft = new Node(robotPosition.move("left"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "l");
            }
            else if (path.size() < 1)
            {
                goUp = new Node(robotPosition.move("up"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "u");
                goRight = new Node(robotPosition.move("right"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "r");
                goLeft = new Node(robotPosition.move("left"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "l");
                goDown = new Node(robotPosition.move("down"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "d");
            }
        }
    }

    int getCost()
    {
        return g+h;
    }

    int h_func()
    {
        int manhattenValue = 0;
        for (int i = 0; i < canPositions.size(); ++i)
        {
            int dist = INT16_MAX;
            for (int j = 0; j < goalPositions.size(); ++j)
            {
                int tempDist = abs(canPositions[i].x - goalPositions[j].x) + abs(canPositions[i].y - goalPositions[j].y);
                if (tempDist < dist)
                    dist = tempDist;
            }
            manhattenValue += dist;
        }

        return manhattenValue;
    }

    int g_func()
    {
        return g+1;
    }

    bool operator==(Node node)
    {
        bool returnVal = false;
        // THEESE NEEDS TO BE SORTED THEN ITS ALL GOOD

        /*if (robotPosition == node.robotPosition)
        {
            returnVal = true;
            for (int i = 0; i < canPositions.size(); ++i)
            {
                if (canPositions[i] != node.canPositions[i])
                    returnVal = false;
            }
            for (int j = 0; j < goalPositions.size(); ++j)
            {
                if (goalPositions[j] != node.goalPositions[j])
                    returnVal = false;
            }
            for (int k = 0; k < holePositions.size(); ++k)
            {
                if (holePositions[k] != node.holePositions[k])
                    returnVal = false;
            }
            for (int l = 0; l < goalsWithCans.size(); ++l)
            {
                if (goalsWithCans[l] != node.goalsWithCans[l])
            }
        }*/

        bool robotPos = robotPosition == node.robotPosition;

        bool canPos = (canPositions.size() == node.canPositions.size());
        if (canPos)
            canPos = equal(canPositions.begin(), canPositions.end(), node.canPositions.begin());

        bool goalPos = (goalPositions.size() == node.goalPositions.size());
        if (goalPos)
            goalPos = equal(goalPositions.begin(), goalPositions.end(), node.goalPositions.begin());

        bool holePos = (holePositions.size() == node.holePositions.size());
        if (holePos)
            holePos = equal(holePositions.begin(), holePositions.end(), node.holePositions.begin());

        bool goalsNcans = (goalsWithCans.size() == node.goalsWithCans.size());
        if (goalsNcans)
            goalsNcans = equal(goalsWithCans.begin(), goalsWithCans.end(), node.goalsWithCans.begin());


        bool pathEqual = (*(path.end()-1) == *(node.path.end()-1));

        returnVal = (robotPos && canPos && goalPos && holePos && goalsNcans && pathEqual);

        return returnVal;
    }

};


class Tree
{
public:
    Tree();
    Tree(vector<vector<int>> &map);

    string aStar();



    Node initialState;

    deque<Node> openSet;
    vector<Node> closedSet;

    int width = 0;
    int height = 0;

    bool stateVisited(Node &node);
    bool isGoal(Node &node);
    bool isDeadLock(Node &node);
    bool isLegalMove(Node &node);
    void sortPoints(vector<Point> & list);
    void filterAndMerge(Node &node);
    char backWards(char &step);


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