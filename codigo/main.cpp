#include <iostream>
#include "cluster.h"
#include <utility>
#include <fstream>
#include <queue>
#include <algorithm>
#include <tuple>
#include <random>
#include <ctime>
#include <set>

using namespace std;
class k_means;
float aux_tsp(const vector<Nodo>&, int pos);
float tsp(const vector<vector<float> >& dist_entre_nodos, int pos, long long int visitados, vector<vector<float>>& costos, const vector<Nodo>& nodos);

vector<Nodo> tsp2(vector<Nodo>& nodos, int nodo_comienzo, float &costo_viaje, float p);
vector<Nodo> tsp_con_grasp(vector<Nodo>& nodos, int nodo_comienzo, float &costo_viaje, float p);
float convertir_para_tsp(k_means& clusterizacion, string nombre_in);


int MAX_X, MAX_Y;
Nodo deposito;

class cluster{
public:

    cluster(){
        x = 0;
        y = 0;
        demanda_total = 0;
        carga_actual = 0;
        cant_nodos = 0;
    }
    void setCantidadNodos(int cn){
        nodos.clear();
        nodos.resize(cn);
        Nodo n;
        for (int i = 0; i < nodos.size(); ++i) {
            nodos[i] = n;
        }
    }
    cluster(const cluster& cl) : nodos(cl.nodos) {    }

    void operator=(const cluster& cl){
        this->x=cl.x;
        this->y =cl.y;
        this->nodos = (cl.nodos);
        this->deposito = cl.deposito;
        this->demanda_total = cl.demanda_total;
        this->cant_nodos = cl.cant_nodos;
        this->carga_actual = cl.carga_actual;

    }
    int getCantNodos(){
        return cant_nodos;
    }
    float getDemandaTotal(){
        return demanda_total;
    }
    float getCargaActual(){
        return carga_actual;
    }
    void setDeposito(Nodo& dep) {
        this->deposito = dep;
    }
    void setDemanda(float d) {
        this->demanda_total = d;
    }
    float getX(){
        return x;
    }
    float getY(){
        return y;
    }
    void setX(float x){
        this->x=x;
    }
    void setY(float y){
        this->y=y;
    }
    float getAvgX(){
        return (acum_pos_x_nodos/cant_nodos);
    }
    float getAvgY(){
        return (acum_pos_y_nodos/cant_nodos);
    }
    Nodo& dame_nodo(int& desde) {
        while (nodos[desde].indice==-1) {
            desde++;
        }
        return nodos[desde];
    }

    int nodo_mas_lejano(float demanda){ //devuelvo el nodo que esta más alejado del centro del cluster y que tiene demanda igual o mayor a la del nodo que quiero agregar
        float maxDist = 0, dem;
        int posMax = -1;

        for (int i = 2; i < nodos.size(); ++i) {
            if(nodos[i].indice!=-1&& nodos[i].demanda <= demanda && maxDist < distancia_de_nodo_al_centro(nodos[i])) {
                maxDist = distancia_de_nodo_al_centro(nodos[i]);
                posMax = i;
            }
        }
        if(posMax==-1){
            maxDist = 0;
            for (int i = 2; i <nodos.size(); ++i) {
                if(nodos[i].indice!=-1 && maxDist < distancia_de_nodo_al_centro(nodos[i])) {
                    maxDist = distancia_de_nodo_al_centro(nodos[i]);
                    posMax = i;
                }
            }
        }
        return posMax;
    }


    int nodo_mas_lejano() {

        float maxDist = 0, dem;
        int posMax = -1;

        for (int i = 2; i < nodos.size(); ++i) {
            if(nodos[i].indice!=-1 && maxDist < distancia_de_nodo_al_centro(nodos[i])) {
                maxDist = distancia_de_nodo_al_centro(nodos[i]);
                posMax = i;
            }
        }
        return posMax;
    }

