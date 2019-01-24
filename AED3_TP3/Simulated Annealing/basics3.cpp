#include "../include/basics3.hpp"

float distancia_euclidea(float x1,float y1, float x2, float y2) {
    float dif_x = x1-x2;
    float dif_y = y1-y2;
    float suma_cuads = pow(dif_x,2) + pow(dif_y,2);
    return sqrt(suma_cuads);
}