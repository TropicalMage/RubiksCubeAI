#include <iostream>
#include "RubiksCube.h"
#include "HeuristicCreator.h"
using namespace std;

void createHeuristicFiles();
int main()
{
    createHeuristicFiles();
    return 0;
}

void createHeuristicFiles() {
    RubiksCube* cube = new RubiksCube();
    HeuristicCreator creator;
    creator.createHeuristicTables(cube);
    creator.printCornerGroup("corners.txt");
    creator.printFirstEdgeGroup("edges1.txt");
    creator.printSecondEdgeGroup("edges2.txt");
}