    void agregar_nodo(Nodo &n){
        if(nodos[n.indice].indice == -1){
            nodos[n.indice] = n;
            cant_nodos++;
            carga_actual+=n.demanda;
            acum_pos_x_nodos+= n.x;
            acum_pos_y_nodos+=n.y;
        }
    }
    bool tiene_exceso(){
        return ((demanda_total < carga_actual)? true : false);
    }
    void eliminar_nodo(Nodo &n){
        if(nodos[n.indice].indice!=-1){
            carga_actual-=n.demanda;
            acum_pos_x_nodos-=n.x;
            acum_pos_y_nodos-=n.y;
            Nodo a;
            a.indice=-1;
            nodos[n.indice]=a;
            cant_nodos--;
        }

    }
    bool existeNodo(Nodo& nodo) {

        if( nodos[nodo.indice].indice!=-1) {
            return true;
        }else return false;
    }
    float distancia_de_nodo_al_centro(Nodo &n) {
        if(nodos[n.indice].indice == -1) return MAX_Y * MAX_X;
        return distancia_euclidea(x,y,n.x,n.y);
    }

    bool operator<(cluster &c){
        if(distancia_euclidea(0,0,this->x,this->y) < distancia_euclidea(0,0,c.x,c.y)){
            return true;
        }else
            return false;
    }

    vector<Nodo> getNodos(){
        vector<Nodo> aux;
        for (int i = 2; i < nodos.size(); ++i) {
            if(nodos[i].indice != -1)
                aux.push_back(nodos[i]);
        }
        return aux;
    }

    void clear_cluster() {
        for (int i = 0; i < nodos.size(); ++i) {
            eliminar_nodo(nodos[i]);
        }

        x = 0;
        y = 0;
        demanda_total = 0;
        acum_pos_x_nodos=0;
        acum_pos_y_nodos=0;
        carga_actual = 0;
        cant_nodos = 0;
    }

private:
    float acum_pos_x_nodos, acum_pos_y_nodos;
    float demanda_total, carga_actual;
    float x, y;
    int id_nodo_mas_cercano;
    Nodo deposito;
    int cant_nodos;

    vector<Nodo> nodos;
};

class k_means{
public:
    k_means(){

    }
    k_means(int cn, int cc, Nodo dep, float cap_total){
        cantidad_de_nodos = cn;
        cluster_de_nodo.resize(cn+1);
        for (int i = 0; i < cluster_de_nodo.size(); ++i) {
            cluster_de_nodo[i] = -1;
        }
        deposito = dep;
        cantidad_de_clusters = cc;
        clusters.resize(cc);
        capacidad = cap_total;
    }
    //pre: tiene cluster asigado
    int getClusterDeNodo(int i){
        return cluster_de_nodo[i];
    }
    cluster& getClusterIesimo(int i){
        return clusters[i];
    }
    cluster& getClusterDeNodoIesimo(int i) {
        return clusters[cluster_de_nodo[i]];
    }

    void operator=(const k_means& c){
        cluster_de_nodo = c.cluster_de_nodo;
        clusters.resize(c.cantidad_de_clusters);

        for (int i = 0; i < clusters.size(); ++i) {
            clusters[i] = c.clusters[i];
        }
        deposito = c.deposito;
        cantidad_de_nodos = c.cantidad_de_nodos;
        capacidad = c.capacidad;
        cantidad_de_clusters = c.cantidad_de_clusters;
    }
    void generar_csv(string nombre){
        ofstream fout;
        fout.open("../salida/" + nombre+"/"+nombre +".csv");
        fout <<deposito.indice<<","<<deposito.x<<","<<deposito.y<<","<<cantidad_de_clusters+1<<endl;
        for (int i = 0; i < cantidad_de_clusters; ++i) {
            vector<Nodo> nodos = getClusterIesimo(i).getNodos();
            for(int j = 0; j < nodos.size(); j++) {
                if(nodos[j].indice!=-1) fout <<nodos[j].indice<<","<<nodos[j].x<<","<<nodos[j].y<<","<<i<<endl;

            }
        }
        fout.close();
    }

