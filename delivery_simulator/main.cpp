#include <iostream>
#include <vector>
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
            throw invalid_argument(
            "Weight must be greater than zero"
        );
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
        return id;
    };
    virtual double cost() =0;
};

class StandardPackage: public Package{
public:
    using Package::Package;
    double cost() override
    {
        price=weight*100;
    };
};

class ExpressPackage: public Package{
    double time;
public:
    ExpressPackage(const string &name, double weight, double time):Package(name,weight),time(time)
    {
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
    }
    double cost() override
    {
        price=weight*150+lvl_fragility*200;
        return price;
    };
};


class Courier{
    public:
    virtual bool canDeliver (const Package& package)  = 0;
    virtual double calculateAward (const Package& package)  = 0;
    virtual string getType() = 0;
};

class WalkingCourier: public Courier{
    using Courier::Courier;
    bool canDeliver (const Package& package) override
        {
            if (package.weight <5)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    double calculateAward (const Package& package) override
    {
        return package.weight*0.1;
    }

};
class BikeCourier: public Courier{
    using Courier::Courier;
    bool canDeliver (const Package& package) override
        {
            if (package.weight <15)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    double calculateAward (const Package& package) override
    {
        return package.price*0.2;
    }
    
};
class CarCourier: public Courier{
    using Courier::Courier;
    bool canDeliver (const Package& package)
    {
        return true;
    }
    double calculateAward (const Package& package) override
    {
        return package.price*0.3;
    }
    
};

class DeliveryService:public Package, Courier{
    Package* package =nullptr;
    Courier* courier =nullptr;
    public:
    void AddPack(Package& package){
        this -> package=&package;
    };
    void RegPack() {

    };
    void FindPack() {

    };
    void SetCourier(Courier& courier) {
        this -> courier=&courier;
    };
    void GetAllDel() {

    };
    double CalcCost() {

    };
    double CalcAward() {

    };
    void PrintPackage (){
        if (package != nullptr)
        {
            package->PrintInfo();
        }

    }
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