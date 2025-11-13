#ifndef USER_H
#define USER_H

#define TVA 1.2
#define FRAIS_DE_STRUCTURE 18000
#define NB_ANNEE_JOUE 5
#define NB_MOIS_DANS_ANNEE 12

typedef struct {
    char *nom;
    float benef;
    float depense;
    int nb_ultra_char;
    int nb_hydroboat;
    int nb_accessoire;
    int nb_aluminium;
    int nb_machine;
    int nb_commerciaux;
} Mois;

#define OPERATION_BENEF 0
#define OPERATION_DEPENSE 1

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
    Operation operations[256];
    int nb_operation;
} Entreprise;


void init_entreprise(Entreprise *Entreprise);
void init_mois(Mois *mois, char *nom);

void __display_mois(const Mois *mois);
void display_entreprise(const Entreprise *entreprise, int annee);

void __display_operation(const Operation *op, int annee);
void display_operations(const Entreprise *entreprise, int annee);

void set_mois_application(Operation *op, int start, int end);
void add_operation(Entreprise *entreprise, Operation operation);

void acheter_une_machine(Entreprise *entreprise, int mois_command);
void vendre_une_machine(Entreprise *entreprise, int mois_vente);

void embaucher_commercial(Entreprise *entreprise, int mois_embauche);
void licencier_commercial(Entreprise *entreprise, int mois_licenciement);

#endif