    int cluster_mas_cercano(int c1) {//Devuelvo el cluster más cercano y con menos demanda posible
        float minDist = MAX_X*MAX_Y;
        int minInd = 0;
        for (int i = 0; i < clusters.size(); ++i) {
            if(i != c1) {
                if(minDist>distancia_euclidea(clusters[i].getX(),clusters[i].getY(),clusters[c1].getX(),clusters[c1].getY())){
                    minDist = distancia_euclidea(clusters[i].getX(),clusters[i].getY(),clusters[c1].getX(),clusters[c1].getY());
                    minInd = i;
                }
            }
        }
        float minDist2 = MAX_Y*MAX_X;
        int minInd2 = 0;
        for (int i = 0; i < clusters.size(); ++i) {
            if(i != c1) {
                if(distancia_euclidea(clusters[i].getX(),clusters[i].getY(),clusters[c1].getX(),clusters[c1].getY()) > minDist && minDist2 > distancia_euclidea(clusters[i].getX(),clusters[i].getY(),clusters[c1].getX(),clusters[c1].getY())){
                    minDist2 = distancia_euclidea(clusters[i].getX(),clusters[i].getY(),clusters[c1].getX(),clusters[c1].getY());
                    minInd2 = i;
                }
            }
        }
        return ( (clusters[minInd].getCargaActual() > clusters[minInd2].getCargaActual())? minInd2 : minInd);
    }

    void juntar_clusters(int c1, int c2){
        if(getClusterIesimo(c1).getCantNodos()> getClusterIesimo(c2).getCantNodos()){
            for (int i = 0; i < getClusterIesimo(c1).getNodos().size(); ++i) {
                if(getClusterIesimo(c1).getNodos()[i].indice!=-1) {
                    agregar_nodo_a_cluster(getClusterIesimo(c1).getNodos()[i],c2);
                }
            }
        }
    }
    void agregar_nodo_a_cluster(Nodo &n, int icluster){
        if(cluster_de_nodo[n.indice] != -1) {
            clusters[cluster_de_nodo[n.indice]].eliminar_nodo(n);
        }
        cluster_de_nodo[n.indice] = icluster;
        clusters[icluster].agregar_nodo(n);
    }

    void agregar_nodo_a_cluster_mas_cercano (Nodo &n){
        int posClusterMin = -1;
        float minDist = MAX_X*MAX_Y;
        if(cluster_de_nodo[n.indice]==-1) {
            for (int i = 0; i < clusters.size(); ++i) {
                if(clusters[i].getCargaActual() + n.demanda <= capacidad && minDist > distancia_euclidea(n.x,n.y,clusters[i].getX(),clusters[i].getY())){
                    minDist=distancia_euclidea(n.x,n.y,clusters[i].getX(),clusters[i].getY());
                    posClusterMin = i;
                }
            }
            if(posClusterMin==-1) {
                for (int i = 0; i < clusters.size(); ++i) {
                    if(minDist > distancia_euclidea(n.x,n.y,clusters[i].getX(),clusters[i].getY())){
                        minDist=distancia_euclidea(n.x,n.y,clusters[i].getX(),clusters[i].getY());
                        posClusterMin = i;
                    }
                }
            }
        }else {
            for (int i = 0; i < clusters.size(); ++i) {
                if(cluster_de_nodo[n.indice]!=i && clusters[i].getCargaActual() + n.demanda <= capacidad && minDist > distancia_euclidea(n.x,n.y,clusters[i].getX(),clusters[i].getY())){
                    minDist=distancia_euclidea(n.x,n.y,clusters[i].getX(),clusters[i].getY());
                    posClusterMin = i;
                }
            }
            if(posClusterMin==-1) {
                for (int i = 0; i < clusters.size(); ++i) {
                    if(cluster_de_nodo[n.indice]!=i && minDist > distancia_euclidea(n.x,n.y,clusters[i].getX(),clusters[i].getY())){
                        minDist=distancia_euclidea(n.x,n.y,clusters[i].getX(),clusters[i].getY());
                        posClusterMin = i;
                    }
                }
            }
        }
        cluster_de_nodo[n.indice] = posClusterMin;
        clusters[posClusterMin].agregar_nodo(n);
    }

    void eliminar_nodo_de_cluster(Nodo &n, int icluster){
        cluster_de_nodo[n.indice] = -1;
        clusters[icluster].eliminar_nodo(n);
    }

