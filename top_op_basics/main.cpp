#include <iostream>
#include <vector>
#include <Eigen/Sparse>
#include <math.h>
using namespace Eigen;
using namespace std;

vector<vector<double>> lk()
{
    double E=1;
    double nu=0.3;
    vector<double> k ={1/2-nu/6, 1/8+nu/8, -1/4-nu/12, -1/8+3*nu/8, -1/4+nu/12, -1/8-nu/8, nu/6, 1/8-3*nu/8};
    vector<vector<double>> KE = {
        {k[0], k[1], k[2], k[3], k[4], k[5], k[6], k[7]},
        {k[1], k[0], k[7], k[6], k[5], k[4], k[3], k[2]},
        {k[2], k[7], k[0], k[5], k[6], k[3], k[4], k[1]},
        {k[3], k[6], k[5], k[0], k[7], k[2], k[1], k[4]},
        {k[4], k[5], k[6], k[7], k[0], k[1], k[2], k[3]},
        {k[5], k[4], k[3], k[2], k[1], k[0], k[7], k[6]},
        {k[6], k[3], k[4], k[1], k[2], k[7], k[0], k[5]},
        {k[7], k[2], k[1], k[4], k[3], k[6], k[5], k[0]}
    };
    return KE;
}

double FE(int nelx, int nely, const vector<vector<double>>& x, double penal)
{
    int ndof = 2* (nelx+1)* (nely+1);
    vector<vector<double>> KE=lk();

    SparseMatrix<double> K(ndof, ndof);
    SparseMatrix<double> F(ndof, 1);
    SparseMatrix<double> U(ndof, 1);

    vector<Triplet<double>> triplets;
    
    for (int ely=1; ely<=nely; ely++)
    {
        for (int elx=1; elx<=nelx; elx++)
        {
            int n1 = (nely+1)*(elx-1)+ely;
            int n2 = (nely+1)*elx+ely;
            vector<int> edof ={2*n1-1, 2*n1, 2*n2-1, 2*n2, 2*n2+1, 2*n2+2, 2*n1+1, 2*n1+2};

            for (int i = 0; i < 8; i++) 
            {
                for (int j = 0; j < 8; j++) 
                {
                    triplets.push_back(
                        Triplet<double>(edof[i] - 1, edof[j] - 1, pow(x[ely - 1][elx - 1], penal) * KE[i][j]));
                }
            }
        }
    }
    return 0;
}

int main()
{
    int nelx = 30;
    int nely = 10;
    double volfrac = 0.5;
    double penal = 3.0;
    double rmin = 1.5;
    
    vector<vector<double>> x(nely, vector<double>(nelx,volfrac));
    vector<vector<double>> xold(nely, vector<double>(nelx,volfrac));
    int loop=0;
    double change=1.0;
    // Start iteration
    while (change > 0.01)
    {
        loop++;
    }

    return 0;
}