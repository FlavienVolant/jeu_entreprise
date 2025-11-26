#include "entreprise.h"

#include "commercial.h"
#include "outils_de_production.h"
#include "etude_de_marche.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
    mois->nb_hydro_boat = 0;
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
           mois->nb_hydro_boat,
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
           op->name, type_str, mois_str, signe * op->value_month);
}

void display_operations_annee_mois(const Entreprise *entreprise, int annee, int mois){
    
    printf("+--------------------------------+\n");
    printf("| Annee: %-1d, Mois: %-2d             |\n", annee, mois);
    printf("+--------------------------------+----------+----------------------------+----------------------+\n");
    printf("| Nom                            | Type     | Mois d'application         | Valeur               |\n");
    printf("+--------------------------------+----------+----------------------------+----------------------+\n");

    int vrai_mois = (annee + 1) * mois;
    for (int i = 0; i < entreprise->nb_operation; i++) {

        Operation op = entreprise->operations[i];

        if(op.mois_application[vrai_mois] == 1)
            __display_operation(&op, annee);
    }

    printf("+--------------------------------+----------+----------------------------+----------------------+\n");
}

void display_operations(const Entreprise *entreprise, int annee) {

    printf("+--------------------------------+\n");
    printf("| Annee: %-1d                       |\n", annee);
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
    
    for (int m = 0; m < NB_MOIS_DANS_ANNEE * NB_ANNEE_JOUE; m++) {
        if (operation.mois_application[m]) {
            Mois *mois = &entreprise->mois[m];

            if (operation.type == OPERATION_BENEF)
                mois->benef += operation.value_month;
            else if (operation.type == OPERATION_DEPENSE)
                mois->depense += operation.value_month;
        }
    }
}

void stop_operation(Entreprise *entreprise, char *op_name, int end){
    Operation *op = get_operation_by_name(entreprise, op_name);
    if(op == NULL) return;

    int nb_mois_total = NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE;

    for (int m = end; m < nb_mois_total; m++) {
        if (op->mois_application[m]) {
            op->mois_application[m] = 0;

            Mois *mois = &entreprise->mois[m];
            if (op->type == OPERATION_BENEF)
                mois->benef -= op->value_month;
            else if (op->type == OPERATION_DEPENSE)
                mois->depense -= op->value_month;
        }
    }
}

void acheter_une_machine(Entreprise *entreprise, int mois_command) {
    
    Operation achat;
    achat.name = "Achat machine";
    achat.mois_creation = mois_command;
    achat.type = OPERATION_DEPENSE;
    achat.value_month = PRIX_ACHAT_MACHINE * TVA;
    set_mois_application(&achat, mois_command + 1, mois_command + 2);

    Operation fonctionnement;
    fonctionnement.name = "Cout fixe machine";
    fonctionnement.mois_creation = mois_command + 1;
    fonctionnement.type = OPERATION_DEPENSE;
    fonctionnement.value_month = COUT_FIXE_PAR_MACHINE;
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
    op.mois_creation = mois_vente;
    op.type = OPERATION_BENEF;
    op.value_month = PRIX_VENTE_MACHINE;
    set_mois_application(&op, mois_vente + 1, mois_vente + 2);

    add_operation(entreprise, op);

    for(int i = mois_vente + 1; i < NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE; i++) {
        entreprise->mois[i].nb_machine -= 1;
    }

    stop_operation(entreprise, "Cout fixe machine", mois_vente + 1);
}

void __produire(Entreprise *entreprise, const Nomenclature *product, int mois_production, int qt) {
    int qt_aluminiums = qt * product->conso_aluminium;
    int qt_accessoires = qt * product->conso_accessoires;
    int temps_production = qt  * product->temps_de_production;
    
    for(int i = mois_production; i < NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE; i++) {
        entreprise->mois[i].nb_aluminium -= qt_aluminiums;
        entreprise->mois[i].nb_accessoire -= qt_accessoires;
        switch (product->type){
            case TYPE_ULTRA_CHAR:
                entreprise->mois[i].nb_ultra_char += qt;
                break;
            case TYPE_HYDRO_BOAT:
                entreprise->mois[i].nb_hydro_boat += qt;
                break;
            default:
                break;
        }
    }

    Operation coutVariable;
    coutVariable.name = "Fabrication 9e/h";
    coutVariable.mois_creation = mois_production;
    coutVariable.type = OPERATION_DEPENSE;
    coutVariable.value_month = COUT_VARIABLE_PAR_HEURE * temps_production * TVA;
    set_mois_application(&coutVariable, mois_production, mois_production + 1);

    add_operation(entreprise, coutVariable);
}

