#ifndef SIMAN_H
#define SIMAN_H

#include "basics3.hpp"

float calcularCostoCamino(vector<Nodo>& camino);

//Con esto represento a las soluciones
class solucionProb{
private:
    float costoTotal;
    float capacidad;
    vector<vector<Nodo>> caminos;

    void calcularCosto(){
        float calculoTotal = 0;

        for(int i=0; i < caminos.size(); i++){
            calculoTotal += calcularCostoCamino(caminos[i]);
        }

        costoTotal = calculoTotal;
    }

public:

    solucionProb(float costoTotal, float capacidad, vector<vector<Nodo>> caminos){
        this->costoTotal = costoTotal;
        this->capacidad = capacidad;
        this->caminos = caminos;
    }

    solucionProb(float capacidad, vector<vector<Nodo>> caminos){
        this->capacidad = capacidad;
        this->caminos = caminos;
        calcularCosto();
    }

    solucionProb(const solucionProb& sp){
        this->costoTotal = sp.getCostoSol();
        this->capacidad = sp.getCapacidad();
        this->caminos = sp.getCaminos();
    }

    float getCostoSol() const{
        return costoTotal;
    }

    float getCapacidad() const{
        return capacidad;
    }

    vector<vector<Nodo>> getCaminos() const{
        return caminos;
    }

    solucionProb operator=(const solucionProb& sp){
        this->costoTotal = sp.getCostoSol();
        this->capacidad = sp.getCapacidad();
        this->caminos = sp.getCaminos();
        return *this;
    }
};

//solucionProb simulatedAnnealing(solucionProb& solucionInicial, float temperaturaMax, float temperaturaMin, float coefEnfriamiento, int cantPasosMax);

solucionProb simulatedAnnealing(solucionProb& solucionInicial, int cantPasosMax, float temperaturaMax, float coefEnfriamiento);

vector<vector<Nodo>> generarCambios(vector<Nodo>& base);

vector<solucionProb> generarVecindario(solucionProb& base);

bool capacidadAlcanza(float capacidad, vector<Nodo> camino);

#endif // SIMAN_H
