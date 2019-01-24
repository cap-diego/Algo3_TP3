#include "System.h"
System::~System(){
    /*for (int i = 0; i < clientes.size(); i++) {
        delete clientes[clientes.size()-1];
        clientes.pop_back();
    }*/
}

System::System(unsigned int n){
    clientes = vector<Customer*>(n);
    depot = 1;
}
void System::setCapacity(unsigned int cap) {//Con esta funcion se guarda la cap max de todos los camiones.
    capacity = cap;
}

void System::addCustomer(unsigned int id, double x, double y) {
    clientes[id-1] = new Customer(id, 0, x, y);
}

void System::setDemand(unsigned int id, unsigned int dem) {
    clientes[id-1]->setDemand(dem);
}

unsigned int System::getCapacity() {
    return capacity;
}

Customer System::getDepot() {
    return *clientes[depot-1];
}

vector<Customer*> System::getCustomers(){
    return clientes;
}

void System::restart() {
    for (Customer* c : clientes) c->unvisit();
}

