#include "Route.h"
Route::Route(){
    actualOcuppied = 0;
}

void Route::addCustomer(Customer a) {
    actualOcuppied+= a.getDemand();
    clientesVisitados.push_back(a);
}

Customer Route::lastDelivery() {
    return clientesVisitados[clientesVisitados.size()-1];
}


vector<Customer> Route::getRoute() {
    return clientesVisitados;
}
unsigned int Route::getActualWeight() {
    return actualOcuppied;
}

Customer Route::firstDelivery(){
    return clientesVisitados[0];
}