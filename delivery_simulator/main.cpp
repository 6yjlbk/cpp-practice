#include <iostream>
using namespace std;

class Package
{
    int id;
    string status, name;
    static int num;
public:
    double weight, price;
    Package(string const  &name, double weight)
    {
        if (weight <0)
        {
            cout <<"Incorrect weight value, pls enter correct num"<<endl;
        }
        else 
        {
            this->weight=weight;
        }
            this->name=name;
            status="Created";
            id=++num;
    }
    void send()
    {
        status="InTransit";
    };
    void deliver()
    {
        status="Delivered";
    };
    void PrintInfo()
    {
        cout<<"Number of packages: "<< num<<"\nID: "<<id<<"\nName: "<<name<<"\nWeight of packages, kg: "<<weight<<"\nStatus: "<<status<<endl;
        cout <<"_______________________________________"<<endl;
    };
    int GetID()
    {
        return num;
    };
    virtual double cost(double weight) =0;
};

class StandardPackage: public Package{
public:
    using Package::Package;
    double cost(double weight) override
    {
        price=weight*100;
    };
};
class ExpressPackage: public Package{
public:
    using Package::Package;
    double cost(double weight) override
    {
        price=weight*180+300;
    };
};
class FragilePackage: public Package{
public:
    using Package::Package;
    double cost(double weight) override
    {
        price=weight*150+200;
    };
};


int Package::num =0;


int main()
{
    StandardPackage package ("Ann",2.5);
    ExpressPackage package2("Max",123);
    package.PrintInfo();
    package2.PrintInfo();
    package.GetID();
    package2.GetID();
    package.send();
    package2.send();
    package.deliver();
    package2.deliver();
    package.PrintInfo();
    package2.PrintInfo();
    return 0;
}