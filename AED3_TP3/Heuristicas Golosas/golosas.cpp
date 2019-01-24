#include "golosas.h"
#include "Respuesta.h"
#include <algorithm>

void mergeRoute(Route& a, Route& b){
    vector<Customer> clientesB = b.getRoute();
    for (int i = 0; i < clientesB.size(); ++i) {
        a.addCustomer(clientesB[i]);//Añado los clientes visitados por B al final de a.
    }
}

Respuesta heuristicaSavings(System& s) {
    vector<Route> soln;
    double saving;
    vector<savingEntreAB> savings;
    vector<Customer *> clientes = s.getCustomers();
    Customer depot = s.getDepot();
    for (int i = 1; i < clientes.size(); ++i) {
        for (int j = i+1; j < clientes.size(); ++j) {
            saving = EuclideanDist(depot, *clientes[i]) + EuclideanDist(depot, *clientes[j]) -
                     EuclideanDist(*clientes[i], *clientes[j]);//Calculo el saving entre i y j
            savingEntreAB actual(clientes[i], clientes[j], saving);//Lo guardo
            savings.push_back(actual);
        }
    }
    //Ordeno los savings de manera decreciente
    sort(savings.begin(), savings.end(), [] (const savingEntreAB& s1, const savingEntreAB& s2) -> bool { return s1.saving > s2.saving; });

    for (int i = 1; i < clientes.size(); ++i) {//Creo un camino que sea ida y vuelta para cada uno de los clientes.
        Route newRoute = Route();
        newRoute.addCustomer(*clientes[i]);
        soln.push_back(newRoute);
    }
    for (savingEntreAB savingActual: savings) {//Aqui unire caminos
        int i = 0;
        int j = 0;
        while(i < soln.size() && !(soln[i].lastDelivery() == *savingActual.from())){//Busco un camino que termine en i
            i++;
        }
        while(j < soln.size() && !(soln[j].firstDelivery() == *savingActual.to())){//Busco un camino que empiece en j
            j++;
        }
        if(i!= soln.size() && j!= soln.size() && i!=j && soln[i].getActualWeight()+soln[j].getActualWeight() <= s.getCapacity()){//De existir dichos caminos
            //Los uno, sea el camino soln[i] = {D,...i,D} y el camino soln[j] = {D,j,....D}, quiero el camino resultado = {D,...i,j...D} es decir los uno
            //En particular los voy a unir si la capacidad del camion resultante de unir ambos sigue siendo <= que la cap max.
            mergeRoute(soln[i],soln[j]);
            soln.erase(soln.begin()+j);//Borro el camino que empezaba en j.
        }

    }

    Respuesta res(soln, &s);
    return res;
}


//Para la siguiente heuristica haremos lo siguiente:
//Armaremos un minheap que respetara la distancia de los clientes al deposito (mientras mas cerca, mas arriba en el heap)
//Y tomaremos el tope para empezar a armar un camino, si puedo agregar otro cliente lo agrego y mientras voy actualizando el heap, Sino, cierro el camino y empiezo con otro
//Y haremos esto hasta que el heap este vacio, es decir, que no haya mas clientes por visitar.
Respuesta heuristicaMasCercanoAlDeposito(System& s){
    vector<Route> soln;
    vector<Customer*> clientes = s.getCustomers();
    priority_queue<Customer*,vector<Customer*>,comparator> distanciasAlDepot;// minHeap con todos los clientes ordenados segun su distancia al depot (de menor a mayor)
    customerToCompare = s.getDepot();//Seteo algo para que el comparator use el depot.
    for (Customer* clienteActual : clientes) {//Lo armo
        if(clienteActual->isDepot()) continue;
        distanciasAlDepot.push(clienteActual);
    }

    while(!distanciasAlDepot.empty()){
        Customer* Actual = distanciasAlDepot.top();//Empiezo por el que ahora esta "mas cerca" del depot
        Route actualRoute = Route();//Creo una nueva ruta
        while(Actual != NULL && Actual->getDemand() + actualRoute.getActualWeight() <= s.getCapacity()) {
            actualRoute.addCustomer(*Actual);//Agrego el actual a la ruta
            Actual->isVisited();//Marco actual como visitado
            distanciasAlDepot.pop();//Saco a Actual del minheap.
            if(!distanciasAlDepot.empty()){
                Actual = distanciasAlDepot.top();// Actualizo Actual al siguiente de menor distancia.
            } else {
                Actual = NULL;//Para que termine el while
            }

        }
        soln.push_back(actualRoute);
    }

    Respuesta res(soln, &s);
    return res;
}

Respuesta heuristicaVecinoMasCercano(System& s) {
    vector<Route> soln;

    vector<Customer*> clientes = s.getCustomers();
    customerToCompare = s.getDepot();
    priority_queue<Customer*, vector<Customer*>, comparator> cercanos_al_deposito(clientes.begin()+1, clientes.end());

    Customer* cliente1 = cercanos_al_deposito.top(); cercanos_al_deposito.pop();
    Route ruta_actual; ruta_actual.addCustomer(*cliente1);
    cliente1->isVisited();

    while (true) {
        Customer* mas_cercano;
        double menor_distancia = -1;

        for (Customer* cliente2 : clientes) {
            if (
                    cliente2->isDepot() ||
                    cliente2 == cliente1 ||
                    cliente2->getstatus()
                    ) continue;

            double dist = EuclideanDist(*cliente1, *cliente2);
            if (dist < menor_distancia || menor_distancia == -1) {
                menor_distancia = dist;
                mas_cercano = cliente2;
            }
        }

        if (menor_distancia != -1 && ruta_actual.getActualWeight() + mas_cercano->getDemand() < s.getCapacity()) {
            ruta_actual.addCustomer(*mas_cercano);
            mas_cercano->isVisited();
        } else {
            soln.push_back(ruta_actual);
            ruta_actual = Route();
            while (cliente1->getstatus() && !cercanos_al_deposito.empty()) {
                cliente1 = cercanos_al_deposito.top(); cercanos_al_deposito.pop();
            }
            if (cliente1->getstatus()) break;
            ruta_actual.addCustomer(*cliente1);
            cliente1->isVisited();
        }
    }

    Respuesta res(soln, &s);
    return res;
}