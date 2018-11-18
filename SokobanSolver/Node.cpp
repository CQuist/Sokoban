//
// Created by quist on 11/18/18.
//

#include "Node.h"


Node::Node(){}

Node::Node(Point robotPosition, vector <Point> canPositions, vector <Point> goalPositions, vector <Point> holePositions, vector<Point> goalsWithCans ,int &g, string currentPath, string nextStep)
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

int Node::robotGotCan()
{
    for (int i = 0; i < canPositions.size(); ++i)
    {
        if (robotPosition == canPositions[i])
            return i;
    }
    return -1;
}

int Node::robotOnGoal()
{
    for (int i = 0; i < goalPositions.size(); ++i)
    {
        if (robotPosition == goalPositions[i])
            return i;
    }
    return -1;
}

vector<Point> Node::updateCanPos(string dir, int index)
{
    vector<Point> newCanPos = canPositions;
    newCanPos[index] = canPositions[index].move(dir);
    return newCanPos;
}

vector<Point> Node::updateGoals()
{

    vector<Point> newGoalPos = goalPositions;
    newGoalPos.erase(newGoalPos.begin()+goalIndex);
    return newGoalPos;
}

vector<Point> Node::updateGoalsWithCans()
{
    vector <Point> newGoalsWithCans = goalsWithCans;
    newGoalsWithCans.push_back(goalPositions[goalIndex]);
    return newGoalsWithCans;
}

vector<Point> Node::updateHoles()
{
    vector<Point> newHolePos = holePositions;
    newHolePos.push_back(canPositions[canIndex]);
    return newHolePos;
}

vector<Point> Node::updateCans()
{
    vector<Point> newCanPos = canPositions;
    newCanPos.erase(newCanPos.begin()+canIndex);
    return newCanPos;
}

void Node::makeSuccessors()
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
                goLeft = new Node(robotPosition.move("left"), updateCanPos("left", canIndex), goalPositions, holePositions, goalsWithCans, g_value, path, "l");

                goUp = new Node(robotPosition.move("right"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "r");
            }
            else if (*(path.end()-1) == 'r') {
                goLeft = new Node(robotPosition.move("left"), updateCans(), updateGoals(), updateHoles(), updateGoalsWithCans(), g_value, path, "l");
                goRight = new Node(robotPosition.move("right"), updateCanPos("right", canIndex), goalPositions, holePositions, goalsWithCans, g_value, path, "r");

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
                goLeft = new Node(robotPosition.move("left"), updateCanPos("left", canIndex), goalPositions, holePositions, goalsWithCans, g_value, path, "l");
            }
            else if (*(path.end()-1) == 'r')
            {
                goLeft = new Node(robotPosition.move("left"), canPositions, goalPositions, holePositions, goalsWithCans, g_value, path, "l");
                goRight = new Node(robotPosition.move("right"), updateCanPos("right", canIndex), goalPositions, holePositions, goalsWithCans, g_value, path, "r");
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

int Node::getCost()
{
    return g+h;
}

int Node::h_func()
{
    vector<vector<int>> outerVector;
    int playerToCanManhattenValue = 0;

    for (int i = 0; i < canPositions.size(); ++i)
    {
        vector<int> innerVector;

        for (int j = 0; j < goalPositions.size(); ++j)
        {
            innerVector.push_back(abs(canPositions[i].x - goalPositions[j].x) + abs(canPositions[i].y - goalPositions[j].y));

        }
        outerVector.push_back(innerVector);

        playerToCanManhattenValue += abs(robotPosition.x - canPositions[i].x) + abs(robotPosition.y - canPositions[i].y);
    }

    Hungarian::Matrix costMatrix = outerVector;

    Hungarian::Result minimumCost = Solve(costMatrix, Hungarian::MODE_MINIMIZE_COST);

    return minimumCost.totalCost;// + playerToCanManhattenValue;
}

int Node::g_func()
{
    return g+1;
}

bool Node::operator==(const Node node) const
{
    bool returnVal = false;


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