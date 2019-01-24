#include "Customer.h"
Customer::Customer(unsigned int id) {
    ID = id;
    visited = false;
}

Customer::Customer(unsigned int id, unsigned int dem, double x, double y) : ID(id), demand(dem), xCoord(x), yCoord(y){
    visited = false;
}
void Customer::setx(double a){
    xCoord = a;
}

void Customer::sety(double b){
    yCoord = b;
}

void Customer::setDemand(unsigned int a) {
    demand = a;
}

void Customer::setID(unsigned int id) {
    ID = id;
}
double Customer::getx() {
    return xCoord;
}

double Customer::gety() {
    return yCoord;
}

unsigned int Customer::getDemand() {
    return demand;
}

unsigned int Customer::getID() {
    return ID;
}

bool Customer::getstatus(){
    return visited;
}
void Customer::isVisited() {
    visited = true;
}

bool Customer::operator==(Customer a) {
    return getID() == a.getID();
}

bool Customer::isDepot(){
    return getID() == 1;
}

void Customer::unvisit() {
    visited = false;
}
