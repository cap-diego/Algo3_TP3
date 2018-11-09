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

vector<Nodo> aux_tsp(const vector<Nodo>&, int pos,float costo);
float tsp(const vector<vector<float> >& dist_entre_nodos, int pos, int visitados, vector<vector<float>>& costos, vector<vector<Nodo> >& sol, const vector<Nodo>& nodos);

vector<Nodo> tsp2(vector<Nodo>& nodos, int nodo_comienzo, float &costo_viaje);

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
        nodos.resize(cn);
        Nodo n;
        n.indice = -1;
        for (int i = 0; i < cn; ++i) {
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

        for (int i = 0; i < nodos.size(); ++i) {
            if(nodos[i].demanda <= demanda && maxDist < distancia_de_nodo_al_centro(nodos[i])) {
                maxDist = distancia_de_nodo_al_centro(nodos[i]);
                posMax = i;
            }
        }
        if(posMax==-1){
            maxDist = 0;
            for (int i = 0; i <nodos.size(); ++i) {
                if(maxDist < distancia_de_nodo_al_centro(nodos[i])) {
                    maxDist = distancia_de_nodo_al_centro(nodos[i]);
                    posMax = i;
                }
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
        for (int i = 0; i < nodos.size(); ++i) {
            if(nodos[i].indice != -1)
                aux.push_back(nodos[i]);
        }
        return aux;
    }

    void clear() {
        for (int i = 0; i < nodos.size(); ++i) {
            eliminar_nodo(nodos[i]);
        }

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
            //si ya pertenece a un cluster, antes de meterlo en otro necesito borrarlo de donde esta
            //entonces lo elimino del que esta
            //cout <<"Quiero agregar nodo " <<n.indice<<" a cluster: "<<icluster<<" y actual en cluster:" <<cluster_de_nodo[n.indice]<<endl;
            clusters[cluster_de_nodo[n.indice]].eliminar_nodo(n);
        }
        cluster_de_nodo[n.indice] = icluster;
        clusters[icluster].agregar_nodo(n);
        //y modifico el cluster del nodo

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
            clusters[i].clear();
        }
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
    int cant_cam = ceil(demanda_total / demanda_satisf_x_camion); //la correcta o necesito 1 mas
    cout << "Cantidad de camiones: " << cant_cam<< endl;


    k_means clusterizacion(cantidad_nodos, cant_cam,deposito,capacidad_camion);
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist_x(0,pos_max_x);
    std::uniform_int_distribution<std::mt19937::result_type> dist_y(0,pos_max_y);
    for (int i = 0; i < cant_cam; ++i) {
        clusterizacion.getClusterIesimo(i).setDeposito(deposito); //Fijo al deposito en la clusterizacion
        clusterizacion.getClusterIesimo(i).setDemanda(capacidad_camion); //Fijo la demanda total

        clusterizacion.getClusterIesimo(i).setX(dist_x(rng)); //fijo posiciones aleatorias
        clusterizacion.getClusterIesimo(i).setY(dist_y(rng));
        clusterizacion.getClusterIesimo(i).setCantidadNodos(cantidad_nodos+1);
    }

        int centroides_iguales = 0;
        while(centroides_iguales!= cant_cam){
            centroides_iguales = 0;
            cout <<centroides_iguales<<endl;
            for (int i = 0; i < cant_cam; ++i) { //Repito por cada cluster
                for (int j = 0; j < cant_cam; ++j) {
                    if (j != i) {
                        for (int k = 2; k < vector_nodos.size(); ++k) {
                            //si el nodo k esta mas cerca del cluster i
                            if(distancia_euclidea(vector_nodos[k].x, vector_nodos[k].y, clusterizacion.getClusterIesimo(i).getX(), clusterizacion.getClusterIesimo(i).getY()) < distancia_euclidea(vector_nodos[k].x, vector_nodos[k].y, clusterizacion.getClusterIesimo(j).getX(), clusterizacion.getClusterIesimo(j).getY())){
                                //si el nodo k esta mas cerca del cluster en el que ya esta o del cluster i
                                if(distancia_euclidea(vector_nodos[k].x, vector_nodos[k].y, clusterizacion.getClusterIesimo(i).getX(), clusterizacion.getClusterIesimo(i).getY()) < clusterizacion.get_distancia_de_nodo_a_cluster(vector_nodos[k])){

                                    //Antes de agregarlo al cluster, me fijo si me lo llena. En ese caso,me fijo si puedo sacar algun nodo y meter al actual
                                    //Si no lo llena entonces agrego normal
                                    if(vector_nodos[k].demanda + clusterizacion.getClusterIesimo(i).getCargaActual() > capacidad_camion){
                                        int nodo_lejano = clusterizacion.getClusterIesimo(i).nodo_mas_lejano(vector_nodos[k].demanda);
                                        if( clusterizacion.getClusterIesimo(i).distancia_de_nodo_al_centro(vector_nodos[nodo_lejano]) > distancia_euclidea(vector_nodos[k].x,vector_nodos[k].y,clusterizacion.getClusterIesimo(i).getX(),clusterizacion.getClusterIesimo(i).getY()) && vector_nodos[k].demanda + clusterizacion.getClusterIesimo(i).getCargaActual() - vector_nodos[nodo_lejano].demanda <= capacidad_camion){
                                            clusterizacion.getClusterIesimo(i).eliminar_nodo(vector_nodos[k]);
                                            clusterizacion.agregar_nodo_a_cluster(vector_nodos[k],i);
                                        }else {

                                        }
                                    }else{
                                        clusterizacion.agregar_nodo_a_cluster(vector_nodos[k],i);
                                    }
                                }
                            }else {
                                //si el nodo k esta mas cerca del cluster j
                                if(distancia_euclidea(vector_nodos[k].x, vector_nodos[k].y, clusterizacion.getClusterIesimo(j).getX(), clusterizacion.getClusterIesimo(j).getY()) < clusterizacion.get_distancia_de_nodo_a_cluster(vector_nodos[k])){

                                    if(distancia_euclidea(vector_nodos[k].x, vector_nodos[k].y, clusterizacion.getClusterIesimo(j).getX(), clusterizacion.getClusterIesimo(j).getY()) < clusterizacion.get_distancia_de_nodo_a_cluster(vector_nodos[k])){
                                        //Antes de agregarlo al cluster, me fijo si me lo llena. En ese caso,me fijo si puedo sacar algun nodo y meter al actual
                                        //Si no lo llena entonces agrego normal
                                        if(vector_nodos[k].demanda + clusterizacion.getClusterIesimo(j).getCargaActual() > capacidad_camion){
                                            int nodo_lejano = clusterizacion.getClusterIesimo(j).nodo_mas_lejano(vector_nodos[k].demanda);
                                            if( clusterizacion.getClusterIesimo(j).distancia_de_nodo_al_centro(vector_nodos[nodo_lejano]) > distancia_euclidea(vector_nodos[k].x,vector_nodos[k].y,clusterizacion.getClusterIesimo(j).getX(),clusterizacion.getClusterIesimo(j).getY()) && vector_nodos[k].demanda + clusterizacion.getClusterIesimo(j).getCargaActual() - vector_nodos[nodo_lejano].demanda <= capacidad_camion){
                                                clusterizacion.getClusterIesimo(j).eliminar_nodo(vector_nodos[k]);
                                                clusterizacion.agregar_nodo_a_cluster(vector_nodos[k],j);
                                            }else {

                                            }
                                        }else{
                                            clusterizacion.agregar_nodo_a_cluster(vector_nodos[k],j);
                                        }
                                    }
                                }

                            }

                        }
                    }
                }
            }
            //Renuevo los centros de cada cluster y vuelvo a calcularlos
            int acum = 0;
            float desviacion;
            for (int j = 0; j < cant_cam; ++j) {
                acum += clusterizacion.getClusterIesimo(j).getCantNodos();
            }
            float prom = floor(acum / cant_cam);
            float acum_desv = 0;
            for (int i = 0; i < cant_cam; ++i) {
                acum_desv += pow(clusterizacion.getClusterIesimo(i).getCantNodos()-prom,2);
            }
            desviacion = sqrt(acum_desv/cant_cam);
            //cout <<"Desv: "<<desviacion<<endl;
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
                    /*if(cant_nodos <= prom - desviacion) {
                        avg_x = rand() % int(pos_max_x);
                        avg_y = rand() % int(pos_max_y);
                        clusterizacion.getClusterIesimo(i).setX(avg_x);
                        clusterizacion.getClusterIesimo(i).setY(avg_y);
                    }else{
                        avg_x = clusterizacion.getClusterIesimo(i).getAvgX();
                        avg_y = clusterizacion.getClusterIesimo(i).getAvgY();
                        clusterizacion.getClusterIesimo(i).setX(avg_x);
                        clusterizacion.getClusterIesimo(i).setY(avg_y);
                    }*/
                }
            }
        }

    cout <<"ANTES DE CONTROLAR EXCESOS"<<endl;

    clusterizacion.mostrar_clusters_completo();
    clusterizacion.generar_csv(nombre_in+"antes");


    //Ahora me fijo que no exista un cluster que sobrepase de demanda para el camion

    vector<int> cluster_con_exceso(cant_cam,-1);//Guardo indices de cluster que tienen exceso de carga
    int cantExcesos = 0;
    float espacios_libres=0, espacios_por_asignar=0;


    for (int l = 0; l < clusterizacion.cant_clusters(); ++l) {
        if(clusterizacion.getClusterIesimo(l).tiene_exceso()){
            cout << "Cluster "<< l<<" tiene exceso"<<endl;
            cluster_con_exceso[l] = l;
            espacios_por_asignar += clusterizacion.getClusterIesimo(l).getCargaActual() - clusterizacion.getCapacidad();
            cantExcesos++;
        }else {
            espacios_libres+= capacidad_camion - clusterizacion.getClusterIesimo(l).getCargaActual();
        }
    }

    if(espacios_libres < espacios_por_asignar){
        cout <<"AUMENTO CLUSTERS"<<endl;
        cant_cam++;
    }

