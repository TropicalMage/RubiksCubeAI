#include "HeuristicCreator.h"
#include <iostream>
using namespace std;

const int num_corner_states = 88179840;
const int num_edge_states = 42577920;
char HeuristicCreator::cornerStates[num_corner_states];
char HeuristicCreator::firstEdgeGroupStates[num_edge_states];
char HeuristicCreator::secondEdgeGroupStates[num_edge_states];
queue<Node*> frontier;
RubiksCube* HeuristicCreator::cube;

HeuristicCreator::HeuristicCreator() {
    for (int i = 0; i < num_corner_states; i++) {
        cornerStates[i] = -1;
    }
    for (int i = 0; i < num_edge_states; i++) {
        firstEdgeGroupStates[i] = -1;
    }
    for (int i = 0; i < num_edge_states; i++) {
        secondEdgeGroupStates[i] = -1;
    }
}

void HeuristicCreator::createHeuristicTables(RubiksCube* cube) {
    this->cube = cube;
    Node *node = new Node();
    node->parent = NULL;
    node->action = -1;
    explore(node);
}

void HeuristicCreator::printSecondEdgeGroup(std::string filename) {
    ofstream file;
    file.open (filename);
    for (int i = 0; i < num_edge_states; i++) {
        file << (int) cornerStates[i] << '\n';
    }
    file.close();
}

void HeuristicCreator::printFirstEdgeGroup(std::string filename) {
    ofstream file;
    file.open (filename);
    for (int i = 0; i < num_edge_states; i++) {
        file << (int) firstEdgeGroupStates[i] << '\n';
    }
    file.close();
}

void HeuristicCreator::printCornerGroup(std::string filename) {
    ofstream file;
    file.open (filename);
    for (int i = 0; i < num_corner_states; i++) {
        file << (int) secondEdgeGroupStates[i] << '\n';
    }
    file.close();
}

//helpers
void HeuristicCreator::explore(Node *node) {
    if (node->getDepth() < MAX_DEPTH) {
        for (char face : RubiksCube::getFaces()) {
            for (int i = 0; i < 3; i++) {
                Node *child = new Node();
                child->parent = node;
                child->action = encode(face, i+1);
                cube->reset();
                applyMovements(cube, child);
                bool wasAdded = addToTable(child);
                if (wasAdded) {
                    frontier.push(child);
                } else {
                    delete child;
                }
            }
        }
        explore(frontier.front());
    }
}
int counter = 0;
bool HeuristicCreator::addToTable(Node *node) {
    bool added = false;
    int depth = node->getDepth();
    int* states = cube->getStates();
    if (cornerStates[states[0]] == 0) {
        cornerStates[states[0]] = depth;
        added = true;
    }

    if (firstEdgeGroupStates[states[1]] == 0) {
        firstEdgeGroupStates[states[1]] = depth;
        added = true;
    }

    if (secondEdgeGroupStates[states[2]] == 0) {
        secondEdgeGroupStates[states[2]] = depth;
        added = true;
    }
    if(depth > counter) {
        counter = depth;
        //cout << depth;
    }
    return added;
}

char HeuristicCreator::encode(char color, char rotations) {
    return (char) ((color * 3) + (rotations - 1));
}

vector<char> HeuristicCreator::decode(char action) {
    char color = (char) (action / 3);
    int rotations = (action % 3) + 1;

    vector<char> actions(rotations);
    for (int i = 0; i < actions.size(); i++) {
        actions[i] = (char) color;
    }

    return actions;
}

void HeuristicCreator::applyMovements(RubiksCube* cube, Node *node) {
    Node* traverser = node;
    while(traverser->parent != NULL) {
        for(char face : decode(traverser->action)) {
            cube->rotateFace(face);
        }
        traverser = traverser->parent;
    }
}

int Node::getDepth() {
    int i = 0;
    Node* traverser = this;
    while(traverser->parent != NULL) {
        i++;
        traverser = traverser->parent;
    }
    return i;
}