    bool nodo_tiene_cluster(int i) {
        if(cluster_de_nodo[i]==-1){
            return false;
        }else return true;
    }
    float get_distancia_de_nodo_a_cluster(Nodo &n){
        if(cluster_de_nodo[n.indice] == -1 ) return MAX_Y*MAX_X;
        return distancia_euclidea(n.x,n.y,clusters[cluster_de_nodo[n.indice]].getX(),clusters[cluster_de_nodo[n.indice]].getY());
    }
    int cant_clusters(){
        return cantidad_de_clusters;
    }

    void mostrar_clusters(){
        for (int i = 0; i < cantidad_de_clusters; ++i) {
            cout <<"Cluster " << i<<" cantidad de nodos: "<< clusters[i].getCantNodos()<<" porcentaje completo de carga: " <<clusters[i].getCargaActual()<<"/"<< clusters[i].getDemandaTotal()<<endl;
        }
    }
    bool existe_cluster_con_exceso(){
        int i = 0;
        while(i < clusters.size()&&clusters[i].getCargaActual()<=clusters[i].getDemandaTotal()) {
            i++;
        }
        return ((i==clusters.size())? false:true);
    }

    void clear_clusters(){
        for (int j = 0; j < cluster_de_nodo.size(); ++j) {
            cluster_de_nodo[j] = -1;
        }
        for (int i = 0; i < clusters.size(); ++i) {
            clusters[i].clear_cluster();
        }
    }
    void agregar_cluster(){
        cantidad_de_clusters++;
        clusters.resize(cantidad_de_clusters);
    }

    void mostrar_clusters_completo(){
        int cant = 0;
        vector<bool> v(cantidad_de_nodos+1,false);
        for (int i = 0; i < cantidad_de_clusters; ++i) {
            cout <<"Cluster " << i<<" cantidad de nodos: "<< clusters[i].getCantNodos()<<" porcentaje completo de carga: " <<clusters[i].getCargaActual()<<"/"<< clusters[i].getDemandaTotal()<<" Nodos: "<<endl;
            for (int j = 0; j < getClusterIesimo(i).getNodos().size(); ++j) {
                if(getClusterIesimo(i).getNodos()[j].indice!=-1) {
                    cout << getClusterIesimo(i).getNodos()[j].indice <<",";
                    cant++;
                }
            }
            cout <<endl<<endl;
        }
        cout <<"Cantidad total de nodos asignados a cluster: "<<cant<<endl;
    }
    int cluster_mas_cercano_a_nodo(Nodo &n) {
        float distMin = MAX_X*MAX_Y;
        int indiceMin;
        for (int i = 0; i < cant_clusters(); ++i) {
            if(cluster_de_nodo[n.indice]!=i && distMin > distancia_euclidea(n.x,n.y, clusters[i].getX(),clusters[i].getY())){
                distMin = distancia_euclidea(n.x,n.y, clusters[i].getX(),clusters[i].getY());
                indiceMin = i;
            }
        }
        return indiceMin;
    }
    float getCapacidad(){
        return this->capacidad;
    }
    void agregar_cluster(cluster &c){
        clusters.push_back(c);
    }
private:
    vector<int> cluster_de_nodo;
    vector<cluster> clusters;
    Nodo deposito;
    int cantidad_de_nodos;
    int cantidad_de_clusters;
    float capacidad;
};

