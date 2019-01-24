#include <iostream>
#include <cstring>
#include <chrono>
#include "VMCs.h"
using namespace std;

#define medir_tiempo(K, CODIGO) \
    [&] () -> double {\
        double tiempo_promedio = 0.0;\
        for (int i = 0; i < K; ++i)\
        {\
            auto tiempo_inicio = chrono::steady_clock::now();\
            CODIGO \
            auto tiempo_fin = chrono::steady_clock::now();\
            tiempo_promedio += chrono::duration<double, milli>(tiempo_fin - tiempo_inicio).count();\
        }\
        return tiempo_promedio / (double)K;\
    }();

#define isAlgorithm(cmp) strcmp(algorithm, cmp) == 0 || strcmp(algorithm,"all") == 0

void execute(const char* filename, bool verbose = true, const char* algorithm = "all") {

    // Leo los datos de archivo o de consola
    if (filename != NULL) {
        if (verbose)
            cout << "Procesando datos del archivo " << filename << endl;
        freopen(filename, "r", stdin);
    }

    string dummy, name, comment, type, edge_weight_type;
    unsigned int dimension, capacity;

    cin >> dummy >> dummy; cin.ignore(1); getline(cin, name);
    cin >> dummy >> dummy; cin.ignore(1); getline(cin, comment);
    cin >> dummy >> dummy; cin.ignore(1); getline(cin, type);
    cin >> dummy >> dummy >> dimension;
    cin >> dummy >> dummy; cin.ignore(1); getline(cin, edge_weight_type);
    cin >> dummy >> dummy >> capacity;
    cin >> dummy;

    System s(dimension);
    s.setCapacity(capacity);

    for (int i = 0; i < dimension; ++i) {
        unsigned int id; double x, y;
        cin >> id >> x >> y;
        s.addCustomer(id, x, y);
    }
    cin >> dummy;
    for (int i = 0; i < dimension; ++i) {
        unsigned int id, dim;
        cin >> id >> dim;
        s.setDemand(id, dim);
    }

    if (filename != NULL) {
        fclose(stdin);
    }
    // Calculo los tiempos
    double t1,t2,t3,t4;
    if (isAlgorithm("Savings"))
        t1 = medir_tiempo(2, heuristicaSavings(s); s.restart(); );
    if (isAlgorithm("MCD"))
        t2 = medir_tiempo(2, heuristicaMasCercanoAlDeposito(s); s.restart(); );
    if (isAlgorithm("VMC"))
        t3 = medir_tiempo(2, heuristicaVecinoMasCercano(s); s.restart(); );

    if (verbose) {
        if (isAlgorithm("Savings")) {
            cout << "Tiempo de Savings: " << t1 << " milisegundos" << endl;
            Respuesta resSavings = heuristicaSavings(s); s.restart();
            cout << "Resultado de Savings: " << endl;
            resSavings.mostrarRespuesta();
        }
        if (isAlgorithm("MCD")) {
            cout << "Tiempo de MCD: " << t2 << " milisegundos" << endl;
            Respuesta resMCD = heuristicaMasCercanoAlDeposito(s); s.restart();
            cout << "Resultado de MCD: " << endl;
            resMCD.mostrarRespuesta();
        }
        if (isAlgorithm("VMC")) {
            cout << "Tiempo de VMC: " << t3 << " milisegundos" << endl;
            Respuesta resVMC = heuristicaVecinoMasCercano(s); s.restart();
            cout << "Resultado de VMC: " << endl;
            resVMC.mostrarRespuesta();
        }
    } else {
        // Guardo los tiempos en archivos
        FILE* f1 = fopen("TiemposSavings", "a");
        FILE* f2 = fopen("TiemposMCD", "a");
        FILE* f3 = fopen("TiemposVMC", "a");
        if (isAlgorithm("Savings"))
            fprintf(f1, "%f\n", t1);
        fclose(f1);
        if (isAlgorithm("MCD"))
            fprintf(f2, "%f\n", t2);
        fclose(f2);
        if (isAlgorithm("VMC"))
            fprintf(f3, "%f\n", t3);
        fclose(f3);
    }
}

int main() {

    execute(NULL);

    return 0;
}