void produire_ultra_char(Entreprise *entreprise, int mois_production, int qt){
    __produire(entreprise, &ultra_char, mois_production, qt);
}

void produire_hydro_boat(Entreprise *entreprise, int mois_production, int qt){
    __produire(entreprise, &hydro_boat, mois_production, qt);
}

void vendre_ultra_char(Entreprise *entreprise, int mois_vente, int delai_de_paiement, int prix_de_vente, int qt){
    float total = prix_de_vente * qt * TVA;
    float log = ultra_char.cout_logistique * qt * TVA;

    Operation logistique;
    logistique.name = "Logistique ultra char";
    logistique.mois_creation = mois_vente;
    logistique.type = OPERATION_DEPENSE;
    logistique.value_month = log;
    set_mois_application(&logistique, mois_vente, mois_vente + 1);

    Operation vente;
    vente.name = "Vente ultra char";
    vente.mois_creation = mois_vente;
    vente.type = OPERATION_BENEF;
    vente.value_month = total;

    switch (delai_de_paiement){
        case 0:
            set_mois_application(&vente, mois_vente, mois_vente + 1);
            break;
        
        case 30:
            set_mois_application(&vente, mois_vente + 1, mois_vente + 2);
            break;
        
        case 45:
            vente.value_month = total/2.0f;
            set_mois_application(&vente, mois_vente + 1, mois_vente + 3);
            break;

        case 60:
            set_mois_application(&vente, mois_vente + 2, mois_vente + 3);
            break;
    }

    add_operation(entreprise, vente);
    add_operation(entreprise, logistique);

    for(int i = mois_vente; i < NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE; i ++) {
        entreprise->mois[i].nb_ultra_char -= qt;
    }
}

void vendre_hydro_boat(Entreprise *entreprise, int mois_vente, int delai_de_paiement, int prix_de_vente, int qt){
    float total = prix_de_vente * qt * TVA;
    float log = hydro_boat.cout_logistique * qt * TVA;

    Operation logistique;
    logistique.name = "Logistique hydroboat";
    logistique.mois_creation = mois_vente;
    logistique.type = OPERATION_DEPENSE;
    logistique.value_month = log;
    set_mois_application(&logistique, mois_vente, mois_vente + 1);

    Operation vente;
    vente.name = "Vente hydro boat";
    vente.mois_creation = mois_vente;
    vente.type = OPERATION_BENEF;
    vente.value_month = total;
    switch (delai_de_paiement){
    case 0:
        set_mois_application(&vente, mois_vente, mois_vente + 1);
        break;
    
    case 30:
        set_mois_application(&vente, mois_vente + 1, mois_vente + 2);
        break;
    
    case 45:
        vente.value_month = total / 2.0f;
        set_mois_application(&vente, mois_vente + 1, mois_vente + 3);
        break;

    case 60:
        set_mois_application(&vente, mois_vente + 2, mois_vente + 3);
        break;
    }

    add_operation(entreprise, vente);
    add_operation(entreprise, logistique);

    for(int i = mois_vente; i < NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE; i ++) {
        entreprise->mois[i].nb_hydro_boat -= qt;
    }
}

void acheter_aluminium(Entreprise *entreprise, const Fournisseur *fournisseur, int mois_command, int lot){
    Operation achat;
    achat.name = "Achat aluminium";
    achat.mois_creation = mois_command;
    achat.type = OPERATION_DEPENSE;
    achat.value_month = lot * prix_lot_aluminium(fournisseur) * TVA;
    set_mois_application(&achat, mois_command + 1 + fournisseur->delai_de_paiement, mois_command + fournisseur->delai_de_paiement + 2);

    add_operation(entreprise, achat);

    for(int i = mois_command + 1; i < NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE; i++) {
        entreprise->mois[i].nb_aluminium += lot * fournisseur->taille_lot_aluminium;
    }
}

