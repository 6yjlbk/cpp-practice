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
        cout<<"Number of packages: "<< num<<"\nID: "<<id<<"\nName: "<<name<<"\nWeight of packages, kg: "<<weight<<"\nStatus: "<<status<<"\nCost: "<< cost() <<endl;
        cout <<"_______________________________________"<<endl;
    };
    int GetID()
    {
        return num;
    };
    virtual double cost() =0;
};

class StandardPackage: public Package{
public:
    using Package::Package;
    double cost() override
    {
        if (weight <0)
        {
            cout <<"Incorrect weight value, pls enter correct num"<<endl;
        }
        else 
        {
            this->weight=weight;
        }
        price=weight*100;
        return price;
    };
};

class ExpressPackage: public Package{
    double time;
public:
    ExpressPackage(const string &name, double weight, double time):Package(name,weight),time(time)
    {
        if (weight <0)
        {
            cout <<"Incorrect weight value, pls enter correct num"<<endl;
        }
        else 
        {
            this->weight=weight;
        }
    }
    double cost() override
    {
        price=weight*100+time*15;
        return price;
    };
};

class FragilePackage: public Package{
    double lvl_fragility;
public:
    FragilePackage(const string &name, double weight, double lvl_fragility):Package(name,weight),lvl_fragility(lvl_fragility)
    {
        if (weight <0)
        {
            cout <<"Incorrect weight value, pls enter correct num"<<endl;
        }
        else 
        {
            this->weight=weight;
        }
    }
    double cost() override
    {
        price=weight*150+lvl_fragility*200;
        return price;
    };
};


int Package::num =0;


int main()
{
    StandardPackage package ("Ann",2.5);
    ExpressPackage package2("Max",123,2.5);
    FragilePackage package3("Bob",5.0,0.8);
    package.PrintInfo();
    package2.PrintInfo();
    package3.PrintInfo();
    package.GetID();
    package2.GetID();
    package3.GetID();
    package.send();
    package2.send();
    package3.send();
    package.deliver();
    package2.deliver();
    package3.deliver();
    package.PrintInfo();
    package2.PrintInfo();
    package3.PrintInfo();
    return 0;
}