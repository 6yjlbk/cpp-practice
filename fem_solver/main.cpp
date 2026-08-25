#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
using namespace std;

class Node {
public:
    int node_id;
    double x, y, force_x, force_y;
    bool constr_x, constr_y;
    Node (int node_id, double x, double y, double force_x, double force_y, bool constr_x, bool constr_y){
        this->node_id=node_id;
        this->x=x;
        this->y=y;
        this->force_x=force_x;
        this->force_y=force_y;
        this->constr_x=constr_x;
        this->constr_y=constr_y;
    };
};

class Element {
public:
    int elem_id;
    Element(int elem_id){
        this->elem_id=elem_id;
    };
};

class RodElement: public Element {
    public:
    int first_node_id, second_node_id;
    double E,A;
    RodElement(int elem_id, int first_node_id,int second_node_id, double E, double A) : Element(elem_id)
    {
        this->first_node_id=first_node_id;
        this->second_node_id=second_node_id;
        this->E=E;
        this->A=A;
    };
    double calculate_length (const Node& first_node, const Node& second_node) const
    {
        double dx = second_node.x - first_node.x;
        double dy = second_node.y - first_node.y;
        double length = sqrt(dx * dx + dy * dy);
        return length;
    }
};

class Model {
    private:
    vector<Node> nodes;
    vector<RodElement> elements;
    public:
    void addNode(const Node& node)
    {
        nodes.push_back(node);
    };
    void addElement(const RodElement& element)
    {
        elements.push_back(element);
    };

    const Node& getNodeByID(int id) const
    {
        for (const Node& node: this->nodes)
        {
            if (node.node_id == id)
            {  
                return node;
            }
        }
        throw runtime_error("Node not found");
    };

     void print() const
    {
        if (!nodes.empty())
        {
            cout<<"Nodes properties:"<<endl;
            for (const Node& node : nodes)
        {
            cout<<"\nID:"<<node.node_id<<"\nX_coordinate:"<<node.x<<"\nY_coordinate:"<<node.y<<"\nForce_x:"<<node.force_x<<"\nForce_y:"<<node.force_y<<"\nConstrain_x:"<<node.constr_x<<"\nConstrain_y:"<<node.constr_y<<endl;
        }   
        }

        if (!elements.empty())
        {
            cout<<"\nElements properties:"<<endl;
            for (const RodElement& element : elements)
        {
            cout<<"\nID:"<<element.elem_id<<"\nFirst_node:"<<element.first_node_id<<"\nSecond_node:"<<element.second_node_id<<"\nE:"<<element.E<<"\nA:"<<element.A<<endl;
            const Node& first_node = getNodeByID(element.first_node_id);
            const Node& second_node = getNodeByID(element.second_node_id);
            cout<<"\nLength: "<<element.calculate_length(first_node, second_node)<<endl;
        }
        } 
    };

};

int main()
{
    Model model1;
    Node node1 (1,0,0,0,0,1,0);
    Node node2(2,1,0,0,0,0,0);
    Node node3(3,2,0,1000,0,0,0);

    RodElement rod1(1,node1.node_id,node2.node_id,200.0e9,0.001);
    RodElement rod2(2,node2.node_id,node3.node_id,200.0e9,0.001);

    model1.addNode(node1);
    model1.addNode(node2);
    model1.addNode(node3);

    model1.addElement(rod1);
    model1.addElement(rod2);

    model1.print();
    return 0;
}
