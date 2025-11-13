#include "fournisseurs.h"

float prix_lot_accessoires(const Fournisseur *fournisseur) {
    return fournisseur->prix_accessoires * fournisseur->taille_lot_accessoires;
}

float prix_lot_aluminium(const Fournisseur *fournisseur) {
    return fournisseur->prix_aluminium * fournisseur->taille_lot_aluminium;
}

const Fournisseur AluStar = {
    .prix_accessoires = 10.0f,
    .taille_lot_accessoires = 5000,
    .prix_aluminium = 2.8f,
    .taille_lot_aluminium = 20000,
    .delai_de_paiement = 0,
};

const Fournisseur FourniStyl = {
    .prix_accessoires = 11.5f,
    .taille_lot_accessoires = 2000,
    .prix_aluminium = 3.5f,
    .taille_lot_aluminium = 5000,
    .delai_de_paiement = 1,
};

const Fournisseur Topmatex = {
    .prix_accessoires = 12.5f,
    .taille_lot_accessoires = 1000,
    .prix_aluminium = 4.0f,
    .taille_lot_aluminium = 1000,
    .delai_de_paiement = 2,
};