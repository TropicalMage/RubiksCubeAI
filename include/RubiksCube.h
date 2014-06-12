#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H

#include <vector>
#include <string>

using namespace std;

class RubiksCube {
public:
    RubiksCube();
    RubiksCube(const RubiksCube&);
    void rotateFace(char);
    void rotateFace(char, int);
    void reset();
    static char getColor(char);
    static vector<char> getFaces();
    int* getStates();
    int getCornerState(char[8][3], vector<char*>);
    int getEdgeState(char[12][2], vector<char*>);
    bool isSolved();
    bool equals(RubiksCube);

    static const char ORANGE = 0;
    static const char RED = 1;
    static const char WHITE = 2;
    static const char YELLOW = 3;
    static const char BLUE = 4;
    static const char GREEN = 5;

    static char faces[];
    static char sequences[6][4];
    static char cubies[20][3];

    char corners[8][3];
    char edges1[6][2];
    char edges2[6][2];

    static vector<char*> defaultCornerPositions;
    static vector<char*> defaultEdgePositions1;
    static vector<char*> defaultEdgePositions2;

private:
    static void rebuildDefaultCornerPositions();
    static void rebuildDefaultEdgePositions1();
    static void rebuildDefaultEdgePositions2();
    static bool hasFace(char[], int, char);
    static void rotateAroundFace(char[],int, char);
    static bool haveSameElements(char[], char[], int);
    static string arrayToString(char[], int size);
    static string arrayToString(vector<char*>, int);
    int getEdgeOrientation(char[], char[]);
};

#endif // RUBIKSCUBE_H
