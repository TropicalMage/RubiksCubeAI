#include "RubiksCube.h"
#include <vector>

char RubiksCube::faces[] = {ORANGE, RED, WHITE, YELLOW, BLUE, GREEN};
char RubiksCube::sequences[6][4] = {
    {WHITE, GREEN, YELLOW, BLUE},
    {BLUE, YELLOW, GREEN, WHITE},
    {BLUE, RED, GREEN, ORANGE},
    {ORANGE, GREEN, RED, BLUE},
    {YELLOW, RED, WHITE, ORANGE},
    {ORANGE, WHITE, RED, YELLOW}
};
char RubiksCube::cubies[20][3] = {
    {ORANGE, WHITE, BLUE},
    {ORANGE, WHITE, GREEN},
    {ORANGE, YELLOW, GREEN},
    {ORANGE, YELLOW, BLUE},
    {RED, WHITE, BLUE},
    {RED, WHITE, GREEN},
    {RED, YELLOW, GREEN},
    {RED, YELLOW, BLUE},
    {ORANGE, WHITE},
    {ORANGE, GREEN},
    {ORANGE, YELLOW},
    {ORANGE, BLUE},
    {WHITE, BLUE},
    {WHITE, GREEN},
    {YELLOW, GREEN},
    {YELLOW, BLUE},
    {RED, WHITE},
    {RED, GREEN},
    {RED, YELLOW},
    {RED, BLUE}
};


const int num_corners = 8;
const int num_edges = 12;

vector<char*> RubiksCube::defaultCornerPositions;
vector<char*> RubiksCube::defaultEdgePositions1;
vector<char*> RubiksCube::defaultEdgePositions2;

RubiksCube::RubiksCube() {
    defaultCornerPositions = vector<char*>();
    defaultEdgePositions1 = vector<char*>();
    defaultEdgePositions2 = vector<char*>();

    for(int i = 0; i < num_corners; i++) {
        for(int j = 0; j < 3; j++) {
            corners[i][j] = cubies[i][j];
        }
    }
    for (int i = 0; i < num_edges / 2; i++) {
        for(int j = 0; j < num_edges / 2; j++) {
            edges1[i][j] = cubies[num_corners + i][j];
        }
    }
    for (int i = 0; i < num_edges / 2; i++) {
        for(int j = 0; j < num_edges / 2; j++) {
            edges2[i][j] = cubies[num_corners + num_edges / 2 + i][j];
        }
    }
}

RubiksCube::RubiksCube(const RubiksCube& cube) {
    defaultCornerPositions = vector<char*>();
    defaultEdgePositions1 = vector<char*>();
    defaultEdgePositions2 = vector<char*>();

    for(int i = 0; i < num_corners; i++) {
        for(int j = 0; j < 3; j++) {
            corners[i][j] = cube.corners[i][j];
        }
    }
    for (int i = 0; i < num_edges / 2; i++) {
        for(int j = 0; j < num_edges / 2; j++) {
            edges1[i][j] = cube.edges1[num_corners + i][j];
        }
    }
    for (int i = 0; i < num_edges / 2; i++) {
        for(int j = 0; j < num_edges / 2; j++) {
            edges2[i][j] = cube.edges2[num_corners + num_edges / 2 + i][j];
        }
    }
}

void RubiksCube::rotateFace(char face) {
    for (int i = 0; i < num_corners; i++) {
        if (hasFace(corners[i], 3, face)) {
            rotateAroundFace(corners[i], 3, face);
        }
    }
    for (int i = 0; i < num_edges/2; i++) {
        if (hasFace(edges1[i], 2, face)) {
            rotateAroundFace(edges1[i], 2, face);
        }
    }
    for (int i = 0; i < num_edges/2; i++) {
        if (hasFace(edges2[i], 2, face)) {
            rotateAroundFace(edges2[i], 2, face);
        }
    }
}

void RubiksCube::rotateFace(char color, int rotations) {
    for (int i = 0; i < rotations; i++) {
        rotateFace(color);
    }
}

char RubiksCube::getColor(char color) {
    switch (color) {
    case YELLOW:
        return 'Y';
    case ORANGE:
        return 'O';
    case WHITE:
        return 'W';
    case RED:
        return 'R';
    case BLUE:
        return 'B';
    case GREEN:
        return 'G';
    default:
        return 0x0;
    }
}

int* RubiksCube::getStates() {
    int states[3];

    rebuildDefaultCornerPositions();
    states[0] = getCornerState(corners, defaultCornerPositions);
    rebuildDefaultEdgePositions1();
    states[1] = getEdgeState(edges1, defaultEdgePositions1);
    rebuildDefaultEdgePositions2();
    states[2] = getEdgeState(edges2, defaultEdgePositions2);
    return states;
}

int RubiksCube::getCornerState(char positions[8][3], vector<char*> defaults) {
    int state = 0;
    int base = 1;
    for (int i = 0; i < num_corners - 1; i++) {
        char* position = positions[i];
        int index = 0;
        while(!haveSameElements(defaults.at(index), position, defaults.size())) {
            index++;
        }
        state += ((index * 3) + (position[0] / 2)) * base;
        base *= defaults.size() * 3;
        defaults.erase(defaults.begin() + index);
    }
    return state;
}

