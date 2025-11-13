#include "entreprise.h"

#include "commercial.h"
#include "outils_de_production.h"
#include "etude_de_marche.h"

#include <stdio.h>
#include <stdlib.h>

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

void __display_mois(const Mois *mois)
{
    printf("| %-5s | %15.2f | %15.2f | %10d | %10d | %15d | %15d | %10d | %10d |\n",
           mois->nom,
           mois->benef,
           mois->depense,
           mois->nb_ultra_char,
           mois->nb_hydroboat,
           mois->nb_accessoire,
           mois->nb_aluminium,
           mois->nb_machine,
           mois->nb_commerciaux);
}

void display_entreprise(const Entreprise *entreprise, int annee)
{
    printf("+-------+-----------------+-----------------+------------+------------+-----------------+-----------------+------------+------------+\n");
    printf("| Mois  |      Benef      |    Depenses     | Ultra Char | Hydro Boat |   Accessoire    |   Aluminium     |  Machine   | Commerciaux|\n");
    printf("+-------+-----------------+-----------------+------------+------------+-----------------+-----------------+------------+------------+\n");

    int start = annee * NB_MOIS_DANS_ANNEE;
    int end = start + NB_MOIS_DANS_ANNEE;
    for (int i = start; i < end; i++) {
        __display_mois(&entreprise->mois[i]);
    }

    printf("+-------+-----------------+-----------------+------------+------------+-----------------+-----------------+------------+------------+\n");
}

void __display_operation(const Operation *op, int annee)
{

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

void display_operations(const Entreprise *entreprise, int annee)
{
    printf("+--------------------------------+----------+----------------------------+----------------------+\n");
    printf("| Nom                            | Type     | Mois d'application         | Valeur               |\n");
    printf("+--------------------------------+----------+----------------------------+----------------------+\n");

    for (int i = 0; i < entreprise->nb_operation; i++) {
        __display_operation(&entreprise->operations[i], annee);
    }

    printf("+--------------------------------+----------+----------------------------+----------------------+\n");
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

void set_mois_application(Operation *op, int start, int end)
{
    for (int i = 0; i < NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE; i++) {
        op->mois_application[i] = 0;
    }


    if (start < 0) start = 0;
    if (end > NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE) end = NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE;

    for (int i = start; i < end; i++) {
        op->mois_application[i] = 1;
    }
}

void acheter_une_machine(Entreprise *entreprise, int mois_command)
{
    Operation op;
    op.name = "Achat machine";
    snprintf(op.desc, sizeof(op.desc), "Achat machine fait le mois %d, paiement le mois %d", mois_command, mois_command + 1);
    op.mois_creation = mois_command;
    op.type = OPERATION_DEPENSE;
    op.value = PRIX_ACHAT_MACHINE;
    set_mois_application(&op, mois_command + 1, mois_command + 2);

    add_operation(entreprise, op);


    for(int i = mois_command + 1; i < NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE; i++) {
        entreprise->mois[i].nb_machine += 1;
    }
}

void vendre_une_machine(Entreprise *entreprise, int mois_vente)
{
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
}

void embaucher_commercial(Entreprise *entreprise, int mois_embauche)
{
    Operation op;
    op.name = "Embauche commercial";
    snprintf(op.desc, sizeof(op.desc), "Embauche faite le mois %d, coût initial %d€", mois_embauche, COMMERCIAL_COUT_RECRUTEMENT);
    op.mois_creation = mois_embauche;
    op.type = OPERATION_DEPENSE;
    op.value = COMMERCIAL_COUT_RECRUTEMENT;
    set_mois_application(&op, mois_embauche, mois_embauche + 1);

    add_operation(entreprise, op);

    for(int i = mois_embauche + 1; i < NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE; i++) {
        entreprise->mois[i].nb_commerciaux += 1;
    }
}

void licencier_commercial(Entreprise *entreprise, int mois_licenciement)
{
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
}

void etude_marche_sensibilite_client(Entreprise *entreprise, int mois_achat)
{
    Operation op;
    op.name = "Etude sensibilite client";
    snprintf(op.desc, sizeof(op.desc), "Etude sensibilite client fait le mois %d, coût %d€", mois_achat, ETUDE_SENSIBILITE_CLIENTS);
    op.mois_creation = mois_achat;
    op.type = OPERATION_DEPENSE;
    op.value = ETUDE_SENSIBILITE_CLIENTS;
    set_mois_application(&op, mois_achat, mois_achat + 1);

    add_operation(entreprise, op);
}

void etude_marche_pub(Entreprise *entreprise, int mois_achat)
{
    Operation op;
    op.name = "Etude marche pub";
    snprintf(op.desc, sizeof(op.desc), "Etude marche pub fait le mois %d, coût %d€", mois_achat, ETUDE_PUBLICITE);
    op.mois_creation = mois_achat;
    op.type = OPERATION_DEPENSE;
    op.value = ETUDE_PUBLICITE;
    set_mois_application(&op, mois_achat, mois_achat + 1);

    add_operation(entreprise, op);
}

void pubs(Entreprise *entreprise, int mois_achat, int valeur)
{
    Operation op;
    op.name = "Pubs";
    snprintf(op.desc, sizeof(op.desc), "Pubs faite le mois %d, coût %d€", mois_achat, valeur);
    op.mois_creation = mois_achat;
    op.type = OPERATION_DEPENSE;
    op.value = valeur;
    set_mois_application(&op, mois_achat, mois_achat + 1);

    add_operation(entreprise, op);
}
