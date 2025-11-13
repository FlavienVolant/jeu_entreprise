#include "entreprise.h"

#include "commercial.h"
#include "outils_de_production.h"
#include "etude_de_marche.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

void init_entreprise(Entreprise *entreprise){
    char* nom_mois[NB_MOIS_DANS_ANNEE] = {"Jan", "Fev", "Mar", "Avr", "Mai", "Juin", "Juil", "Aout", "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE; i++) {
        Mois current;
        init_mois(&current, nom_mois[i % NB_MOIS_DANS_ANNEE]);
        entreprise->mois[i] = current;
    }
    
    entreprise->nb_operation = 0;
}

void init_mois(Mois *mois, char *nom){
    mois->nom = nom;
    mois->benef = 0;
    mois->depense = 0;
    mois->nb_ultra_char = 0;
    mois->nb_hydroboat = 0;
    mois->nb_accessoire = 0;
    mois->nb_aluminium = 0;
    mois->nb_machine = 0;
    mois->nb_commerciaux = 0;
}

float get_balance(const Mois *mois){
    return mois->benef - mois->depense;
}

void __display_mois(const Mois *mois, float *capital) {
    float balance = get_balance(mois);
    *capital += balance; // le capital augmente si balance > 0

    printf("| %-5s | %15.2f | %15.2f | %15.2f | %15.2f | %10d | %10d | %15d | %15d | %10d | %10d |\n",
           mois->nom,
           mois->benef,
           mois->depense,
           balance,
           *capital,
           mois->nb_ultra_char,
           mois->nb_hydroboat,
           mois->nb_accessoire,
           mois->nb_aluminium,
           mois->nb_machine,
           mois->nb_commerciaux);
}

void display_entreprise(const Entreprise *entreprise, int annee, float capital_depart) {
    printf("+-------+-----------------+-----------------+-----------------+-----------------+------------+------------+-----------------+-----------------+------------+------------+\n");
    printf("| Mois  |      Benef      |    Depenses     |     Balance     |     Capital     | Ultra Char | Hydro Boat |   Accessoire    |   Aluminium     |  Machine   | Commerciaux|\n");
    printf("+-------+-----------------+-----------------+-----------------+-----------------+------------+------------+-----------------+-----------------+------------+------------+\n");

    float capital = capital_depart;

    int start = annee * NB_MOIS_DANS_ANNEE;
    int end = start + NB_MOIS_DANS_ANNEE;
    for (int i = start; i < end; i++) {
        __display_mois(&entreprise->mois[i], &capital);
    }

    printf("+-------+-----------------+-----------------+-----------------+-----------------+------------+------------+-----------------+-----------------+------------+------------+\n");
}

void __display_operation(const Operation *op, int annee){

    char *type_str;
    int signe;
    if(op->type == OPERATION_BENEF){
        type_str = "Benefice";
        signe = 1;
    }else {
        type_str = "Depense ";
        signe = -1;
    }
    
    char mois_str[64];
    int pos = 0; 

    int start = annee * NB_MOIS_DANS_ANNEE;
    int end = start + NB_MOIS_DANS_ANNEE;

    for (int m = start; m < end; m++) {
        if (op->mois_application[m]) {
            if (pos > 0) mois_str[pos++] = ',';
            int mois = m % NB_MOIS_DANS_ANNEE;
            pos += sprintf(&mois_str[pos], "%d", mois);
        }
    }
    mois_str[pos] = '\0';

    printf("| %-30s | %-8s | %-26s | %20.2f |\n",
           op->name, type_str, mois_str, signe * op->value);
}

void display_operations(const Entreprise *entreprise, int annee) {
    printf("+--------------------------------+----------+----------------------------+----------------------+\n");
    printf("| Nom                            | Type     | Mois d'application         | Valeur               |\n");
    printf("+--------------------------------+----------+----------------------------+----------------------+\n");

    for (int i = 0; i < entreprise->nb_operation; i++) {
        __display_operation(&entreprise->operations[i], annee);
    }

    printf("+--------------------------------+----------+----------------------------+----------------------+\n");
}

Operation *get_operation_by_name(Entreprise *entreprise, const char *name) {   
    Operation *res = NULL;
    for(int i = 0; i < OPERATION_LEN; i++) {
        res = &entreprise->operations[i];
        if(strcmp(res->name, name) == 0)
            return res;
    }
    return NULL;
}


void set_mois_application(Operation *op, int start, int end) {
    for (int i = 0; i < NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE; i++) {
        op->mois_application[i] = 0;
    }


    if (start < 0) start = 0;
    if (end > NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE) end = NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE;

    for (int i = start; i < end; i++) {
        op->mois_application[i] = 1;
    }
}

