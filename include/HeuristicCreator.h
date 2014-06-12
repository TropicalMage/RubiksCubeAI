#ifndef HEURISTICCREATOR_H
#define HEURISTICCREATOR_H
#include "RubiksCube.h"
#include <queue>
#include <fstream>

struct Node {
    Node* parent;
    char action;
    int getDepth();
};
class HeuristicCreator {
public:
    HeuristicCreator();
    void createHeuristicTables(RubiksCube* cube);
    void printCornerGroup(std::string filename);
    void printFirstEdgeGroup(std::string filename);
    void printSecondEdgeGroup(std::string filename);

private:
    static char cornerStates[];
    static char firstEdgeGroupStates[];
    static char secondEdgeGroupStates[];

    static const int MAX_DEPTH = 11;
    static RubiksCube* cube;
    void explore(Node *node);
    void applyMovements(RubiksCube*, Node*);
    static char encode(char, char);
    static vector<char> decode(char action);
    bool addToTable(Node*);
};

#endif // HEURISTICCREATOR_H
