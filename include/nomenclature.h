#ifndef NOMENCLATURE_H
#define NOMENCLATURE_H

typedef struct {
    float temps_de_production;
    float conso_accessoires;
    float conso_aluminium;
    float cout_logistique;
} Nomenclature;

const Nomenclature N_ULTRA_CHAR = {
    .temps_de_production = 0.5f,
    .conso_accessoires = 1.0f,
    .conso_aluminium = 2.0f,
    .cout_logistique = 5.0f
};

const Nomenclature N_HYDRO_BOAT = {
    .temps_de_production = 10.0f,
    .conso_accessoires = 6.0f,
    .conso_aluminium = 18.0f,
    .cout_logistique = 45.0f
};

#endif