void add_operation(Entreprise *entreprise, Operation operation) {
    if (entreprise->nb_operation >= 256) {
        printf("Impossible d'ajouter l'opération : tableau plein.\n");
        return;
    }

    entreprise->operations[entreprise->nb_operation++] = operation;
    
    int nb_mois_actifs = 0;

    for (int m = 0; m < NB_MOIS_DANS_ANNEE * NB_ANNEE_JOUE; m++) {
        if (operation.mois_application[m])
            nb_mois_actifs++;
    }
    
    if (nb_mois_actifs == 0) return;

    float value_par_mois = operation.value / nb_mois_actifs;
    
    for (int m = 0; m < NB_MOIS_DANS_ANNEE * NB_ANNEE_JOUE; m++) {
        if (operation.mois_application[m]) {
            Mois *mois = &entreprise->mois[m];

            if (operation.type == OPERATION_BENEF)
                mois->benef += value_par_mois;
            else if (operation.type == OPERATION_DEPENSE)
                mois->depense += value_par_mois;
        }
    }
}

void stop_operation(Entreprise *entreprise, char *op_name, int end){
    Operation *op = get_operation_by_name(entreprise, op_name);
    if(op == NULL) return;

    int nb_mois_total = NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE;
    int nb_mois_avant = 0;
    int nb_mois_apres = 0;

    for (int m = 0; m < nb_mois_total; m++) {
        if (op->mois_application[m])
            nb_mois_avant++;
    }

    if (nb_mois_avant == 0) return;

    float ancienne_value_par_mois = op->value / nb_mois_avant;

    for (int m = end; m < nb_mois_total; m++) {
        if (op->mois_application[m]) {
            op->mois_application[m] = 0;

            Mois *mois = &entreprise->mois[m];
            if (op->type == OPERATION_BENEF)
                mois->benef -= ancienne_value_par_mois;
            else if (op->type == OPERATION_DEPENSE)
                mois->depense -= ancienne_value_par_mois;
        }
    }

    for (int m = 0; m < nb_mois_total; m++) {
        if (op->mois_application[m])
            nb_mois_apres++;
    }

    op->value = ancienne_value_par_mois * nb_mois_apres;
}

void acheter_une_machine(Entreprise *entreprise, int mois_command) {
    
    Operation achat;
    achat.name = "Achat machine";
    snprintf(achat.desc, sizeof(achat.desc), "Achat machine fait le mois %d, paiement le mois %d", mois_command, mois_command + 1);
    achat.mois_creation = mois_command;
    achat.type = OPERATION_DEPENSE;
    achat.value = PRIX_ACHAT_MACHINE; // * TVA ?
    set_mois_application(&achat, mois_command + 1, mois_command + 2);

    Operation fonctionnement;
    fonctionnement.name = "Fonctionnement Machine";
    snprintf(achat.desc, sizeof(achat.desc), "Frais fonctionnement machine commencant le %d, cout %d", mois_command + 1, COUT_FIXE_PAR_MACHINE); // TO CHECK
    fonctionnement.mois_creation = mois_command + 1;
    fonctionnement.type = OPERATION_DEPENSE;
    fonctionnement.value = COUT_FIXE_PAR_MACHINE * (NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE - mois_command - 1);
    set_mois_application(&fonctionnement, mois_command + 1, NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE);

    add_operation(entreprise, achat);
    add_operation(entreprise, fonctionnement);

    for(int i = mois_command + 1; i < NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE; i++) {
        entreprise->mois[i].nb_machine += 1;
    }
}

void vendre_une_machine(Entreprise *entreprise, int mois_vente) {
    Operation op;
    op.name = "Vente machine";
    snprintf(op.desc, sizeof(op.desc), "Vente machine fait le mois %d, paiement le mois %d", mois_vente, mois_vente + 1);
    op.mois_creation = mois_vente;
    op.type = OPERATION_BENEF;
    op.value = PRIX_VENTE_MACHINE;
    set_mois_application(&op, mois_vente + 1, mois_vente + 2); // TO CHECK

    add_operation(entreprise, op);

    for(int i = mois_vente + 1; i < NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE; i++) { // TO CHECK
        entreprise->mois[i].nb_machine -= 1;
    }

    stop_operation(entreprise, "Fonctionnement Machine", mois_vente + 1); // TO CHECK
}

void acheter_aluminium(Entreprise *entreprise, const Fournisseur *fournisseur, int mois_command, int lot){
    Operation achat;
    achat.name = "Achat aluminium";
    snprintf(achat.desc, sizeof(achat.desc), "Achat %i lots d'alu fait le mois %d, paiement le mois %d", lot, mois_command, mois_command + fournisseur->delai_de_paiement);
    achat.mois_creation = mois_command;
    achat.type = OPERATION_DEPENSE;
    achat.value = lot * prix_lot_aluminium(fournisseur);
    set_mois_application(&achat, mois_command + fournisseur->delai_de_paiement, mois_command + fournisseur->delai_de_paiement + 1);

    add_operation(entreprise, achat);

    for(int i = mois_command + 1; i < NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE; i++) { // TO CHECK
        entreprise->mois[i].nb_aluminium += lot * fournisseur->taille_lot_aluminium;
    }
}

