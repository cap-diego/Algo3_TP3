
#ifndef CLARK_WRIGHT_RESPUESTA_H
#define CLARK_WRIGHT_RESPUESTA_H

#include <iostream>
#include "System.h"
using namespace std;
class Respuesta {
private:
    System* sActual;
    vector<Route> rutas;
public:
    Respuesta(vector<Route>& a, System* s) : rutas(a), sActual(s){};
    unsigned int cantidadDeCamiones();
    double costoTotal();
    vector<Route> obtenerRutas();
    void mostrarRutas();
    void mostrarRespuesta();
};


#endif //CLARK_WRIGHT_RESPUESTA_H
