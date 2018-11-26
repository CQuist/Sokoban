#include <iostream>
#include "Tree.h"
#include <chrono>

int main()
{
    std::chrono::seconds startTime = std::chrono::duration_cast< std::chrono::seconds >(std::chrono::system_clock::now().time_since_epoch());
    /*
    priority_queue <Node, vector<Node>, nodeCostCompare> openSet;

    for (int i = 0; i < 1000; ++i)
    {
        Node tempnode;
        tempnode.h = rand() % 100;
        tempnode.g = rand() % 100;
        openSet.push(tempnode);
        cout << tempnode.h << " " << tempnode.g << " " << tempnode.getCost() << endl;
    }

    cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;

    int j = 0;
    while (!openSet.empty())
    {
        Node tempNode = openSet.top();
        cout << tempNode.getCost() << " " << tempNode.h << " " << endl;
        openSet.pop();
        j++;
    }
    */



        /* PROBLEMS

        improve memory mangement
        destroy unused node (the once not added to open list)

        a* om til ida*

    */
/*
    vector<Point> canPos;
    canPos.emplace_back(Point(0,2));
    canPos.emplace_back(Point(1,2));
    vector<Point> goalPos;
    goalPos.emplace_back(Point(2,0));
    goalPos.emplace_back(Point(2,1));
    vector<Point> holePos;
    holePos.emplace_back(Point(2,2));
    holePos.emplace_back(Point(2,3));
    holePos.emplace_back(Point(3,2));
    Point robotPos = Point(1,1);
    string path = "AA";
    string nextStep = "B   ";
    NodeV2 tempNode(robotPos, canPos, goalPos, holePos, path, nextStep);

    Tree newTestTree;
    newTestTree.width = 4;
    newTestTree.height = 4;


    for (int i = 0; i < newTestTree.width; ++i)
    {
        for (int j = 0; j < newTestTree.height; ++j)
        {
            Point destination(i,j);
            cout << "Destination: " << i << "," << j << "  " << newTestTree.isLegalRobotMove(tempNode, destination, path) << " " << path << endl;
        }
    }
*/

    // 0 = EMPTY
    // 1 = ROBOT
    // 2 = CAN
    // 3 = GOAL
    // 4 = INACCESSIBLE

    vector<vector<int>> map = {{0,0,0,3,0},
                              {0,0,0,0,0},
                              {3,0,3,0,3},
                              {0,0,4,0,4},
                              {0,2,3,0,0},
                              {4,4,0,2,0},
                              {4,4,0,4,0},
                              {0,2,2,0,0},
                              {0,2,1,0,0},
                              {0,0,0,4,4}};

    /*
    vector<vector<int>> map = {{0,2,3,3},
                               {0,0,0,0},
                               {2,3,2,0},
                               {0,2,0,0},
                               {3,1,0,0}};
*/
/*    vector<vector<int>> map = {{0,3,0,3},
                               {0,2,4,4},
                               {0,0,4,4},
                               {0,2,4,4},
                               {0,1,4,4}};
*/
/*    vector<vector<int>> map = {{1,0,0},
                               {0,3,0},
                               {0,0,2}};
*/
    //Index: 88546 h: 1 g: 0 Total Cost: 1
    //Index: 88547 h: 1 g: 0 Total Cost: 1
    //Index: 88548 h: 1 g: 0 Total Cost: 1
    //Index: 88549 h: 0 g: 0 Total Cost: 0
    //rrrlluuuudlurrllldddurrrrdddullluuddlddrrr
/*
    vector<vector<int>> map = {{0,0,0,3,4},
                               {2,0,2,0,4},
                               {0,3,0,0,0},
                               {3,2,0,4,0},
                               {0,1,0,2,0},
                               {0,4,4,0,3},
                               {0,2,0,3,0}};
*/
    // DEADLOCK EXAMPLE
/*    vector<vector<int>> map = {{0,0,3,0},
                               {0,2,2,0},
                               {0,2,2,0},
                               {0,0,0,0},
                               {0,1,0,0}};

*/
    // 2018 COMP MAP
    //Index:  6762875  |  h:   0  |  g: 151  |  Cost: 151
    //dlllluuuurrrldruuuuuddddlllddruudlurrrldruuuudrullllrulddurrrdddlllddddruuuudlurrrldruuuudrullrdddlldddrdrrulllrdluuuudlurrrldruuuudruullllrrddurrlurdd
/*    vector<vector<int>> map = {{3,0,0,0,0},
                               {0,0,0,0,0},
                               {3,4,3,0,3},
                               {4,4,4,0,4},
                               {0,2,0,0,0},
                               {0,2,0,0,0},
                               {0,0,4,4,4},
                               {0,2,0,2,1},
                               {0,0,0,0,0},
                               {0,0,0,4,4}};
*/

    //Index: 1546923 h: 1 g: 106 Robot pos: 1,0 number of complted cans: 2
    //Index: 1546924 h: 14 g: 93 Robot pos: 0,6 number of complted cans: 1
    //Index: 1546925 h: 27 g: 80 Robot pos: 1,8 number of complted cans: 0
    //Index: 1546926 h: 30 g: 77 Robot pos: 0,8 number of complted cans: 0
    //lldlluuuurrrldruuuudrullrdddlllddruudlurrrldruuuudruulddulllrulddurrdrrldddllldddruuuudlurrrldruuuuudrullll

    //Index:   177700  |  h:  18  |  g:  88  |  Cost: 106
    //Index:   177701  |  h:   0  |  g: 107  |  Cost: 107
    //lldlluuuurrrldruuuudrullllrulddurrrdddlllddruudlurrrldruuuuuddddlllddddruuuudlurrrldruuuudruullllrrdrrlurdd
/*    vector<vector<int>> map = {{3,0,0,0,0},
                               {0,0,0,0,0},
                               {3,4,0,0,3},
                               {4,4,4,0,4},
                               {0,2,0,0,0},
                               {0,2,0,0,0},
                               {0,0,4,4,4},
                               {0,2,0,0,1},
                               {0,0,0,0,0},
                               {0,0,0,4,4}};
*/
    Tree testTree(map);

    cout << testTree.aStar() << endl;
    std::chrono::seconds endTime = std::chrono::duration_cast< std::chrono::seconds >(std::chrono::system_clock::now().time_since_epoch());
    std::chrono::seconds runTime = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
    cout << runTime.count() << endl;




    /*
    Node node;

    node.path = "rrr";
    node.canIndex = 1;

    cout << testTree.isLegalMove(node) << endl;
    */
    /*
    Node node1;
    Node node2;
    Node node3;
    Node node4;
    Node node5;
    Node node6;
    Node node7;
    Node node8;
    Node node9;

    node1.h = 5; node1.g = 10;
    node2.h = 3; node2.g = 14;
    node3.h = 7; node3.g = 41;
    node4.h = 9; node4.g = 32;
    node5.h = 3; node5.g = 1;
    node6.h = 14; node6.g = 51;
    node7.h = 8; node7.g = 21;
    node8.h = 2; node8.g = 24;
    node9.h = 11; node9.g = 31;

    testTree.openSet.push_back(node1);
    testTree.openSet.push_back(node2);
    testTree.openSet.push_back(node3);
    testTree.openSet.push_back(node4);
    testTree.openSet.push_back(node5);
    testTree.openSet.push_back(node6);
    testTree.openSet.push_back(node7);
    testTree.openSet.push_back(node8);
    testTree.openSet.push_back(node9);

    cout << testTree.openSet[0].getCost() << " " << testTree.openSet[1].getCost() << " " << testTree.openSet[2].getCost() << " " << testTree.openSet[3].getCost() << " " << testTree.openSet[4].getCost() << " " << testTree.openSet[5].getCost() << " " << testTree.openSet[6].getCost() << " " << testTree.openSet[7].getCost() << " " << testTree.openSet[8].getCost() << endl;


    cout << testTree.openSet[0].getCost() << " " << testTree.openSet[1].getCost() << " " << testTree.openSet[2].getCost() << " " << testTree.openSet[3].getCost() << " " << testTree.openSet[4].getCost() << " " << testTree.openSet[5].getCost() << " " << testTree.openSet[6].getCost() << " " << testTree.openSet[7].getCost() << " " << testTree.openSet[8].getCost() << endl;
    */

    return 0;
}