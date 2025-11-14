#ifndef ENTREPRISE_H
#define ENTREPRISE_H

#include "fournisseurs.h"
#include "nomenclature.h"

#define TVA 1.2
#define FRAIS_DE_STRUCTURE 18000
#define NB_ANNEE_JOUE 5
#define NB_MOIS_DANS_ANNEE 12

typedef struct {
    char *nom;
    float benef;
    float depense;
    int nb_ultra_char;
    int nb_hydro_boat;
    int nb_accessoire;
    int nb_aluminium;
    int nb_machine;
    int nb_commerciaux;
} Mois;

#define OPERATION_BENEF 0
#define OPERATION_DEPENSE 1
#define OPERATION_LEN 256

typedef struct {
    char *name;
    char desc[128];
    int type; // OPERATION_BENEF OPERATION_DEPENSE
    int mois_creation;
    int mois_application[NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE];
    float value;
} Operation;

typedef struct {
    Mois mois[NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE];
    Operation operations[OPERATION_LEN];
    int nb_operation;
} Entreprise;


void init_entreprise(Entreprise *Entreprise);
void init_mois(Mois *mois, char *nom);

float get_balance(const Mois *mois);

void __display_mois(const Mois *mois, float* capital);
void display_entreprise(const Entreprise *entreprise, int annee, float capital);

void __display_operation(const Operation *op, int annee);
void display_operations(const Entreprise *entreprise, int annee);
Operation *get_operation_by_name(Entreprise *entreprise, const char *name);

void set_mois_application(Operation *op, int start, int end);
void add_operation(Entreprise *entreprise, Operation operation);
void stop_operation(Entreprise *entreprise, char *op_name, int end);

void acheter_une_machine(Entreprise *entreprise, int mois_command);
void vendre_une_machine(Entreprise *entreprise, int mois_vente);

void produire_ultra_char(Entreprise *entreprise, int mois_production, int qt);
void produire_hydro_boat(Entreprise *entreprise, int mois_production, int qt);

void vendre_ultra_char(Entreprise *entreprise, int mois_vente, int delai_de_paiement, int prix_de_vente, int qt);
void vendre_hydro_boat(Entreprise *entreprise, int mois_vente, int delai_de_paiement, int prix_de_vente, int qt);

void acheter_aluminium(Entreprise *entreprise, const Fournisseur *fournisseur, int mois_command, int lot);
void acheter_accessoire(Entreprise *entreprise, const Fournisseur *fournisseur, int mois_command, int lot);

void embaucher_commercial(Entreprise *entreprise, int mois_embauche);
void licencier_commercial(Entreprise *entreprise, int mois_licenciement);

void etude_marche_sensibilite_client(Entreprise *entreprise, int mois_achat);
void etude_marche_pub(Entreprise *entreprise, int mois_achat);

void pubs(Entreprise *entreprise, int mois_achat, int valeur);

#endif