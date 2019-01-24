#include "../include/sweep.hpp"

#include <fstream>
#include <sstream>

#define LEYENDO_PUNTOS 0
#define DEFINIENDO_DEMANDAS 1
#define DEFINIENDO_PUNTO_CENTRAL 2

void ListaAdyacencias::convertirAListaInc(ListaIncidencia &l) {
        Arista ar;
        list<AristaAd>::iterator it;
        for (int i = 0; i < nodos.size(); ++i) {
            it = rep[i].begin();
            ar.desde = nodos[i];
            while(it!=rep[i].end()){
                ar.peso = (*it).peso;
                ar.hasta = (*it).adyacente;
                l.agregarArista(ar);
                it++;
            }
        }
}

void MatrizAdyacencias::convertirAListaInc(ListaIncidencia& li) {
    Arista ar;
    int cantAg=0;
    for (int i = 0; i < rep.size(); ++i) {
        for (int j = 0; j < rep[0].size(); ++j) {
            if(rep[i][j] !=-1) {//si hay arista que los une
                ar.peso=rep[i][j];
                ar.desde = nodos[i];

                ar.hasta = nodos[j];
                ar.indice=cantAg++;
                li.agregarArista(ar);
            }
        }
    }
}

void MatrizAdyacencias::convertirAListaAdy(ListaAdyacencias& la) {
    Arista ar;
    int cantAg=0;
    for (int i = 0; i < (rep.size()); ++i) {
        for (int j = 0; j < rep[0].size(); ++j) {
            if(rep[i][j] !=-1) {//si hay arista que los une
                ar.peso=rep[i][j];
                ar.desde = nodos[i];
                ar.hasta = nodos[j];
                ar.indice=cantAg++;
                la.agregarArista(ar);
            }
        }
    }
}

//////////////////

void cargarDatos(ListaCordPol& lcp, vector<Nodo>& vn, float& capacidad, string& nombreCaso, int& kopt, float& costoOpt){
    //Hay que leer el archivo y llenar las estructuras
    ifstream entrada;
    entrada.open("inputCPP.txt");

    //Obtenemos el nombre del caso, la cantidad optima de camiones y la solucion optima
    entrada >> nombreCaso;
    entrada >> kopt;
    entrada >> costoOpt;

    int tam = 0;
    entrada >> tam;
    vn = vector<Nodo>(tam);

    entrada >> capacidad;

    std::string line;
    int estado = LEYENDO_PUNTOS; //0 - ingreso puntos, 1 - ingreso demandas, 2 - ingreso punto central
    int cont = 0;
    bool seteadoNodoCentral = false;

    while(std::getline(entrada, line)){
        //Esto es para asegurar que la primera linea leida no sea vacia

        if(line.compare("") == 0)
            continue;

        if(line.compare("Dem") == 0 || cont > tam){
            estado = DEFINIENDO_DEMANDAS;
            cont = 0;
            continue;
        }

        if(line.compare("Dep") == 0 || cont > tam){
            estado = DEFINIENDO_PUNTO_CENTRAL;
            cont = 0;
            continue;
        }

        cont++;

        switch(estado){
        case LEYENDO_PUNTOS: {
            std::stringstream ss(line);

            int id;
            float x;
            float y;

            ss >> id >> x >> y;

            vn[id - 1] = Nodo(x, y, id);

            } break;

        case DEFINIENDO_DEMANDAS:{
            std::stringstream ss(line);

            int id;
            float demanda;

            ss >> id >> demanda;

            vn[id - 1].demanda = demanda;

            } break;

        case DEFINIENDO_PUNTO_CENTRAL:{
            //ESTO SOLO DEBERIA EJECUTARSE UNA VEZ
            if(!seteadoNodoCentral){
                std::stringstream ss(line);

                int id;

                ss >> id;

                if(id != -1){
                    lcp.setearNodoBase(vn[id - 1]);
                }

                seteadoNodoCentral = true;
            }
            } break;
        }
    }

    entrada.close();

    if(!seteadoNodoCentral){
        lcp.setearNodoBase(vn[0]);
    }

    //Ya esta cargado el vector
    //Ahora cargo la lista Coord. Polares
    for(int i = 0; i < vn.size(); i++){
        //Agrego todos los puntos menos el de depósito, que sabemos que siempre va a estar en el camino
        if(i + 1 != lcp.getNodoBase().indice){
            lcp.agregarNodo(vn[i]);
        }
    }

    cout << "TERMINO LA CARGA DE DATOS" << endl;
    //Debería estar cargada la lista
}

//Ahora hacemos el sweep
vector<vector<Nodo>> generarClusters(ListaCordPol& lcp, const vector<Nodo>& vn, float capacidad){
    vector<vector<Nodo>> vecClusters;

    float capActual = 0;
    vector<Nodo> vecActual;
    vecActual.push_back(lcp.getNodoBase());

    while(!lcp.vacia()){
        if(capActual + lcp.siguiente().demanda <= capacidad){
            capActual += lcp.siguiente().demanda;
            vecActual.push_back(vn[lcp.siguiente().id - 1]);
            lcp.pop();
        }
        else{
            //Inserto el conjunto y reseteo las variables
            capActual = 0;
            vecClusters.push_back(vecActual);

            vecActual.clear();
            vecActual.push_back(lcp.getNodoBase());
        }

    }

    //Agrego el ultimo cluster que siempre se forma
    vecClusters.push_back(vecActual);

    return vecClusters;
}

//TSP

