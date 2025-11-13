#ifndef FOURNISSEURS_H
#define FOURNISSEURS_H

typedef struct {
    float prix_accessoires;
    int taille_lot_accessoires;

    float prix_aluminium;
    int taille_lot_aluminium;

    int delai_de_paiement;
} Fournisseur;

float prix_lot_accessoires(Fournisseur fournisseur) {
    return fournisseur.prix_accessoires * fournisseur.taille_lot_accessoires;
}

float prix_lot_aluminium(Fournisseur fournisseur) {
    return fournisseur.prix_aluminium * fournisseur.taille_lot_aluminium;
}

const Fournisseur AluStar {
    .prix_accessoires = 10.0f,
    .taille_lot_accessoires = 5000,
    .prix_aluminium = 2.8f,
    .taille_lot_aluminium = 20000,
    .delai_de_paiement = 0,
};

const Fournisseur FourniStyl {
    .prix_accessoires = 11.5f,
    .taille_lot_accessoires = 2000,
    .prix_aluminium = 3.5f,
    .taille_lot_aluminium = 5000,
    .delai_de_paiement = 30,
};

const Fournisseur Topmatex {
    .prix_accessoires = 12.5f,
    .taille_lot_accessoires = 1000,
    .prix_aluminium = 4.0f,
    .taille_lot_aluminium = 1000,
    .delai_de_paiement = 60,
};

#endif