#pragma once
#include <vector>
using namespace std;

vector<vector<double>> lk();

vector<double> solveLinearSystem(vector<vector<double>> A, vector<double> b);

vector<double> FE(int nelx, int nely, const vector<vector<double>>& x, double penal);

vector<vector<double>> check(int nelx, int nely, double rmin, const vector<vector<double>>& x, const vector<vector<double>>& dc);

vector<vector<double>> OC(int nelx,int nely,const vector<vector<double>>& x,int volfrac, const vector<vector<double>>& dc);