int main() {

    int cantidad_nodos;
    int capacidad_camion;
    vector<Nodo> vector_nodos; //Tamaño = cantidad_nodos
    float pos_max_x = 0, pos_max_y = 0;
    srand(time(NULL));
    float demanda_total = 0;
    string nombre_in = "M-n121-k7";
    ifstream fin;
    fin.open("../entrada/"+ nombre_in + "/"+nombre_in +".vrp.txt");
    //fin.open("../X-n110-k13.vrp.txt");
    if (fin.is_open()) {
        fin >> cantidad_nodos >> capacidad_camion;
        cout <<cantidad_nodos<<","<<capacidad_camion<<endl;
        vector_nodos.resize(cantidad_nodos+1);
        Nodo a;
        a.indice=-1;
        a.x=-1;
        a.y=-1;
        vector_nodos[0] = a;
        for (int i = 1; i < vector_nodos.size(); ++i) { //nodo 0 no se usa y el 1 es el deposito
            Nodo n;
            fin >> (n.indice) >> n.x >> n.y;
            if (pos_max_x < n.x) pos_max_x = n.x;
            if (pos_max_y < n.y) pos_max_y = n.y;
            //cout <<"Guardo el: "<<n.indice<< " X: "<<n.x<< " Y: "<<n.y<<endl;
            vector_nodos[n.indice] = n;
        }
        for (int j = 1; j < vector_nodos.size(); ++j) {
            int indice;
            fin >> indice >> vector_nodos[j].demanda;
            demanda_total += vector_nodos[j].demanda;
        }
    } else{
        cout << "Error" << endl; exit(0);
    }
    MAX_X = pos_max_x;
    MAX_Y = pos_max_y;
    deposito = vector_nodos[1];

    cout << "La demanda total es : " << demanda_total << endl << "Y el promedio es: " << ceil(demanda_total/cantidad_nodos)<< endl;

    //Aca digo que los k-vecinos deberian ser:
    float promedio_demanda = floor(demanda_total / cantidad_nodos);
    float cantidad_clientes_puede_visitar_camion = floor(capacidad_camion / promedio_demanda);
    cout << "Cant clientes que puede visitar: " << cantidad_clientes_puede_visitar_camion <<endl;
    int cant_clie_satisface_camion = floor(capacidad_camion / promedio_demanda);
    float demanda_satisf_x_camion = floor(cant_clie_satisface_camion * promedio_demanda);
    int cant_cam = floor(demanda_total / demanda_satisf_x_camion); //la correcta o necesito 1 mas
    cout << "Cantidad de camiones: " << cant_cam<< endl;


    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist_x(0,pos_max_x);
    std::uniform_int_distribution<std::mt19937::result_type> dist_y(0,pos_max_y);

    bool repetir=true;
    long long int cantidad_de_iteraciones_hasta_aumentar_cluster = 0, intentos_de_optimizar = 0, tope_intentos_de_optimizar=cantidad_nodos, intentos_para_cluster_particular=0, tope_intentos_para_cluster_particular = pow(cantidad_nodos,2);
    float mejor_sol = INT64_MAX, mejor_sol2 = INT64_MAX;
    int mejor_sol_cluster=cantidad_nodos, mejor_sol_cluster2 = cantidad_nodos;
    k_means clust_real, mejor_cluster_actual, mejor_cluster_actual2;
    bool seguir_buscando_para_mas_clusters=true;

    while(repetir) { //este ciclo tiene como objetivo: generar varios tipos de clusterizaciones diferentes para encontrar la mejor solucion (para el parametro de repeticiones permito, claramente no la mejor absoluta). Algunos tipos de clusterizacion  no van a serir y este ciclo sirve para empezar otra de 0. Otras van a servir y este ciclo sirve para empezar otra de 0 y ver si es mejor (al realizar tsp a los clusters)
        cantidad_de_iteraciones_hasta_aumentar_cluster++;
        k_means clusterizacion(cantidad_nodos, cant_cam,deposito,capacidad_camion);
        for (int i = 0; i < cant_cam; ++i) {
            clusterizacion.getClusterIesimo(i).setDeposito(deposito); //Fijo al deposito en la clusterizacion
            clusterizacion.getClusterIesimo(i).setDemanda(capacidad_camion); //Fijo la demanda total

            clusterizacion.getClusterIesimo(i).setX(dist_x(rng)); //fijo posiciones aleatorias
            clusterizacion.getClusterIesimo(i).setY(dist_y(rng));
            clusterizacion.getClusterIesimo(i).setCantidadNodos(cantidad_nodos+1); //Nos movemos 1 para poder indexar directo del indice de los nodos que va de 1 a n
        }
        vector<bool> asignados(cantidad_nodos+1,false);
        asignados[0] = true;
        asignados[1] = true;
        int centroides_iguales = 0;
        intentos_para_cluster_particular = 0;
        //while(!(centroides_iguales==cant_cam && intentos_para_cluster_particular>=tope_intentos_para_cluster_particular)){
        while(centroides_iguales!= cant_cam && intentos_para_cluster_particular<tope_intentos_para_cluster_particular){ //Este ciclo me permite formar una clusterizacion, esto es, que cada no de los k clusters tengan a los nodos distribuidos (no necesariamente con las demnadas correctas), pero si con las posiciones de cada centro de cluster sin chance a modificarse mas. Todo lo de adentr de este ciclo es para formar el cluster
            intentos_para_cluster_particular++;
            //cout <<"Intentos con este cluster: " <<intentos_para_cluster_particular<<"/"<<tope_intentos_para_cluster_particular<<endl<<"Intentos de optimizar: "<<intentos_de_optimizar<<"/"<<tope_intentos_de_optimizar<<endl;
            centroides_iguales = 0;
            for (int i = 0; i < cant_cam; ++i) { //Repito por cada cluster
                for (int j = 0; j < cant_cam; ++j) {
                    if (j != i) {
                        int k=2;
                        while(k < vector_nodos.size()) {
                            //si el nodo k esta mas cerca del cluster i que del j
                            if(distancia_euclidea(vector_nodos[k].x, vector_nodos[k].y, clusterizacion.getClusterIesimo(i).getX(), clusterizacion.getClusterIesimo(i).getY()) < distancia_euclidea(vector_nodos[k].x, vector_nodos[k].y, clusterizacion.getClusterIesimo(j).getX(), clusterizacion.getClusterIesimo(j).getY())){
                                //si el nodo k esta mas cerca del cluster en el que ya esta o del cluster i
                                if(distancia_euclidea(vector_nodos[k].x, vector_nodos[k].y, clusterizacion.getClusterIesimo(i).getX(), clusterizacion.getClusterIesimo(i).getY()) < clusterizacion.get_distancia_de_nodo_a_cluster(vector_nodos[k])){
                                    //Antes de agregarlo al cluster, me fijo si me lo llena. En ese caso,me fijo si puedo sacar algun nodo y meter al actual
                                    //Si no lo llena entonces agrego normal

                                    if(clusterizacion.getClusterIesimo(i).getCargaActual() + vector_nodos[k].demanda <= capacidad_camion) {
                                        //if(clusterizacion.getClusterIesimo(i).tiene_exceso()) cout <<"Voy a agregar al clluster "<<i<<" el nodo "<<vector_nodos[k].indice<<" de demanda "<< vector_nodos[k].demanda <<" carga act cluster: "<<clusterizacion.getClusterIesimo(i).getCargaActual()<<endl;
                                        clusterizacion.agregar_nodo_a_cluster(vector_nodos[k],i);
                                        asignados[vector_nodos[k].indice] = true;
                                    }else {
                                        int posNodoLejano = clusterizacion.getClusterIesimo(i).nodo_mas_lejano(i);
                                        if(clusterizacion.get_distancia_de_nodo_a_cluster(vector_nodos[posNodoLejano]) > distancia_euclidea(vector_nodos[k].x,vector_nodos[k].y, clusterizacion.getClusterIesimo(i).getX(),clusterizacion.getClusterIesimo(i).getY()) ){
                                            if(clusterizacion.getClusterIesimo(i).getCargaActual() - vector_nodos[posNodoLejano].demanda + vector_nodos[k].demanda <=capacidad_camion) {
                                                clusterizacion.eliminar_nodo_de_cluster(vector_nodos[posNodoLejano],i);
                                                asignados[vector_nodos[posNodoLejano].indice] = false;
                                                clusterizacion.agregar_nodo_a_cluster(vector_nodos[k],i);
                                                asignados[vector_nodos[k].indice] = true;
                                            }
                                        }
                                    }

                                }
                            }
                            k++;
                        }
                    }
                }
            }
            //Renuevo los centros de cada cluster y vuelvo a calcularlos
            int acum = 0;
            for (int j = 0; j < clusterizacion.cant_clusters(); ++j) {
                acum += clusterizacion.getClusterIesimo(j).getCantNodos();
            }
            float prom = floor(acum / cant_cam);
            for (int i = 0; i < cant_cam; ++i) {
                float avg_x = clusterizacion.getClusterIesimo(i).getAvgX(), avg_y = clusterizacion.getClusterIesimo(i).getAvgY();
                int cant_nodos = clusterizacion.getClusterIesimo(i).getCantNodos();
                if(clusterizacion.getClusterIesimo(i).getX() == avg_x && clusterizacion.getClusterIesimo(i).getY() == avg_y) {
                    centroides_iguales++;
                }else {
                    avg_x = clusterizacion.getClusterIesimo(i).getAvgX();
                    avg_y = clusterizacion.getClusterIesimo(i).getAvgY();
                    clusterizacion.getClusterIesimo(i).setX(avg_x);
                    clusterizacion.getClusterIesimo(i).setY(avg_y);
                }
            }
        }
        int nodosAsig=0;
        float espacios_libres=0, espacios_por_asignar=0, demandas=0;


        for (int l = 0; l < clusterizacion.cant_clusters(); ++l) {
            demandas += clusterizacion.getClusterIesimo(l).getCargaActual();
            nodosAsig+=clusterizacion.getClusterIesimo(l).getCantNodos();
            if(clusterizacion.getClusterIesimo(l).tiene_exceso()){
                espacios_por_asignar += clusterizacion.getClusterIesimo(l).getCargaActual() - clusterizacion.getCapacidad();
            }else {
                espacios_libres+= capacidad_camion - clusterizacion.getClusterIesimo(l).getCargaActual();
            }
        }

        repetir = false;


        if(demandas < demanda_total) repetir = true;

        if(cantidad_de_iteraciones_hasta_aumentar_cluster > cantidad_nodos){
            //si despues de realizar cierta cantidad de iteraciones sigo sin poder clusterizar, entonces aumento un cluster
            cant_cam++;
            cout <<" VOY A NECESITAR OTRO CLUSTER-------------------------------------------------------------, ahora tengo: "<<cant_cam<<endl;
            repetir = true;
            cantidad_de_iteraciones_hasta_aumentar_cluster=0;
            continue;
        }
        if(espacios_libres < espacios_por_asignar){
            cant_cam++;
            cout <<" VOY A NECESITAR OTRO CLUSTER-------------------------------------------------------------, ahora tengo: "<<cant_cam<<endl;
            repetir = true;
            continue;
        }
        if(nodosAsig < cantidad_nodos-1){ //Le resto 1 porque el deposito no cuenta
            repetir = true;
            if((promedio_demanda * (cantidad_nodos-1-nodosAsig)) > espacios_libres){
                cant_cam++;
                cout <<" VOY A NECESITAR OTRO CLUSTER-------------------------------------------------------------, ahora tengo: "<<cant_cam<<endl;
            }
            continue;
        }
        if(repetir && intentos_de_optimizar> tope_intentos_de_optimizar){
            clust_real = mejor_cluster_actual;
            repetir = false;
        }

        if(!repetir) { //significa que ya encontre una posible solucion
            if(intentos_de_optimizar > tope_intentos_de_optimizar){ //no tengo mas intentos, tengo que dar la solucion (o la mejor que ya haya encontrado)
                clust_real = mejor_cluster_actual;
            }else{//si me quedan intentos disp, veo si puedo encontrar una mejor sol
                float valor = convertir_para_tsp(clusterizacion, nombre_in);
                if(mejor_sol_cluster >= cant_cam){
                    if(mejor_sol>valor) {
                        mejor_sol_cluster = cant_cam;
                        mejor_sol = valor;
                        mejor_cluster_actual = clusterizacion;
                    }

                }else if(mejor_sol_cluster < cant_cam){
                    if(mejor_sol2>valor){
                        mejor_sol2 = valor;
                        mejor_sol_cluster2 = cant_cam;
                        mejor_cluster_actual2 = clusterizacion;
                    }
                }
                repetir = true;
            }
            intentos_de_optimizar++;
        }
        if(!seguir_buscando_para_mas_clusters && mejor_sol_cluster<cant_cam) { //si ya encontre una solucion para una determinada cantidad de cammiones y ahora estoy buscando nuevas soluciones pero con mas camiones
            repetir = false;
        }
    }
    clust_real.mostrar_clusters();
    float v = convertir_para_tsp(mejor_cluster_actual,nombre_in);
    clust_real.generar_csv(nombre_in);
    cout <<"MEJOR SOLUCION ENCONTRADA CON "<<mejor_sol_cluster<<" clusters fue: "<<v<<endl;
    if(seguir_buscando_para_mas_clusters && mejor_sol_cluster2 <= cant_cam) cout <<"MEJOR SOLUCION ENCONTRADA CON "<<mejor_sol_cluster2<<" clusters fue: "<<mejor_sol2<<endl;
    std::ofstream outfile;
    string salida = string("../salida") + "/" +nombre_in[0]+"/"+"soluciones.txt";
    outfile.open(salida, std::ios_base::app);
    if (outfile.is_open()){
        outfile <<cantidad_nodos<<","<<mejor_sol_cluster<<","<<v<<endl;
    }
    return 0;
}

