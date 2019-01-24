#ifndef CLARK_WRIGHT_SYSTEM_H
#define CLARK_WRIGHT_SYSTEM_H
#include <cmath>
#include <vector>
#include <scoped_allocator>
#include "Customer.h"
#include "Route.h"
using namespace std;

struct savingEntreAB{
    Customer* A;
    Customer* B;
    double saving;
    savingEntreAB(Customer* A_, Customer* B_,double saving_) : A(A_),B(B_),saving(saving_){}
    Customer* to(){
        return B;
    }
    Customer* from(){
        return A;
    }
};
class System {
private:
    int depot;
    unsigned int capacity;
    vector<Customer*> clientes;
public:
    ~System();
    System(unsigned int n);
    void setCapacity(unsigned int cap);
    void addCustomer(unsigned int id, double x, double y);
    void setDemand(unsigned int id, unsigned int dem);
    unsigned int getCapacity();
    Customer getDepot();
    vector<Customer*> getCustomers();
    void restart();

};

static double EuclideanDist(Customer A, Customer B) {
    double xDif = pow(A.getx() - B.getx(),2);
    double yDif = pow(A.gety() - B.gety(),2);
    double res = sqrt(xDif+yDif);
    return res;
}

#endif //CLARK_WRIGHT_SYSTEM_H
