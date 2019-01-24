
#ifndef CLARK_WRIGHT_ROUTE_H
#define CLARK_WRIGHT_ROUTE_H


#include "Customer.h"
#include <vector>
using namespace std;

class Route {
private:
    unsigned actualOcuppied;
    vector<Customer> clientesVisitados;

public:
    Route();
    unsigned int getActualWeight();
    void addCustomer(Customer a);
    Customer lastDelivery();
    vector<Customer> getRoute();
    Customer firstDelivery();
};


#endif //CLARK_WRIGHT_ROUTE_H