/*
    for (int m = 0; m < cluster_con_exceso.size(); ++m) {
        if(cluster_con_exceso[m] != -1) {
            //Recorro los nodos y  los mando hacia el cluster mas cercano que los pueda contener
            for (auto& v : clusterizacion.getClusterIesimo(m).getNodos()) {
                int nearest_cluster = clusterizacion.cluster_mas_cercano_a_nodo(v);
                if(clusterizacion.getClusterIesimo(nearest_cluster).getCargaActual() + v.demanda < capacidad_camion) {
                    //si puede el cluster mas cercano albergar a v
                    clusterizacion.agregar_nodo_a_cluster(v,nearest_cluster);
                }
            }
        }
    }*/

    cout <<"DESPUES DE SACAR EXCESOS "<<endl;
    clusterizacion.mostrar_clusters();
    clusterizacion.generar_csv(nombre_in);


    float costo_viaje = 0;

    for (int i = 0; i < clusterizacion.cant_clusters(); ++i) {
        vector<Nodo> nodos = clusterizacion.getClusterIesimo(i).getNodos();
        vector<Nodo> nodos_con_dep(nodos.size()+1);
        nodos_con_dep[0] = deposito;
        for (int j = 0; j < nodos.size(); ++j) {
            nodos_con_dep[j + 1] = nodos[j];
        }
        cout <<"Ruteo " << i<<endl;
        vector<Nodo> camino = tsp2(nodos_con_dep,(i),costo_viaje);
        for(auto c:camino){
            cout <<c.indice<<" , ";
        }
        cout <<endl;
        exportar_grafo(camino,costo_viaje,i,nombre_in);
    }
    cout <<"Costo total: "<<costo_viaje<<endl;


    return 0;
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

