#include <iostream>
#include "Tree.h"
#include <chrono>

// M = Move forward
// L = turn Left
// R = turn Right
// C = Deliver Can

string pathInterpreter(string path);


int main()
{
    std::chrono::seconds startTime = std::chrono::duration_cast< std::chrono::seconds >(std::chrono::system_clock::now().time_since_epoch());


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
    string path = "dlllluuuurrrldruuuuuddddlllddruudlurrrldruuuudrullllrulddurrrdddlllddddruuuudlurrrldruuuudrullrdddlldddrdrrulllrdluuuudlurrrldruuuudruullllrrddurrlurdd";
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
    Tree solverTree(map);

    cout << pathInterpreter(solverTree.aStar()) << endl;
    std::chrono::seconds endTime = std::chrono::duration_cast< std::chrono::seconds >(std::chrono::system_clock::now().time_since_epoch());
    std::chrono::seconds runTime = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
    cout << runTime.count() << endl;


    return 0;
}

string pathInterpreter(string path)
{
    string moves = "";
    for (int i = 0; i < path.size()-2; ++i)
    {
        if(path[i] == 'u')
        {
            if(path[i+1] == 'u')
            {
                moves.push_back('M');
            }
            else if (path[i+1] == 'd')
            {
                moves.push_back('C');
                if (path[i+2] == 'd')
                {
                    moves.push_back('L');
                    moves.push_back('L');
                }
                else if (path[i+2] == 'l')
                {
                    moves.push_back('L');
                }
                else if (path[i+2] == 'r')
                {
                    moves.push_back('R');
                }
                i++;
            }
            else if (path[i+1] == 'l')
            {
                moves.push_back('M');
                moves.push_back('L');
            }
            else if (path[i+1] == 'r')
            {
                moves.push_back('M');
                moves.push_back('R');
            }
        }
        else if (path[i] == 'd')
        {
            if(path[i+1] == 'u')
            {
                moves.push_back('C');
                if (path[i+2] == 'u')
                {
                    moves.push_back('L');
                    moves.push_back('L');
                }
                else if (path[i+2] == 'l')
                {
                    moves.push_back('R');
                }
                else if (path[i+2] == 'r')
                {
                    moves.push_back('L');
                }
                i++;
            }
            else if (path[i+1] == 'd')
            {
                moves.push_back('M');
            }
            else if (path[i+1] == 'l')
            {
                moves.push_back('M');
                moves.push_back('R');
            }
            else if (path[i+1] == 'r')
            {
                moves.push_back('M');
                moves.push_back('L');
            }
        }
        else if (path[i] == 'l')
        {
            if(path[i+1] == 'u')
            {
                moves.push_back('M');
                moves.push_back('R');
            }
            else if (path[i+1] == 'd')
            {
                moves.push_back('M');
                moves.push_back('L');
            }
            else if (path[i+1] == 'l')
            {
                moves.push_back('M');
            }
            else if (path[i+1] == 'r')
            {
                moves.push_back('C');
                if (path[i+2] == 'r')
                {
                    moves.push_back('R');
                    moves.push_back('R');
                }
                else if (path[i+2] == 'u')
                {
                    moves.push_back('R');
                }
                else if (path[i+2] == 'd')
                {
                    moves.push_back('L');
                }
                i++;
            }
        }
        else if (path[i] == 'r')
        {
            if(path[i+1] == 'u')
            {
                moves.push_back('M');
                moves.push_back('L');
            }
            else if (path[i+1] == 'd')
            {
                moves.push_back('M');
                moves.push_back('R');
            }
            else if (path[i+1] == 'l')
            {
                moves.push_back('C');
                if (path[i+2] == 'l')
                {
                    moves.push_back('R');
                    moves.push_back('R');
                }
                else if (path[i+2] == 'u')
                {
                    moves.push_back('L');
                }
                else if (path[i+2] == 'd')
                {
                    moves.push_back('R');
                }
                i++;
            }
            else if (path[i+1] == 'r')
            {
                moves.push_back('M');
            }
        }
    }
    moves.push_back('M');
    moves.push_back('M');
    return moves;
}