float convertir_para_tsp(k_means& clusterizacion, string nombre_in) {
    float costo_viaje = 0;
    float ctsp2=0;
    vector<vector<Nodo> > mat(clusterizacion.cant_clusters());
    cout <<"cant clusters: "<<clusterizacion.cant_clusters()<<endl;
    for (int i = 0; i < clusterizacion.cant_clusters(); ++i) {
        cout <<"cant nodos del cluster "<< i<<": "<<clusterizacion.getClusterIesimo(i).getCantNodos()<<endl;
        vector<Nodo> nodos = clusterizacion.getClusterIesimo(i).getNodos();
        vector<Nodo> nodos_con_dep(nodos.size()+1);
        nodos_con_dep[0] = deposito;
        for (int j = 0; j < nodos.size(); ++j) {
            nodos_con_dep[j + 1] = nodos[j];
        }
        cout <<"Ruteo " << i<<endl;
        vector<Nodo> camino = tsp_con_grasp(nodos_con_dep,0,costo_viaje,1);
        for(auto c:camino){
            cout <<c.indice<<" , ";
        }
        cout <<endl;
        ctsp2+=costo_viaje;

        exportar_grafo(camino,costo_viaje,i,nombre_in);
    }
    cout <<"Costo total: "<<ctsp2<<endl;
    return ctsp2;
}

