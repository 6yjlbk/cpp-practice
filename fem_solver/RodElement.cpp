#include <vector>
#include <cmath>
#include <stdexcept>
#include "Node.h"
#include "Element.h"
#include "RodElement.h"


    RodElement::RodElement(int elem_id, int first_node_id,int second_node_id, double E, double A) : Element(elem_id)
    {
        this->first_node_id=first_node_id;
        this->second_node_id=second_node_id;
        this->E=E;
        this->A=A;
    };
    double RodElement::calculate_length (const Node& first_node, const Node& second_node) const
    {
        double dx = second_node.x - first_node.x;
        double dy = second_node.y - first_node.y;
        double length = sqrt(dx * dx + dy * dy);
        return length;
    };
    std::vector<std::vector<double>> RodElement::calculate_k_l (const Node& first_node, const Node& second_node) const
    {
        double length=calculate_length(first_node, second_node);
        if (length <= 0)
        {
            throw std::runtime_error("Rod element has zero length");
        }
        std::vector<std::vector<double>> k_l(2,std::vector<double>(2));
        for (int i=0; i<=1; i++)
        {
            for (int j=0; j<=1; j++)
            {
                k_l[i][j]=pow(-1,i+j)*E*A/length;
            }
        }
        return k_l;
    }
    double RodElement::calculate_extension(double first_displacement, double second_displacement) const
    {
        return second_displacement-first_displacement;
    }


    RodElement::RodResults RodElement::calculate_results(const Node& first_node, const Node& second_node, double first_displacement, double second_displacement) const
    {
        RodResults results;
        results.length = calculate_length(first_node, second_node);
        if (results.length <= 0.0)
        {
            throw std::runtime_error("Rod element has zero length");
        }
        results.extension=calculate_extension(first_displacement, second_displacement);
        results.strain = results.extension/results.length;
        results.stress=results.strain*E;
        results.normal_force=results.stress*A;
        return results;
    }