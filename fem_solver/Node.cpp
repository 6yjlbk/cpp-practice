#include "Node.h"

Node::Node(int node_id, double x, double y, double force_x, double force_y, bool constr_x, bool constr_y)
{
    this->node_id=node_id;
    this->x=x;
    this->y=y;
    this->force_x=force_x;
    this->force_y=force_y;
    this->constr_x=constr_x;
    this->constr_y=constr_y;
}