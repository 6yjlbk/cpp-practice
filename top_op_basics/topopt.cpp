#include <cmath>
#include "topopt.h"
#include <algorithm>

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

vector<vector<double>> check(int nelx, int nely, double rmin, const vector<vector<double>>& x, const vector<vector<double>>& dc)
{
    vector<vector<double>> dcn(nely, vector<double>(nelx, 0.0));
    int r = static_cast<int>(round(rmin));

    for (int i = 1; i <= nelx; i++)
    {
        for (int j = 1; j <= nely; j++)
        {
            double sum = 0.0;

            for (int k = max(i - r, 1); k <= min(i + r, nelx); k++)
            {
                for (int l = max(j - r, 1); l <= min(j + r, nely); l++)
                {
                    double fac = rmin - sqrt(pow(i - k, 2) + pow(j - l, 2));
                    double weight = max(0.0, fac);

                    sum += weight;
                    dcn[j - 1][i - 1] += weight * x[l - 1][k - 1] * dc[l - 1][k - 1];
                }
            }

            dcn[j - 1][i - 1] /= x[j - 1][i - 1] * sum;
        }
    }

    return dcn;
}

vector<vector<double>> OC(int nelx,int nely,const vector<vector<double>>& x,int volfrac, const vector<vector<double>>& dc)
{
    vector<vector<double>> xnew(nely, vector<double>(nelx, 0.0));
    double l1=0; double l2=10000; double move =0.2;
    while ((l2-l1)>1e-4)
    {
        double lmid = 0.5 * (l2 + l1);
        double sumXnew = 0.0;
        for (int ely = 0; ely < nely; ely++)
        {
            for (int elx = 0; elx < nelx; elx++)
            {
                double value = x[ely][elx] * sqrt(-dc[ely][elx] / lmid);

                value = min(x[ely][elx] + move, value);
                value = min(1.0, value);
                value = max(x[ely][elx] - move, value);
                value = max(0.001, value);

                xnew[ely][elx] = value;
                sumXnew += value;
            }
        }

        if (sumXnew - volfrac * nelx * nely > 0.0)
        {
            l1 = lmid;
        }
        else
        {
            l2 = lmid;
        } 
    }
    return xnew;
}