void RubiksCube::reset() {
    for(int i = 0; i < num_corners; i++) {
        for(int j = 0; j < 3; j++) {
            corners[i][j] = cubies[i][j];
        }
    }
    for (int i = 0; i < num_edges / 2; i++) {
        for(int j = 0; j < num_edges / 2; j++) {
            edges1[i][j] = cubies[num_corners + i][j];
        }
    }
    for (int i = 0; i < num_edges / 2; i++) {
        for(int j = 0; j < num_edges / 2; j++) {
            edges2[i][j] = cubies[num_corners + num_edges / 2 + i][j];
        }
    }
}

int RubiksCube::getEdgeState(char positions[12][2], vector<char*> defaults) {
    int state = 0;
    int base = 1;
    for (int i = 0; i < num_edges; i++) {
        char* position = positions[i];
        int index = 0;
        while(!haveSameElements(defaults.at(index), position, defaults.size())) {
            index++;
        }
        int orientation = getEdgeOrientation(defaults.at(index), position);
        state += ((index * 2) + orientation) * base;
        base *= defaults.size() * 2;
        defaults.erase(defaults.begin() + index);
    }
    return state;
}

void RubiksCube::rebuildDefaultCornerPositions() {
    vector<char*>::iterator it;
    for ( it = defaultCornerPositions.begin(); it != defaultCornerPositions.end(); ) {
        delete *it;
        it = defaultCornerPositions.erase(it);
    }
    for (int i = 0; i < num_corners; i++) {
        char* position = new char[3];
        for(int j = 0; j < 3; j++) {
            position[j] = cubies[i][j];
        }
        defaultCornerPositions.push_back(position);
    }
}

void RubiksCube::rebuildDefaultEdgePositions1() {
    vector<char*>::iterator it;
    for ( it = defaultEdgePositions1.begin(); it != defaultEdgePositions1.end(); ) {
        delete *it;
        it = defaultEdgePositions1.erase(it);
    }
    for (int i = 0; i < num_edges; i++) {
        char* position = new char[2];
        for(int j = 0; j < 2; j++) {
            position[j] = cubies[num_corners + i][j];
        }
        defaultEdgePositions1.push_back(position);
    }
}

void RubiksCube::rebuildDefaultEdgePositions2() {
    vector<char*>::iterator it;
    for ( it = defaultEdgePositions2.begin(); it != defaultEdgePositions2.end(); ) {
        delete *it;
        it = defaultEdgePositions2.erase(it);
    }
    for (int i = 0; i < num_edges/2; i++) {
        char* position = new char[3];
        for(int j = 0; j < 2; j++) {
            position[j] = cubies[num_corners + num_edges/2 + i][j];
        }
        defaultEdgePositions2.push_back(position);
    }

    for (int i = 0; i < num_edges/2; i++) {
        char* position = new char[3];
        for(int j = 0; j < 2; j++) {
            position[j] = cubies[num_corners + i][j];
        }
        defaultEdgePositions2.push_back(position);
    }
}

bool RubiksCube::hasFace(char position[], int sides, char face) {
    for (int i = 0; i < sides; i++) {
        if (position[i] == face) {
            return true;
        }
    }
    return false;
}

void RubiksCube::rotateAroundFace(char* position, int sides, char face) {
    for (int i = 0; i < sides; i++) {
        if (position[i] != face) {
            int j = 0;
            while (j < 4 && position[i] != sequences[face][j]) {
                j++;
            }
            position[i] = sequences[face][((j + 1) % 4)];
        }
    }
}

bool RubiksCube::haveSameElements(char a1[], char a2[], int size) {
    for (int i = 0; i < size; i++) {
        bool found = false;
        for (int j = 0; j < size; j++) {
            if (a1[i] == a2[j]) {
                found = true;
            }
        }
        if (!found) {
            return false;
        }
    }
    return true;
}

std::string RubiksCube::arrayToString(char array[], int size) {
    std::string s = "";

    for (int i = 0; i < size; i++) {
        s += getColor(array[i]);
    }

    return s;
}

std::string RubiksCube::arrayToString(vector<char*> array, int innerSize) {
    std::string s = "";

    for (int i = 0; i < innerSize; i++) {
        s += arrayToString(array[i], innerSize) + " ";
    }

    return s;
}

bool RubiksCube::isSolved() {
    int* states = getStates();
    for(int i = 0; i < 3; i++) {
        if(states[i] > 0 ) {
            return false;
        }
    }
    return true;
}

bool RubiksCube::equals(RubiksCube other) {
    for(int i = 0; i < num_corners; i++) {
        for(int j = 0; j < 3; j++) {
            if(cubies[i][j] != other.cubies[i][j]) return false;
        }
    }
    for(int i = 0; i < num_edges; i++) {
        for(int j = 0; j < 2; j++) {
            if(cubies[num_corners + i][j] != other.cubies[num_corners + i][j]) return false;
        }
    }
    return true;;
}
vector<char> RubiksCube::getFaces() {
    vector<char> returnedFaces = vector<char>();
    for(int i = 0; i < 6; i++) {
        returnedFaces.push_back(faces[i]);
    }
    return returnedFaces;
}

int RubiksCube::getEdgeOrientation(char defaultPosition[], char position[]) {
    for(int i = 0; i < 2; i++) {
        if(defaultPosition[i] == position[0]) {
            return i;
        }
    }
    return -1;
}

