#ifndef NOMENCLATURE_H
#define NOMENCLATURE_H

typedef struct {
    float temps_de_production;
    int conso_accessoires;
    int conso_aluminium;
    int cout_logistique;
} Nomenclature;

extern const Nomenclature ultra_char;
extern const Nomenclature hydro_boat;

#endif