void acheter_accessoire(Entreprise *entreprise, const Fournisseur *fournisseur, int mois_command, int lot) {
    Operation achat;
    achat.name = "Achat accessoire";
    achat.mois_creation = mois_command;
    achat.type = OPERATION_DEPENSE;
    achat.value_month = lot * prix_lot_accessoires(fournisseur) * TVA;
    set_mois_application(&achat, mois_command + 1 + fournisseur->delai_de_paiement, mois_command + fournisseur->delai_de_paiement + 2);

    add_operation(entreprise, achat);

    for(int i = mois_command + 1; i < NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE; i++) {
        entreprise->mois[i].nb_accessoire += lot * fournisseur->taille_lot_accessoires;
    }
}

void embaucher_commercial(Entreprise *entreprise, int mois_embauche) {
    Operation embauche;
    embauche.name = "Embauche commercial";
    embauche.mois_creation = mois_embauche;
    embauche.type = OPERATION_DEPENSE;
    embauche.value_month = COMMERCIAL_COUT_RECRUTEMENT;
    set_mois_application(&embauche, mois_embauche, mois_embauche + 1);

    Operation salaire;
    salaire.name = "Salaire commercial";
    salaire.mois_creation = mois_embauche + 1;
    salaire.type = OPERATION_DEPENSE;
    salaire.value_month = COMMERCIAL_SALAIRE_MENSUEL;
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
    op.mois_creation = mois_licenciement;
    op.type = OPERATION_DEPENSE;
    op.value_month = COMMERCIAL_COUT_LICENCIEMENT;
    set_mois_application(&op, mois_licenciement, mois_licenciement + 1);

    add_operation(entreprise, op);

    for(int i = mois_licenciement + 1; i < NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE; i++) {
        entreprise->mois[i].nb_commerciaux -= 1;
    }

    stop_operation(entreprise, "Salaire commercial", mois_licenciement + 1);
}

void etude_marche_sensibilite_client(Entreprise *entreprise, int mois_achat) {
    Operation op;
    op.name = "Etude sensibilite client";
    op.mois_creation = mois_achat;
    op.type = OPERATION_DEPENSE;
    op.value_month = ETUDE_SENSIBILITE_CLIENTS * TVA;
    set_mois_application(&op, mois_achat, mois_achat + 1);

    add_operation(entreprise, op);
}

void etude_marche_pub(Entreprise *entreprise, int mois_achat) {
    Operation op;
    op.name = "Etude marche pub";
    op.mois_creation = mois_achat;
    op.type = OPERATION_DEPENSE;
    op.value_month = ETUDE_PUBLICITE * TVA;
    set_mois_application(&op, mois_achat, mois_achat + 1);

    add_operation(entreprise, op);
}

void pubs(Entreprise *entreprise, int mois_achat, int valeur) {
    Operation op;
    op.name = "Pubs";
    op.mois_creation = mois_achat;
    op.type = OPERATION_DEPENSE;
    op.value_month = valeur * TVA;
    set_mois_application(&op, mois_achat, mois_achat + 1);

    add_operation(entreprise, op);
}

/**
 * taux: in %
 * duree: in month, mois_emptrunts + duree is the last month to repay
 */
void emprunts(Entreprise *entreprise, float montant, float taux, int mois_emprunts, int duree){

    Operation emprunt = {
        .name = "Emprunt",
        .type = OPERATION_BENEF,
        .mois_creation = mois_emprunts,
        .value_month = montant,
    };

    set_mois_application(&emprunt, mois_emprunts, mois_emprunts + 1);

    float value = montant * (1 + taux / 100) / duree;
    if(taux != 0) {
        float r = taux / 100.0f;
        float i = powf(1.0f + r, 1.0f/12.0f) - 1.0f;
        value = montant * i / (1.0f - powf(1.0f + i, - duree));
    }

    Operation repay = {
        .name = "Remboursement",
        .type = OPERATION_DEPENSE,
        .mois_creation = mois_emprunts,
        .value_month = value,
    };

    set_mois_application(&repay, mois_emprunts, mois_emprunts + duree);

    add_operation(entreprise, emprunt);
    add_operation(entreprise, repay);
}

void autre_entree(Entreprise *entreprise, float montant, int mois_arrive){
    Operation entree = {
        .name = "Autre entree",
        .type = OPERATION_BENEF,
        .mois_creation = mois_arrive,
        .value_month = montant,
    };
    set_mois_application(&entree, mois_arrive, mois_arrive + 1);
    add_operation(entreprise, entree);     
}