vector<Nodo> tsp2(vector<Nodo>& nodos, int nodo_comienzo, float &costo_viaje){
    vector<Nodo> camino;
     //contiene los nodos del cluster
    vector<vector<float> > distancias_entre_nodos;
    vector<bool> visitados(nodos.size(),false);
    distancias_entre_nodos.resize(nodos.size());
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
    int act = 0;
    camino.push_back(nodos[0]);
    while(hay_nodos){
        visitados[act] = true;
        float minDist = MAX_X*MAX_Y;
        int posMin = -1;
        for (int i = 0; i < nodos.size(); ++i) {
            if( i!= act && visitados[i]==false && minDist > distancias_entre_nodos[act][i]){
                minDist = distancias_entre_nodos[act][i];
                posMin = i;
            }
        }
        if(posMin == -1) { //Significa que no hay mas que visitar
            hay_nodos= false;
            costo_viaje += distancias_entre_nodos[act][0];
            //cout <<"Agrego trayecto de "<< nodos[act].indice<<" a "<< nodos[0].indice<<endl;
            camino.push_back(nodos[0]);
        }else{
            //cout <<"Agrego trayecto de "<< nodos[act].indice<<" a "<< nodos[posMin].indice<<endl;
            camino.push_back(nodos[posMin]);
            costo_viaje+=minDist;
            act = posMin;
        }
    }
    return camino;
}

