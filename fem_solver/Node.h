#pragma once
class Node {
public:
    int node_id;
    double x, y, force_x, force_y;
    bool constr_x, constr_y;
    Node (int node_id, double x, double y, double force_x, double force_y, bool constr_x, bool constr_y);
};