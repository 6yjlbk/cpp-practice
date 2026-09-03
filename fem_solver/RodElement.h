#pragma once

#include <vector>
#include "Node.h"
#include "Element.h"

class RodElement: public Element 
{
    public:
    struct RodResults {double length; double extension; double strain; double stress; double normal_force;};
    int first_node_id, second_node_id;
    double E,A;
    RodElement(int elem_id, int first_node_id,int second_node_id, double E, double A);
    double calculate_length (const Node& first_node, const Node& second_node) const;
    std::vector<std::vector<double>> calculate_k_l (const Node& first_node, const Node& second_node) const;
    double calculate_extension(double first_displacement, double second_displacement) const;
    RodResults calculate_results(const Node& first_node, const Node& second_node, double first_displacement, double second_displacement) const;
};