void exportar_grafo(vector<Nodo>& camino, float costo,int ruteo, string in){
    ofstream fout;
    auto s = std::to_string(ruteo);
    fout.open("../salida/" + in+"/"+in + "-ruteo" + s + ".csv");
    for (int i = 0; i < camino.size()-1; ++i) {
        fout << camino[i].indice<<","<<camino[i].x<<","<< camino[i].y<<endl;//indice,posx,posy
    }
    for (int j = 0; j < camino.size()-1; ++j) {
        fout << camino[j].indice<<","<<camino[j+1].indice<<endl;//indice_nodo_a,indice_nodo_b
    }
    fout<<costo;
    fout.close();
}

vector<Nodo> tsp2(vector<Nodo>& nodos, int nodo_comienzo, float &costo_viaje, float p){
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
        if(posMin == -1) { //Significa que no hay mas que visitar
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

vector<Nodo> tsp_con_grasp(vector<Nodo>& nodos, int nodo_comienzo, float &costo_viaje, float p) {
    //Genero solucion con heuristica golosa
    float costo_sol_golosa=0, costo_aux = 0, costo_min;
    vector<Nodo> sol = tsp2(nodos,nodo_comienzo,costo_sol_golosa,1);
    costo_min = costo_sol_golosa;
    vector<Nodo> aux;
    float p_shift = 0.1;
    while (p>=0.5){
        aux = tsp2(nodos,nodo_comienzo,costo_aux,p);
        if(costo_aux < costo_sol_golosa){
            sol = aux;
            costo_min = costo_aux;
        }
        costo_aux=0;
        p -=p_shift;
    }
    costo_viaje = costo_min;
    return sol;
}

float distancia_euclidea(float x1,float y1, float x2, float y2) {
    float dif_x = x1-x2;
    float dif_y = y1-y2;
    float suma_cuads = pow(dif_x,2) + pow(dif_y,2);
    return sqrt(suma_cuads);
}
