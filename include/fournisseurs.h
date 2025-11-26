#ifndef FOURNISSEURS_H
#define FOURNISSEURS_H

typedef struct {
    float prix_accessoires;
    int taille_lot_accessoires;

    float prix_aluminium;
    int taille_lot_aluminium;

    int delai_de_paiement;
} Fournisseur;

float prix_lot_accessoires(const Fournisseur *fournisseur);
float prix_lot_aluminium(const Fournisseur *fournisseur);

extern Fournisseur AluStar;
extern Fournisseur FourniStyl;
extern Fournisseur Topmatex;

#endif