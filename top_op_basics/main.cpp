#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include "topopt.h"

using namespace std;

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
    double dc=0.0;
    int maxIter = 100;
    // Start iteration
    while (change > 0.01 && loop<=maxIter)
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
                vector<double> Ue(8);
                for (int i =0; i<8; i++)
                {
                    Ue[i]=U[edof[i]-1];
                }
                double UeKEUe=0.0;
                for (int i = 0; i < 8; i++) 
                {
                for (int j = 0; j < 8; j++)
                {
                    UeKEUe += Ue[i] * KE[i][j] * Ue[j];
                }
                }
                double xval = x[ely - 1][elx - 1];
                c += pow(xval, penal) * UeKEUe;
                dc[ely - 1][elx - 1] = -penal * pow(xval, penal - 1.0) * UeKEUe;
            }
         }
        dc =check(nelx,nely,rmin,x,dc);
        x=OC(nelx,nely,x,volfrac,dc);

        change = 0.0;
        double sumX = 0.0;

        for (int ely = 0; ely < nely; ely++)
        {
            for (int elx = 0; elx < nelx; elx++)
            {
                change = max(change, abs(x[ely][elx] - xold[ely][elx]));
                sumX += x[ely][elx];
            }
        }

        double volume = sumX / (nelx * nely);

        cout << " It.: " << setw(4) << loop
            << " Obj.: " << setw(10) << fixed << setprecision(4) << c
            << " Vol.: " << setw(6) << fixed << setprecision(3) << volume
            << " ch.: " << setw(6) << fixed << setprecision(3) << change
            << endl;
    }

    return 0;
}