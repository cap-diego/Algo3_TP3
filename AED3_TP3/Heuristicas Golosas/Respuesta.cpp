#include "Respuesta.h"

unsigned int Respuesta::cantidadDeCamiones() {
    return (unsigned int) rutas.size();
}
double Respuesta::costoTotal() {
    double pesoCamino = 0;

    for(int k = 0; k < rutas.size(); k++) {//Ciclo para imprimir los caminos
        vector<Customer> r = rutas[k].getRoute();
        pesoCamino+= EuclideanDist(sActual->getDepot(),r[0]);
        pesoCamino+= EuclideanDist(sActual->getDepot(),r[r.size()-1]);
        for (int i = 0; i <r.size()-1 ; ++i) {
            pesoCamino+= EuclideanDist(r[i],r[i+1]);
        }
    }
    return pesoCamino;
}
vector<Route> Respuesta::obtenerRutas() {
    return rutas;
}

void Respuesta::mostrarRutas() {
    for(int k = 0; k < rutas.size(); k++) {//Ciclo para imprimir los caminos
        vector<Customer> r = rutas[k].getRoute();
        cout << "1 ";
        for (int i = 0; i <r.size()-1 ; ++i) {
            cout << r[i].getID() << " ";
        }
        cout << r[r.size()-1].getID() << " 1" << endl;
    }
}

void Respuesta::mostrarRespuesta() {
    cout << cantidadDeCamiones() << endl;
    mostrarRutas();
    cout << costoTotal() << endl;
}