vector<Nodo> aux_tsp(const vector<Nodo>& nodos, int pos, float costo) {
    vector<vector<float> > costos(nodos.size());
    vector<vector<Nodo> > sol;
    sol.resize((1<<nodos.size())-1);
    for (auto& v : sol)
        v.push_back(nodos[0]);
    for(auto& neighbors : costos){
        neighbors = vector<float>((1 << nodos.size()) - 1, MAX_X*MAX_Y);//vector que tiene (2^n)-1 posiciones y cargadas inicialemnte con un valor maximo. El vector representa las posibles combinaciones
    }
    vector<vector<float> > distancias_entre_nodos;
    distancias_entre_nodos.resize(nodos.size());

    float maxDist = 0;

    for (int i = 0; i < nodos.size(); i++) {
        distancias_entre_nodos[i].resize(nodos.size());
        for (int j = 0; j < nodos.size(); ++j) {
            distancias_entre_nodos[i][j] = distancia_euclidea(nodos[i].x,nodos[i].y,nodos[j].x,nodos[j].y);
            if(maxDist < distancias_entre_nodos[i][j]) maxDist = distancias_entre_nodos[i][j];
        }
    }
    costo = tsp(distancias_entre_nodos,0,1,costos,sol,nodos);
    int maxNodos = 0;
    int posMax;
    for (int k = 0; k < sol.size(); ++k) {
        if(maxNodos < sol[k].size()){
            posMax = k;
        }
    }
    return sol[posMax];
}


float tsp(const vector<vector<float> >& dist_entre_nodos, int pos, int visitados, vector<vector<float>>& costos, vector<vector<Nodo> >& sol, const vector<Nodo>& nodos) {

    if(visitados == ((1 << dist_entre_nodos.size()) - 1)){//si visitados ya movio tantos bits como ciudades -1, entonces solo falta volver al origen
        sol[visitados].push_back(nodos[0]);
        return dist_entre_nodos[pos][0]; // return to starting city
    }

    if(costos[pos][visitados] != MAX_X*MAX_Y){
        return costos[pos][visitados];
    }


    for(int i = 0; i < dist_entre_nodos.size(); ++i) {
        if(i!=pos  && !(visitados & (1<<i))) {//visitados & (1<<i)  es fijarse si el iesimo ya fue visitado

            float dist = dist_entre_nodos[pos][i] + tsp(dist_entre_nodos, i, visitados | (1 << i), costos,sol,nodos); //visititados | (1<<i) es sumar a los visitados i visitas mas, es decir, que cuando este ciclo itero i veces, significa que ya calculamos i posibles soluciones de tsp diferentes, cada una yendo del nodo actual hacia otro nodo y ver finamente cual es mejor
            if (dist < costos[pos][visitados]) {
                costos[pos][visitados] = dist;
                sol[visitados].push_back(nodos[i]);
            }
        }
    }
    return costos[pos][visitados];
}


float distancia_euclidea(float x1,float y1, float x2, float y2) {
    float dif_x = x1-x2;
    float dif_y = y1-y2;
    float suma_cuads = pow(dif_x,2) + pow(dif_y,2);
    return sqrt(suma_cuads);
}
