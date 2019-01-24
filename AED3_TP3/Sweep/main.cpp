#include <iostream>
#include "sweep.hpp"
#include "siman.hpp"
#include "basics3.hpp"
#include <time.h>
#include <fstream>
#include <chrono>
#include <sstream>

using namespace std;

float MAX_X, MAX_Y;



void exportar_grafo(vector<Nodo>& camino, float costo,int ruteo, string in);
float convertir_para_tsp(vector<vector<Nodo>> clusterizacion, string nombre_in, float MAX_X, float MAX_Y);
float convertir_para_tsp2(k_means& clusterizacion, string nombre_in, Nodo depo, float MAX_X, float MAX_Y);

int main()
{
    ListaCordPol lcp;
    vector<Nodo> vn;
    float capacidad;
    vector<vector<Nodo>> clusters;
    string nombreCaso;
    int kOpt;
    float costoOpt;
    string clusteringUsado;
    vector<vector<Nodo>> caminosSol;

    //clusteringUsado = "SWEEP";

    srand(time(NULL));

    auto timeStartSweep = chrono::steady_clock::now();

    cargarDatos(lcp, vn, capacidad, nombreCaso, kOpt, costoOpt);

    cout << "------------------------------" << endl;

    cout << "TAM. DE LA LISTA: " << lcp.tam() << endl;

    cout << "------------------------------" << endl;

    cout << "SE CARGARON TODOS LOS DATOS" << "\n";

    cout << "------------------------------" << "\n";

    //CALCULO EL X e Y MAXIMOS
    float max_x, max_y;
    max_x = vn[0].x;
    max_y = vn[0].y;

    for(int i = 1; i < vn.size(); i++){
        if(vn[i].x > max_x){
            max_x = vn[i].x;
        }
        if(vn[i].y > max_y){
            max_y = vn[i].y;
        }
    }

    //SWEEP
    cout << "GENERANDO CLUSTERS" << "\n";

    clusteringUsado = "SWEEP";

    clusters = generarClusters(lcp, vn, capacidad);

    cout << "------------------------------" << "\n";

    cout << "RESOLVIENDO CAMINOS POR TSP" << "\n";

    //Voy solucionando todos los caminos
    for(int i = 0; i < clusters.size(); i++){
        float costoSol = 0;
        caminosSol.push_back(tsp_con_grasp(clusters[i], 0, costoSol, 1, max_x, max_y));
    }

    auto timeEndSweep = chrono::steady_clock::now();

    //Acá calculo lo que tardó en resolverlo el sweep
    double timeSweep = chrono::duration<double, milli>(timeEndSweep - timeStartSweep).count();

    cout << "------------------------------" << endl;

    //system("pause");

    cout << "------------------------------" << endl;

    cout << "MOSTRANDO CAMINOS" << endl;

    for(int i = 0; i < caminosSol.size(); i++) {
        cout << "CAMINO " << i << ": [";
        for (int j = 0; j < caminosSol[i].size(); j++) {
            cout << caminosSol[i][j].indice << ((j == caminosSol[i].size() - 1)? ("]") : (", "));
        }
        cout << endl;
    }

    cout << "------------------------------" << endl;
    cout << "------------------------------" << endl;

    cout << "FIN DEL PROGRAMA" << endl;

    cout << "------------------------------" << endl;

    return 0;
}

float convertir_para_tsp(vector<vector<Nodo>> clusterizacion, string nombre_in, float MAX_X, float MAX_Y){
    float costo_viaje = 0;
    float ctsp2=0;
    vector<vector<Nodo> > mat(clusterizacion.size());

    for (int i = 0; i < clusterizacion.size(); ++i) {
        vector<Nodo> nodos(clusterizacion[i].size() - 1);

        for(int j=0; j<nodos.size(); j++){
            nodos[j] = clusterizacion[i][j];
        }

        ctsp2 += calcularCostoCamino(clusterizacion[i]);
        exportar_grafo(clusterizacion[i], costo_viaje, i, nombre_in);
    }
    cout <<"Costo total: "<<ctsp2<<endl;
    return ctsp2;
}

float convertir_para_tsp2(k_means& clusterizacion, string nombre_in, Nodo depo, float MAX_X, float MAX_Y){
    float costo_viaje = 0;
    float ctsp2=0;
    int cant_rutas=0;
    for (int i = 0; i < clusterizacion.cant_clusters(); ++i) {
        vector<Nodo> nodos = clusterizacion.getClusterIesimo(i).getNodos();
        vector<Nodo> nodos_con_dep(nodos.size()+1); //le sumo para agregar el deposito
        nodos_con_dep[0] = depo;
        for (int j = 1; j < nodos_con_dep.size(); ++j) {
            nodos_con_dep[j] = nodos[j-1];
        }
        costo_viaje = 0;
        vector<Nodo> camino;
        if(nodos_con_dep.size()!=1) {
            cant_rutas++;
            camino = tsp_con_grasp(nodos_con_dep,0,costo_viaje,1,MAX_X, MAX_Y);
        }
        ctsp2+=costo_viaje;
        if(nodos_con_dep.size()!=1) exportar_grafo(camino,costo_viaje,i,nombre_in);
    }
    return ctsp2;
}

void exportar_grafo(vector<Nodo>& camino, float costo, int ruteo, string in){
    ofstream fout;
    std::ostringstream ss;

    ss << ruteo;

    auto s = ss.str();

    fout.open("../salida/" + in + "/"+ in + "-ruteo" + s + ".csv");
    for (int i = 0; i < camino.size()-1; ++i) {
        fout << camino[i].indice<<","<<camino[i].x<<","<< camino[i].y<<endl;//indice,posx,posy
    }
    for (int j = 0; j < camino.size()-1; ++j) {
        fout << camino[j].indice<<","<<camino[j+1].indice<<endl;//indice_nodo_a,indice_nodo_b
    }
    fout<<costo;
    fout.close();
}