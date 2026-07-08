#include <iostream>
#include <vector>
#include <Eigen/SparseLU>
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

vector<double> solveLinearSystem(vector<vector<double>> A, vector<double> b)
{
    int n = b.size();

    for (int k = 0; k < n; k++) {
        int pivot = k;

        for (int i = k + 1; i < n; i++) {
            if (abs(A[i][k]) > abs(A[pivot][k])) {
                pivot = i;
            }
        }

        swap(A[k], A[pivot]);
        swap(b[k], b[pivot]);

        for (int i = k + 1; i < n; i++) {
            double factor = A[i][k] / A[k][k];

            for (int j = k; j < n; j++) {
                A[i][j] -= factor * A[k][j];
            }

            b[i] -= factor * b[k];
        }
    }

    vector<double> x(n, 0.0);

    for (int i = n - 1; i >= 0; i--) {
        double sum = b[i];

        for (int j = i + 1; j < n; j++) {
            sum -= A[i][j] * x[j];
        }

        x[i] = sum / A[i][i];
    }

    return x;
}

vector<double> FE(int nelx, int nely, const vector<vector<double>>& x, double penal)
{
    int ndof = 2 * (nelx + 1) * (nely + 1);

    vector<vector<double>> KE = lk();

    vector<vector<double>> K(ndof, vector<double>(ndof, 0.0));
    vector<double> F(ndof, 0.0);
    vector<double> U(ndof, 0.0);

    for (int ely = 1; ely <= nely; ely++) {
        for (int elx = 1; elx <= nelx; elx++) {
            int n1 = (nely + 1) * (elx - 1) + ely;
            int n2 = (nely + 1) * elx + ely;

            vector<int> edof = {2*n1-1, 2*n1, 2*n2-1, 2*n2, 2*n2+1, 2*n2+2, 2*n1+1, 2*n1+2};

            double factor = pow(x[ely - 1][elx - 1], penal);

            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    K[edof[i] - 1][edof[j] - 1] += factor * KE[i][j];
                }
            }
        }
    }

    F[1] = -1.0;

    vector<int> fixeddofs;

    for (int dof = 0; dof <= 2 * (nely + 1) - 1; dof += 2) {
        fixeddofs.push_back(dof);
    }

    fixeddofs.push_back(ndof - 1);

    sort(fixeddofs.begin(), fixeddofs.end());
    fixeddofs.erase(unique(fixeddofs.begin(), fixeddofs.end()), fixeddofs.end());

    vector<int> freedofs;

    for (int dof = 0; dof < ndof; dof++) {
        if (!binary_search(fixeddofs.begin(), fixeddofs.end(), dof)) {
            freedofs.push_back(dof);
        }
    }

    int nfree = freedofs.size();

    vector<vector<double>> Kff(nfree, vector<double>(nfree, 0.0));
    vector<double> Ff(nfree, 0.0);

    for (int i = 0; i < nfree; i++) {
        Ff[i] = F[freedofs[i]];

        for (int j = 0; j < nfree; j++) {
            Kff[i][j] = K[freedofs[i]][freedofs[j]];
        }
    }

    vector<double> Uf = solveLinearSystem(Kff, Ff);

    for (int i = 0; i < nfree; i++) {
        U[freedofs[i]] = Uf[i];
    }

    for (int i = 0; i < fixeddofs.size(); i++) {
        U[fixeddofs[i]] = 0.0;
    }

    return U;
}

double check(int nelx, int nely, double rmin, const vector<vector<double>>& x)
{

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
        xold=x;
        vector <double> U=FE(nelx, nely, x, penal);
        vector<vector<double>> KE =lk();
        vector<vector<double>> dc(nely, vector<double>(nelx, 0.0));
         double c=0.;
         for (int ely=1;ely<=nely;ely++)
         {
            for (int elx=1; elx<=nelx;elx++)
            {
                int n1=(nely+1)*(elx-1)+ely;
                int n2=(nely+1)*elx + ely;
                vector<int> edof = {2*n1-1, 2*n1, 2*n2-1, 2*n2, 2*n2+1, 2*n2+2, 2*n1+1, 2*n1+2};
                vector<double> Ue = U{2*n1-1, 2*n1, 2*n2-1, 2*n2, 2*n2+1, 2*n2+2, 2*n1+1, 2*n1+2};
                double c = pow(x(ely,elx),penal-1)*Ue'*KE*Ue;
            }
         }
    }

    return 0;
}