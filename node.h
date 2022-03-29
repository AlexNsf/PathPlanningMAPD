#ifndef PATHPLANNINGMAPD_NODE_H
#define PATHPLANNINGMAPD_NODE_H

struct Node {
    int i = 0, j = 0;
    double F = 0, g = 0, H = 0;
    Node* prev = nullptr;

    Node() = default;

    Node(int i_, int j_) : i(i_), j(j_) {}
};

#endif //PATHPLANNINGMAPD_NODE_H
