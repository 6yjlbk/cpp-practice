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
    vector<vector<double>> calculate_k_l (const Node& first_node, const Node& second_node) const
    {
        double length=calculate_length(first_node, second_node);
        if (length <= 0)
        {
            throw runtime_error("Rod element has zero length");
        }
        vector<vector<double>> k_l(2,vector<double>(2));
        for (int i=0; i<=1; i++)
        {
            for (int j=0; j<=1; j++)
            {
                k_l[i][j]=pow(-1,i+j)*E*A/length;
            }
        }
        return k_l;
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
        for (const Node& node: nodes)
        {
            if (node.node_id == id)
            {  
                return node;
            }
        }
        throw runtime_error("Node not found");
    };

    size_t getNodeIndexByID (int id) const
    {
        for (size_t i=0; i<nodes.size(); i++)
        {
            if (nodes[i].node_id == id)
            {  
                return i;
            }
        }
        throw runtime_error("Node index not found");
    }

    vector<vector<double>> calculate_K() const
    {
        size_t matrix_size=nodes.size();
        vector<vector<double>> K(matrix_size,vector<double>(matrix_size));
        for (const RodElement& element: elements){
            const Node& first_node=getNodeByID(element.first_node_id);
            const Node& second_node=getNodeByID(element.second_node_id);
            vector<vector<double>> k_l=element.calculate_k_l(first_node,second_node);
            size_t first_index = getNodeIndexByID(element.first_node_id);
            size_t second_index = getNodeIndexByID(element.second_node_id);
            K[first_index][first_index]+=k_l[0][0];
            K[first_index][second_index]+=k_l[0][1];
            K[second_index][first_index]+=k_l[1][0];
            K[second_index][second_index]+=k_l[1][1];
        }

        return K;
    }

    vector<double> calculate_F() const
    {
        
        size_t vector_size=nodes.size();
        vector<double> F(vector_size);
        for (size_t i=0; i<vector_size; i++)
        {
            F[i]=nodes[i].force_x;
        }
        return F;
    }

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
            vector<vector<double>> k_l =element.calculate_k_l(first_node, second_node);
            cout <<"Local stiffness matrix:" <<endl;
            for (const vector<double>& row : k_l)
            {
                for (double value : row)
                {
                    cout<<value<<" ";   
                }
                cout <<endl;
            }
        }
            vector<vector<double>> K =calculate_K();
            cout <<"Global stiffness matrix:" <<endl;
            for (const vector<double>& row : K)
            {
                for (double value : row)
                {
                    cout<<value<<" ";   
                }
                cout <<endl;
            }
            vector<double> F=calculate_F();
            cout<<"Force vector:"<<endl;
            for (const double value: F)
            {
                cout<<value<<" ";  
            }
            cout<<endl;

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
