#ifndef PATHPLANNINGMAPD_NODE_H
#define PATHPLANNINGMAPD_NODE_H

struct Node {
    int i = 0, j = 0;
    double F = 0, g = 0, H = 0;
    Node* prev = nullptr;
    int64_t ts_opened = -1;

    Node() = default;

    Node(int i_, int j_) : i(i_), j(j_) {}
};

struct NodeComparator {
    bool operator()(const Node* node_1, const Node* node_2) const {
        return std::tie(node_1->F, node_1->i, node_1->j) < std::tie(node_2->F, node_2->i, node_2->j);
    }
};

#endif //PATHPLANNINGMAPD_NODE_H
