#ifndef NOMENCLATURE_H
#define NOMENCLATURE_H

#define TYPE_ULTRA_CHAR 0
#define TYPE_HYDRO_BOAT 1

typedef struct {
    int type; // TYPE_ULTRA_CHAR or TYPE_HYDRO_BOAT
    float temps_de_production;
    int conso_accessoires;
    int conso_aluminium;
    int cout_logistique;
} Nomenclature;

extern const Nomenclature ultra_char;
extern const Nomenclature hydro_boat;

#endif