#include "../include/siman.hpp"
#include <time.h>
#include <math.h>

float calcularCostoCamino(vector<Nodo>& camino){
    float calculoCamino = 0;

    //calcula el costo del camino entre nodos
    for(int j=0; j < camino.size() - 1;j++){
        calculoCamino += distancia_euclidea(camino[j].x, camino[j].y, camino[j+1].x, camino[j+1].y);
    }

    return calculoCamino;
}

//solucionProb simulatedAnnealing(solucionProb& solucionInicial, float temperaturaMax, float temperaturaMin, float coefEnfriamiento, int cantPasosMax)
solucionProb simulatedAnnealing(solucionProb& solucionInicial, int cantPasosMax, float temperaturaMax, float coefEnfriamiento){
    float tempActual = temperaturaMax;
    solucionProb solFinal = solucionInicial;
    solucionProb solActual = solucionInicial;

    //Elige al azar un indice
    std::random_device seed;
    std::mt19937 engine(seed());

    int cantPasos = 0;
    while(cantPasos < cantPasosMax){

        cout << "TEMP ACTUAL: " << tempActual << "\n";

        vector<solucionProb> vecindario = generarVecindario(solActual);

        std::uniform_int_distribution<int> choose(0, vecindario.size()-1);

        int index = choose(engine);

        solucionProb solTentativa = vecindario[index];

        //Calculo la probabilidad de pasar a esta solucion
        float r = (float) rand()/RAND_MAX;

        float prob = -1;

        float energiaEstadoActual = solActual.getCostoSol();
        float energiaNuevoEstado = solTentativa.getCostoSol();

        float deltaE = energiaEstadoActual - energiaNuevoEstado;

        cout << "COSTO ESTADO ACTUAL: " << energiaEstadoActual << "\n";
        cout << "COSTO ESTADO NUEVO: " << energiaNuevoEstado << "\n";
        cout << "DELTA E: " << deltaE << "\n";
        cout << "PASO: " << cantPasos << "\n";

        if(energiaNuevoEstado < energiaEstadoActual){
            solActual = solTentativa;
        }
        else{
            prob = exp(((deltaE) / tempActual));
            cout << "Probabilidad: " << prob << "\n";
        }

        if(prob - 0.5 >= r){
            solActual = solTentativa;
        }

        //Guardamos la mejor
        if(solActual.getCostoSol() < solFinal.getCostoSol()){
            float costoFinal = solFinal.getCostoSol();
            float costoNuevo = solActual.getCostoSol();

            solFinal = solActual;

            cout << "SE REDUJO EL COSTO DE LA SOLUCION DE " << costoFinal << " A " << costoNuevo <<"\n";
        }

        tempActual *= coefEnfriamiento;

        cantPasos++;

        cout << "----------------------------------------" << "\n";
    }

    return solFinal;
}

vector<vector<Nodo>> generarCambios(vector<Nodo>& base){
    vector<vector<Nodo>> sol;
    for(int i=1; i < base.size() - 2; i++){
        vector<Nodo> nuevo = base;
        nuevo[i] = base[i+1];
        nuevo[i+1] = base[i];
        sol.push_back(nuevo);
    }
    return sol;
}

vector<solucionProb> generarVecindario(solucionProb& base){
    vector<solucionProb> sol;

    for(int i=0; i < base.getCaminos().size(); i++){
        vector<vector<Nodo>> variaciones = generarCambios(base.getCaminos()[i]);
        vector<vector<Nodo>> caminos = base.getCaminos();

        for(int j=0; j < variaciones.size(); j++){
            caminos[i] = variaciones[j];
            sol.push_back(solucionProb(base.getCapacidad(), caminos));
        }
    }

    for (int i = 0; i < base.getCaminos().size()-1; ++i) {
        vector<vector<Nodo>> nueva = base.getCaminos();
        Nodo n = nueva[i][nueva[i].size()-2];
        nueva[i][nueva[i].size()-2] = nueva[i+1][1];
        nueva[i+1][1] = n;
        if (capacidadAlcanza(base.getCapacidad(), nueva[i]) && capacidadAlcanza(base.getCapacidad(), nueva[i+1]))
            sol.push_back(solucionProb(base.getCapacidad(), nueva));
    }

    return sol;
}

bool capacidadAlcanza(float capacidad, vector<Nodo> camino){
    float demandaTotal = 0;

    for(int i=0; i < camino.size(); i++){
        demandaTotal += camino[i].demanda;
    }

    return (capacidad >= demandaTotal);
}
