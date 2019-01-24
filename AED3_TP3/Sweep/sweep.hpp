#ifndef SWEEP_HPP
#define SWEEP_HPP

#include "basics3.hpp"

//Cargar los datos
void cargarDatos(ListaCordPol& lcp, vector<Nodo>& vn, float& capacidad, string& nombreCaso, int& kopt, float& costoOpt);

//Con esto hacemos los clusters del sweep
vector<vector<Nodo>> generarClusters(ListaCordPol& lcp, const vector<Nodo>& vn, float capacidad);

//Resuelvo caminos con TSP
vector<Nodo> tsp2(vector<Nodo>& nodos, int nodo_comienzo, float &costo_viaje, float p, float MAX_X, float MAX_Y);

vector<Nodo> tsp_con_grasp(vector<Nodo>& nodos, int nodo_comienzo, float &costo_viaje, float p, float MAX_X, float MAX_Y);

vector<Nodo> opt_swap2(vector<Nodo>& solucion_actual, int tope1, int tope2, float& costo_solucion);

void generarOutput(const vector<vector<Nodo>>& clusters, string nombre, Nodo depo);

#endif // SWEEP_HPP