vector<Nodo> tsp2(vector<Nodo>& nodos, int nodo_comienzo, float &costo_viaje, float p, float MAX_X, float MAX_Y){
    vector<Nodo> camino;
    //contiene los nodos del cluster
    vector<vector<float> > distancias_entre_nodos;
    vector<bool> visitados(nodos.size(),false);
    distancias_entre_nodos.resize(nodos.size());

    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> proba_p(0,1);

    for (int i = 0; i < nodos.size(); i++) {
        distancias_entre_nodos[i].resize(nodos.size());
        for (int j = 0; j < nodos.size(); ++j) {
            if(i!=j)
                distancias_entre_nodos[i][j] = distancia_euclidea(nodos[i].x,nodos[i].y,nodos[j].x,nodos[j].y);
            else
                distancias_entre_nodos[i][j] = MAX_Y*MAX_X;
        }
    }

    bool hay_nodos = true;
    int act = nodo_comienzo;
    int posible_nodo = -1;
    camino.push_back(nodos[nodo_comienzo]);
    while(hay_nodos){
        visitados[act] = true;
        float minDist = MAX_X*MAX_Y;
        int posMin = -1;
        posible_nodo = -1;
        for (int i = 0; i < nodos.size(); ++i) {
            if(i!= act && visitados[i]==false){
                posible_nodo = i;
            }
            if( i!= act && visitados[i]==false && minDist > distancias_entre_nodos[act][i]){
                minDist = distancias_entre_nodos[act][i];
                posMin = i;
            }
        }
        if(proba_p(rng) > p && posible_nodo!=-1) { //si ademas de tener que agarrar un vertice aleat se que existe (podrian estar todos visitados)
            posMin = posible_nodo;
            minDist = distancias_entre_nodos[act][posible_nodo];
        }
        if(posible_nodo == -1) { //Significa que no hay mas que visitar
            hay_nodos= false;
            costo_viaje += distancias_entre_nodos[act][nodo_comienzo];
            //cout <<"Agrego trayecto de "<< nodos[act].indice<<" a "<< nodos[0].indice<<endl;
            camino.push_back(nodos[nodo_comienzo]);
        }else{
            //cout <<"Agrego trayecto de "<< nodos[act].indice<<" a "<< nodos[posMin].indice<<endl;
            camino.push_back(nodos[posMin]);
            costo_viaje+=minDist;
            act = posMin;
        }
    }
    return camino;
}

vector<Nodo> tsp_con_grasp(vector<Nodo>& nodos, int nodo_comienzo, float &costo_viaje, float p, float MAX_X, float MAX_Y) {
    //Genero solucion con heuristica golosa
    float costo_sol_golosa=0;
    vector<Nodo> sol = tsp2(nodos,nodo_comienzo,costo_sol_golosa,1, MAX_X, MAX_Y); //conseguimos la primera solucion de forma golosa
    vector<Nodo> mejor_so, mejor_sol = sol;
    float mejor_costo = costo_sol_golosa;
    float shift = 0.1;
    while(p>=1) {//O(1)
        //Aplico busqueda lineal a nuevas soluciones
        for (int i = 1; i < sol.size() - 2; ++i) {//O(TAMCLUSTER_i * TAMCLUSTER_i * opt_swap(TAM_CLUSTER_i)) = O(TAMCLUSTER_i * TAMCLUSTER_i * TAMCLUSTER_i) < O(n^3)

            float costo_nueva_sol = 0;
            for (int j = i + 1; j < sol.size()-1; ++j) {
                costo_nueva_sol=0;
                vector<Nodo> aux = opt_swap2(sol,i,j,costo_nueva_sol);
                if(costo_nueva_sol<mejor_costo){
                    mejor_sol=aux;
                    mejor_costo=costo_nueva_sol;
                }
            }


        }
        p-=shift;
        costo_sol_golosa = 0;
        sol = tsp2(nodos,nodo_comienzo,costo_sol_golosa,p,MAX_X, MAX_Y);
        if(costo_sol_golosa<mejor_costo){
            mejor_costo=costo_sol_golosa;
            sol = mejor_sol;
        }
    }
    costo_viaje = mejor_costo;
    return mejor_sol;
}

//

vector<Nodo> opt_swap2(vector<Nodo>& solucion_actual, int tope1, int tope2, float& costo_solucion){//O(TAMCLUSTER_i)

    costo_solucion = 0;
    for (int i = tope2; i <= tope1; i--) {//le saco las puntas que son los
        Nodo aux = solucion_actual[tope1+abs(tope2-i)];
        solucion_actual[tope1+abs(tope2-i)]=solucion_actual[i];
        solucion_actual[i] = aux;
    }
    for (int i = 0; i < solucion_actual.size(); ++i) {
        costo_solucion += distancia_euclidea(solucion_actual[i].x, solucion_actual[i].y, solucion_actual[i + 1].x, solucion_actual[i + 1].y);
    }

    return solucion_actual;

}

//

void generarOutput(const vector<vector<Nodo>>& clusters, string nombre, Nodo depo){
    int cantClusters = 1;

    ofstream salida;
    salida.open("../salida/" + nombre + "/" + nombre + ".csv");

    salida << depo.indice << ",";
    salida << depo.x << ",";
    salida << depo.y << ",";
    salida << clusters.size() + 1 << endl;

    for(int i = 0; i < clusters.size(); i++){
        for(int j = 0 ; j < clusters[i].size(); j++){
            if(!(depo == clusters[i][j])){
                salida << clusters[i][j].indice << ",";
                salida << clusters[i][j].x << ",";
                salida << clusters[i][j].y << ",";
                salida << cantClusters << endl;
            }
        }
        cantClusters++;
    }

    salida.close();
}
