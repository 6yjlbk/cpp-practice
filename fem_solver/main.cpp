#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <utility>
#include "Node.h"
#include "Element.h"
#include "RodElement.h"
using namespace std;

/*
cd D:\Git\cpp-practice\fem_solver
g++ -std=c++20 -Wall -Wextra -Wpedantic -g main.cpp Node.cpp Element.cpp -o fem_solver.exe
.\fem_solver.exe
*/ 


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

    void apply_bc(vector<vector<double>>& K, vector<double>& F) const
    {
        for (size_t i=0; i<nodes.size();i++)
        {
            if (nodes[i].constr_x == true)
            {  
                F[i]=0;
                for (size_t j=0;j<nodes.size();j++)
                {
                    K[i][j]=0;
                    K[j][i]=0;
                }
                K[i][i]=1;
            }
        }
    }

     vector<double> matrix_solver (vector<vector<double>> K,vector<double> F) const // Gauss
    {
        size_t system_size=F.size();
        if (system_size == 0)
        {
            throw runtime_error ("System is empty");
        }
        if (K.size() != system_size)
        {
            throw runtime_error("Matrix and vector don't match");
        }
        for (const vector<double>& row : K)
        {
            if (row.size() != system_size)
            {
                throw runtime_error("Matrix is not a sqaure");
            }
        }
        
        for (size_t pivot=0; pivot <system_size; pivot++)
        {
            size_t pivot_row = pivot;
            double maximum_value = abs(K[pivot][pivot]);

            for (size_t row=pivot +1; row< system_size; row++)
            {
                double candidate=abs(K[row][pivot]);
                if (candidate > maximum_value)
                {
                    maximum_value=candidate;
                    pivot_row=row;
                }
            }
            if (maximum_value < 1e-12)
            {
                throw runtime_error("Stiffness matrix is singular");
            }
            if (pivot_row != pivot)
            {
                swap(K[pivot], K[pivot_row]);
                swap(F[pivot], F[pivot_row]);
            }

            for (size_t row = pivot + 1; row < system_size; row++)
            {
                double factor = K[row][pivot] / K[pivot][pivot];

                K[row][pivot] = 0.0;

                for (size_t column = pivot + 1; column < system_size; column++)
                {
                    K[row][column] = K[row][column] - factor * K[pivot][column];
                }

                F[row] = F[row] - factor * F[pivot];
            }
        }
        vector<double> U(system_size);
        for (size_t reverse_index = system_size; reverse_index > 0; reverse_index --)
        {
            size_t row = reverse_index -1;
            double right_side = F[row];
            for (size_t column = row +1; column < system_size; column++)
            {
                right_side = right_side - K[row][column]*U[column];
            }
            U[row] = right_side / K[row][row];
        }
        return U;
    }


    vector<double> calculate_R (const vector<vector<double>>& K, const vector<double>& U, const vector<double>& F) const
    {
        vector<double> R (nodes.size());
        for (size_t i=0; i<nodes.size();i++)
        {
            double internal_force=0;
            for (size_t j=0; j<nodes.size();j++)
            {
                internal_force+=K[i][j]*U[j];
            }
            R[i]=internal_force-F[i];
        }
        return R;
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
            vector<vector<double>> K_original=calculate_K();
            vector<double> F_original=calculate_F();
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
            apply_bc(K,F);
            vector<double> U=matrix_solver(K,F);
            cout <<"Global stiffness matrix after bc:" <<endl;
            for (const vector<double>& row : K)
            {
                for (double value : row)
                {
                    cout<<value<<" ";   
                }
                cout <<endl;
            }
            cout<<"Force vector after bc:"<<endl;
            for (const double value: F)
            {
                cout<<value<<" ";  
            }
            cout<<endl;
            cout <<"Node displacements:"<<endl;
            for (size_t i=0;i<U.size();i++)
            {
                cout<<"Node"<<nodes[i].node_id<<":"<<U[i]<<endl;
            }
            vector<double> R =calculate_R(K_original,U,F_original);
            cout <<"Node reactions:"<<endl;
            for (size_t i=0;i<U.size();i++)
            {
                cout<<"Node"<<nodes[i].node_id<<":"<<R[i]<<endl;
            }
            cout<<"Element Results:"<<endl;
            for (const RodElement& element : elements)
            {
                size_t first_index=getNodeIndexByID(element.first_node_id);
                size_t second_index=getNodeIndexByID(element.second_node_id);
                const Node& first_node = getNodeByID(element.first_node_id);
                const Node& second_node = getNodeByID(element.second_node_id);
                double first_displacement=U[first_index];
                double second_displacement=U[second_index];
                RodElement::RodResults results=element.calculate_results(first_node,second_node,first_displacement,second_displacement);
                cout<<"Element ID: "<<element.elem_id<<endl<<"Length: "<<results.length<<endl<<"Extension: "<<results.extension<<endl<<"Strain: "<<results.strain<<endl<<"Stress: "<<results.stress<<endl<<"Normal force: "<<results.normal_force<<endl;
            }
        }
    }

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