void acheter_accessoire(Entreprise *entreprise, const Fournisseur *fournisseur, int mois_command, int lot) {
    Operation achat;
    achat.name = "Achat accessoire";
    snprintf(achat.desc, sizeof(achat.desc), "Achat %i lots d'acc fait le mois %d, paiement le mois %d", lot, mois_command, mois_command + fournisseur->delai_de_paiement);
    achat.mois_creation = mois_command;
    achat.type = OPERATION_DEPENSE;
    achat.value = lot * prix_lot_accessoires(fournisseur);
    set_mois_application(&achat, mois_command + fournisseur->delai_de_paiement, mois_command + fournisseur->delai_de_paiement + 1);

    add_operation(entreprise, achat);

    for(int i = mois_command + 1; i < NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE; i++) { // TO CHECK
        entreprise->mois[i].nb_accessoire += lot * fournisseur->taille_lot_accessoires;
    }
}

void embaucher_commercial(Entreprise *entreprise, int mois_embauche) {
    Operation embauche;
    embauche.name = "Embauche commercial";
    snprintf(embauche.desc, sizeof(embauche.desc), "Embauche faite le mois %d, coût initial %d€", mois_embauche, COMMERCIAL_COUT_RECRUTEMENT);
    embauche.mois_creation = mois_embauche;
    embauche.type = OPERATION_DEPENSE;
    embauche.value = COMMERCIAL_COUT_RECRUTEMENT;
    set_mois_application(&embauche, mois_embauche, mois_embauche + 1);

    Operation salaire;
    salaire.name = "Salaire commercial";
    snprintf(salaire.desc, sizeof(salaire.desc), "Salaire commercial commencant le %d, cout %d€", mois_embauche + 1, COMMERCIAL_SALAIRE_MENSUEL);
    salaire.mois_creation = mois_embauche + 1;
    salaire.type = OPERATION_DEPENSE;
    salaire.value = COMMERCIAL_SALAIRE_MENSUEL * (NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE - mois_embauche - 1) ;
    set_mois_application(&salaire, mois_embauche + 1, NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE);

    add_operation(entreprise, embauche);
    add_operation(entreprise, salaire);

    for(int i = mois_embauche + 1; i < NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE; i++) {
        entreprise->mois[i].nb_commerciaux += 1;
    }
}

void licencier_commercial(Entreprise *entreprise, int mois_licenciement) {
    Operation op;
    op.name = "Licenciement commercial";
    snprintf(op.desc, sizeof(op.desc), "Licenciement fait le mois %d, coût initial %d€", mois_licenciement, COMMERCIAL_COUT_LICENCIEMENT);
    op.mois_creation = mois_licenciement;
    op.type = OPERATION_DEPENSE; // TO CHECK
    op.value = COMMERCIAL_COUT_LICENCIEMENT;
    set_mois_application(&op, mois_licenciement, mois_licenciement + 1); // TO CHECK

    add_operation(entreprise, op);

    for(int i = mois_licenciement + 1; i < NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE; i++) { // TO CHECK
        entreprise->mois[i].nb_commerciaux -= 1;
    }

    stop_operation(entreprise, "Salaire commercial", mois_licenciement + 1); // TO CHECK
}

void etude_marche_sensibilite_client(Entreprise *entreprise, int mois_achat) {
    Operation op;
    op.name = "Etude sensibilite client";
    snprintf(op.desc, sizeof(op.desc), "Etude sensibilite client fait le mois %d, coût %d€", mois_achat, ETUDE_SENSIBILITE_CLIENTS);
    op.mois_creation = mois_achat;
    op.type = OPERATION_DEPENSE;
    op.value = ETUDE_SENSIBILITE_CLIENTS;
    set_mois_application(&op, mois_achat, mois_achat + 1);

    add_operation(entreprise, op);
}

void etude_marche_pub(Entreprise *entreprise, int mois_achat) {
    Operation op;
    op.name = "Etude marche pub";
    snprintf(op.desc, sizeof(op.desc), "Etude marche pub fait le mois %d, coût %d€", mois_achat, ETUDE_PUBLICITE);
    op.mois_creation = mois_achat;
    op.type = OPERATION_DEPENSE;
    op.value = ETUDE_PUBLICITE;
    set_mois_application(&op, mois_achat, mois_achat + 1);

    add_operation(entreprise, op);
}

void pubs(Entreprise *entreprise, int mois_achat, int valeur) {
    Operation op;
    op.name = "Pubs";
    snprintf(op.desc, sizeof(op.desc), "Pubs faite le mois %d, coût %d€", mois_achat, valeur);
    op.mois_creation = mois_achat;
    op.type = OPERATION_DEPENSE;
    op.value = valeur;
    set_mois_application(&op, mois_achat, mois_achat + 1);

    add_operation(entreprise, op);
}
