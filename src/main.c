#include "stdio.h"
#include "entreprise.h"
#include "outils_de_production.h"
#include "fournisseurs.h"

int main(){

    Entreprise entreprise;
    init_entreprise(&entreprise);

    // mois Janvier

    // ajout des 3000 ultra char debut
    for(int i = 0; i < NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE; i++) {
        entreprise.mois[i].nb_ultra_char += 3000;
    }

    Operation terain_locaux = {
        .name = "terrain_locaux",
        .type = OPERATION_DEPENSE,
        .mois_creation = 0,
        .value = 160000 * TVA,
    };

    set_mois_application(&terain_locaux, 0, 1);

    Operation aide_etat = {
        .name = "Aide",
        .type = OPERATION_BENEF,
        .mois_creation = 0,
        .value = 90000,
    };

    set_mois_application(&aide_etat, 0, 1);

    Operation remboursement_aide_etat = {
        .name = "remboursement Aide sur 5 ans",
        .type = OPERATION_DEPENSE,
        .mois_creation = 0,
        .value = 90000,
    };

    set_mois_application(&remboursement_aide_etat, 0, NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE);

    Operation frais_de_structure = {
        .name = "Frais de structure",
        .type = OPERATION_DEPENSE,
        .mois_creation = 0,
        .value = COUT_FIXE_STRUCTURE * TVA * NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE, // pour avoir COUT_FIXE_STRUCTURE par mois
    };

    set_mois_application(&frais_de_structure, 0, NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE);

    add_operation(&entreprise, terain_locaux);
    add_operation(&entreprise, aide_etat);
    add_operation(&entreprise, remboursement_aide_etat);
    add_operation(&entreprise, frais_de_structure);
    
    acheter_une_machine(&entreprise, 0);
    embaucher_commercial(&entreprise, 0);
    embaucher_commercial(&entreprise, 0);
    etude_marche_sensibilite_client(&entreprise, 0);
    etude_marche_pub(&entreprise, 0);
    pubs(&entreprise, 0, 12000);

    acheter_accessoire(&entreprise, &FourniStyl, 0, 2);
    acheter_aluminium(&entreprise, &FourniStyl, 0, 2);

    vendre_ultra_char(&entreprise, 0, 45, 43, 3000);

    // mois Fevrier

    Operation emprunt_machine = {
        .name = "Emprunt Machine",
        .type = OPERATION_BENEF,
        .mois_creation = 1,
        .value = 60000,
    };

    set_mois_application(&emprunt_machine, 1, 2);
    add_operation(&entreprise, emprunt_machine);

    Operation remboursement_emprunt_machine = {
        .name = "Emprunt Machine sur 5 ans",
        .type = OPERATION_DEPENSE,
        .mois_creation = 1,
        .value = 60000 * 1.04f,
    };

    set_mois_application(&remboursement_emprunt_machine, 1, NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE);
    add_operation(&entreprise, remboursement_emprunt_machine);

    Operation remboursement_tva_1 = {
        .name = "Remboursement TVA",
        .type = OPERATION_BENEF,
        .mois_creation = 1,
        .value = 33400,
    };

    set_mois_application(&remboursement_tva_1, 1, 2);
    add_operation(&entreprise, remboursement_tva_1);

    produire_ultra_char(&entreprise, 1, 2000);
    vendre_ultra_char(&entreprise, 1, 45, 60, 1606);
    pubs(&entreprise, 1, 15000);

    acheter_accessoire(&entreprise, &AluStar, 1, 1);
    acheter_aluminium(&entreprise, &FourniStyl, 1, 2);
    acheter_une_machine(&entreprise, 1);
    // produire_ultra_char(&entreprise, 2, 500);

    // mois Mars
    vendre_ultra_char(&entreprise, 2, 45, 50, 2383);
    produire_ultra_char(&entreprise, 2, 4000);

    acheter_accessoire(&entreprise, &FourniStyl, 2, 1);
    acheter_aluminium(&entreprise, &FourniStyl, 2, 1);

    pubs(&entreprise, 2, 20000);

    Operation remboursement_tva_2 = {
        .name = "Remboursement TVA",
        .type = OPERATION_BENEF,
        .mois_creation = 1,
        .value = 13234,
    };
    set_mois_application(&remboursement_tva_2, 2, 3);
    add_operation(&entreprise, remboursement_tva_2);

    Operation marchandise_janvier = {
        .name = "Paiement des marchandises",
        .type = OPERATION_DEPENSE,
        .mois_creation = 2,
        .value = 108000 - 22400, // todo fix
    };

    set_mois_application(&marchandise_janvier, 2, 3);
    add_operation(&entreprise, marchandise_janvier);

    // mois Avril
    vendre_ultra_char(&entreprise, 3, 45, 30, 2011);
    pubs(&entreprise, 3, 10000);
    pubs(&entreprise, 3, 5000);
    etude_marche_pub(&entreprise, 3);

    Operation inflows = {
        .name = "Other inflows",
        .type = OPERATION_BENEF,
        .mois_creation = 3,
        .value = 121153,
    };
    set_mois_application(&inflows, 3, 4);
    add_operation(&entreprise, inflows);    

    Operation tva_hot_fix = {
        .name = "tva_hot_fix",
        .type = OPERATION_DEPENSE,
        .mois_creation = 3,
        .value = 7000,
    };
    set_mois_application(&tva_hot_fix, 3, 4);
    add_operation(&entreprise, tva_hot_fix);    

    produire_hydro_boat(&entreprise, 3, 200);

    acheter_accessoire(&entreprise, &FourniStyl, 3, 1);

    // mois mai

    produire_ultra_char(&entreprise, 4, 2200);
    acheter_aluminium(&entreprise, &AluStar, 4, 1);
    acheter_accessoire(&entreprise, &AluStar, 4, 1);

    pubs(&entreprise, 4, 5000);
    pubs(&entreprise, 4, 15000);

    vendre_ultra_char(&entreprise, 4, 30, 47, 2200);
    vendre_hydro_boat(&entreprise, 4, 30, 700, 200);

    display_operations(&entreprise, 0);
    display_entreprise(&entreprise, 0, 500000);
    //display_operations_annee_mois(&entreprise, 0, 3);

    return 0;
}