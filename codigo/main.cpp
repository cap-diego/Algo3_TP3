#include <iostream>
#include "cluster.h"
#include <utility>
#include <fstream>
#include <random>

using namespace std;

float distancia_euclidea(float x1,float y1, float x2, float y2);

int main() {

    int cantidad_nodos;
    int capacidad_camion;
    vector<Nodo> vector_nodos; //Tamaño = cantidad_nodos
    float pos_max_x = 0, pos_max_y = 0;
    float demanda_total = 0;
    ifstream fin;
    fin.open("../X-n110-k13.txt");

    if (fin.is_open()) {
        fin >> cantidad_nodos >> capacidad_camion;
        for (int i = 0; i < cantidad_nodos; ++i) {
            Nodo n;
            fin >> (n.indice) >> n.x >> n.y;
            if (pos_max_x < n.x) pos_max_x = n.x;
            if (pos_max_y < n.y) pos_max_y = n.y;
            //cout <<"Guardo el: "<<n.indice<< " X: "<<n.x<< " Y: "<<n.y<<endl;
            vector_nodos.push_back(n);
        }
        for (int j = 0; j < cantidad_nodos; ++j) {
            int indice;

            fin >> indice >> vector_nodos[j].demanda;
            demanda_total += vector_nodos[j].demanda;
        }
    } else
        cout << "Error" << endl;

    cout << "La demanda total es : " << demanda_total << endl << "Y el promedio es: " << demanda_total << " casi:"
         << cantidad_nodos << endl;

    //Aca digo que los k-vecinos deberian ser:
    float cantidad_clientes_puede_visitar_camion = capacidad_camion / (demanda_total / cantidad_nodos);
    //cantDespuesDeComa(cantidad_clientes_puede_visitar_camion,2);
    cout << "Cant clientes que puede visitar: " << cantidad_clientes_puede_visitar_camion << "/"
         << ceil(cantidad_clientes_puede_visitar_camion) << endl;
    int cant_cam = ceil(cantidad_nodos / ceil(cantidad_clientes_puede_visitar_camion)) + 1;//Sumo uno porque el deposito va a tener propio cluster ( y cada cluster sería como un camion)
    cout << "Cantidad de camiones: " << cant_cam << endl;
    //Determinar k crontrides random
    //O(cant clusters * #iteraciones * #instancias * #dim=2)

    vector<Nodo> centroides;
    centroides.resize(cant_cam);
    std::mt19937 rng;
    rng.seed(std::random_device()());
    for (int l = 0; l < cant_cam; ++l) {
        centroides[l].indice = l;
        std::uniform_int_distribution<std::mt19937::result_type> dist6_x(0, pos_max_x);
        centroides[l].x = dist6_x(rng);
        std::uniform_int_distribution<std::mt19937::result_type> dist6_y(0, pos_max_y);
        centroides[l].y = dist6_y(rng);
        cout << "Centroide " << l << " nodo: (x:" << centroides[l].x << " , y:" << centroides[l].y << ")" << endl;
    }

    vector<pair<int, float> > cluster(cantidad_nodos, make_pair(-1, 0)); //indice, demanda
    int centroides_iguales = 0;
    for(int w=0; centroides_iguales!= cant_cam;w++){
        centroides_iguales = 0;
        for (int i = 0; i < cant_cam; ++i) { //Repito por cada cluster
            float dif_pos_x, dif_pos_y;
            pair<float, float> coord;

            for (int j = 0; j < cant_cam; ++j) {
                if (j != i) {
                    dif_pos_x = centroides[i].x - centroides[j].x;
                    dif_pos_y = centroides[i].y - centroides[j].y;
                    if (dif_pos_x > dif_pos_y) {
                        coord.first = dif_pos_x;
                        coord.second = 0;
                    } else {
                        coord.first = 0;
                        coord.second = dif_pos_y;
                    }

                    //Aca asigno a cada cluster los nodos que le corresponden
                    for (int k = 1; k < cantidad_nodos; ++k) { //el k=0 es el deposito
                        if (coord.first == 0 && vector_nodos[k].y >= coord.second) {
                            if (cluster[k].first == -1 ||
                                distancia_euclidea(vector_nodos[k].x, vector_nodos[k].y, centroides[i].x, centroides[i].y) <
                                cluster[k].second) {//Si el nuevo cluster al que puede pertenecer esta mas cerca del que ya pertenece o si no pertenece a ninguno, entonces asigno cluster
                                cluster[k].first = i;//centroides[i].indice; //Centroide al que pertenece
                                cluster[k].second = distancia_euclidea(vector_nodos[k].x, vector_nodos[k].y,
                                                                       centroides[i].x,
                                                                       centroides[i].y);//Distancia al centroide
                            }

                        } else if (coord.first == 0 && vector_nodos[k].y < coord.second) {
                            if (cluster[k].first == -1 ||
                                distancia_euclidea(vector_nodos[k].x, vector_nodos[k].y, centroides[j].x, centroides[j].y) <
                                cluster[k].second) {//Si el nuevo cluster al que puede pertenecer esta mas cerca del que ya pertenece o si no pertenece a ninguno, entonces asigno cluster
                                cluster[k].first = j;//centroides[j].indice; //Centroide al que pertenece
                                cluster[k].second = distancia_euclidea(vector_nodos[k].x, vector_nodos[k].y,
                                                                       centroides[j].x,
                                                                       centroides[j].y);//Distancia al centroide
                            }
                        }
                        if (coord.second == 0 && vector_nodos[k].x >= coord.first) {
                            if (cluster[k].first == -1 ||
                                distancia_euclidea(vector_nodos[k].x, vector_nodos[k].y, centroides[i].x, centroides[i].y) <
                                cluster[k].second) {//Si el nuevo cluster al que puede pertenecer esta mas cerca del que ya pertenece o si no pertenece a ninguno, entonces asigno cluster
                                cluster[k].first = i;//centroides[i].indice; //Centroide al que pertenece
                                cluster[k].second = distancia_euclidea(vector_nodos[k].x, vector_nodos[k].y,
                                                                       centroides[i].x,
                                                                       centroides[i].y);//Distancia al centroide
                            }
                        } else if (coord.second == 0 && vector_nodos[k].x < coord.first) {
                            if (cluster[k].first == -1 ||
                                distancia_euclidea(vector_nodos[k].x, vector_nodos[k].y, centroides[j].x, centroides[j].y) <
                                cluster[k].second) {//Si el nuevo cluster al que puede pertenecer esta mas cerca del que ya pertenece o si no pertenece a ninguno, entonces asigno cluster
                                cluster[k].first = j;//centroides[j].indice; //Centroide al que pertenece
                                cluster[k].second = distancia_euclidea(vector_nodos[k].x, vector_nodos[k].y,
                                                                       centroides[j].x,
                                                                       centroides[j].y);//Distancia al centroide
                            }
                        }
                    }
                }
            }
        }

        //Renuevo los centroides y vuelvo a calcular los clusters
        for (int i = 0; i < cant_cam; ++i) {
            float acum_x = 0, acum_y = 0, avg_x, avg_y;
            int cant_nodos = 0;
            for (int j = 1; j < cantidad_nodos; ++j) {//El nodo 0 no lo considero porque es el deposito
                if (cluster[j].first == i) {
                    acum_x += vector_nodos[j].x;
                    acum_y += vector_nodos[j].y;
                    cant_nodos++;
                }
            }
            avg_x = acum_x / cant_nodos;
            avg_y = acum_y / cant_nodos;
            if(centroides[i].x == avg_x && centroides[i].y == avg_y) {
                centroides_iguales++;
            }else{
                centroides[i].x = avg_x;
                centroides[i].y = avg_y;
                centroides[i].indice = i;
            }
            cout << "Centroide " << centroides[i].indice<<"/"<<i << " nodo: (x:" << centroides[i].x << " , y:" << centroides[i].y << ")" << endl;
        }
    }

    //Ahora me fijo que no exista un cluster que sobrepase de demanda para el camion
    vector<pair<int, float> > demandas_cluster(cant_cam,make_pair(-1,0)); //first=indice cluster, second=demanda
    vector<int> cluster_con_exceso;

    for (int i = 1; i < cant_cam; ++i) { //Empiezo en 1 porque el 0 es el cluter del deposito
        float acum=0;
        for (int j = 1; j < cantidad_nodos; ++j) {
            if(cluster[j].first == i) { //Si el nodo j tiene como cluster al i
                demandas_cluster[i].first = i;
                acum += vector_nodos[j].demanda;
                demandas_cluster[i].second =acum;
            }
        }
        if(demandas_cluster[i].second > capacidad_camion)cluster_con_exceso.push_back(i);
    }

    //Ordeno demandas de clusters de mayor a menor. Empiezo del final y mientras exista cluster con exceso. Saco nodo y lo reorgannizo en alguno de los primeros cluster que pueda.
    /*for (int i = 1; i < demandas_cluster.size(); ++i) {
        for (int j = 1; j < demandas_cluster.size()-1; ++j) {
            if(demandas_cluster[j+1].second > demandas_cluster[j].second){
                float auxval;
                int auxind;
                auxval = demandas_cluster[j].second;
                auxind = demandas_cluster[j].first;
                demandas_cluster[j].second = demandas_cluster[j+1].second;
                demandas_cluster[j].first = demandas_cluster[j+1].first;
                demandas_cluster[j+1].second = auxval;
                demandas_cluster[j+1].first = auxind;
            }
        }
    }*/


    for (int i = 1; i < cant_cam; ++i) {
        cout  <<"Cluster: "<<demandas_cluster[i].first<<"/"<<i<<" demanda: "<< demandas_cluster[i].second<<endl;
    }

    cout <<"Clusters con exceso: "<< cluster_con_exceso.size()<<endl;

    for (int m = 1; m < cantidad_nodos; ++m) {
        for (int n = 0; n < cluster_con_exceso.size(); ++n) {
            if(cluster[m].first == cluster_con_exceso[n]){ //Si nodo actual esta en cluster con exceso lo muevo a otro
                int indice_nuevo_cluster=cluster[m].first, cant_pruebas=0;
                while (indice_nuevo_cluster < demandas_cluster.size() || demandas_cluster[cluster[m].first].second<capacidad_camion){
                    std::uniform_int_distribution<std::mt19937::result_type> ind_aleat(demandas_cluster.size()-1-cluster_con_exceso.size(),demandas_cluster.size()-1);
                    //cout <<"ind generado: "<< indice_nuevo_cluster<<endl;

                    if(demandas_cluster[indice_nuevo_cluster].second + vector_nodos[m].demanda > capacidad_camion){ //Si mover a este cluster no me sirve
                        indice_nuevo_cluster=-1;
                    }else {
                        cout<<"Muevo el nodo "<<m<< " de demanda "<< vector_nodos[m].demanda<< " (cluster "<<cluster[m].first<<"/"<<demandas_cluster[cluster[m].first].first<<") hacia cluster "<<demandas_cluster[indice_nuevo_cluster].first<<endl;
                        demandas_cluster[indice_nuevo_cluster].second+=vector_nodos[m].demanda;
                        demandas_cluster[cluster[m].first].second -= vector_nodos[m].demanda;
                        cluster[m].first = demandas_cluster[indice_nuevo_cluster].first;
                    }
                    indice_nuevo_cluster++;
                }
                if(demandas_cluster[cluster[m].first].second<=capacidad_camion) {
                    cluster_con_exceso[n]=-1;
                }//Ya no tiene exceso
            }
        }
    }

    for (int i = 0; i < demandas_cluster.size(); ++i) {
        cout  <<"Cluster: "<<demandas_cluster[i].first<<" demanda: "<< demandas_cluster[i].second<<endl;
    }

    //EXPORTO SOLUCION PARA HACER TSP
    ofstream fout;
    fout.open("../X-n110-k13.csv");
    for (int i = 0; i < cluster.size(); ++i) {
        fout<< (i+1)<<"," << vector_nodos[i].x<<","<<vector_nodos[i].y<<","<<cluster[i].first<<endl;
    }
    fout.close();
    return 0;
}

float distancia_euclidea(float x1,float y1, float x2, float y2) {
    float dif_x = x1-x2;
    float dif_y = y1-y2;
    float suma_cuads = pow(dif_x,2) + pow(dif_y,2);
    return sqrt(suma_cuads);
}


float distancia(Nodo& n1, Nodo& n2){
    return(sqrt(pow(n1.x-n2.x,2)+ pow(n1.y- n2.y,2)) );
}