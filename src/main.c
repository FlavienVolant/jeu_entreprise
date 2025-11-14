#include "stdio.h"
#include "entreprise.h"
#include "outils_de_production.h"
#include "fournisseurs.h"

int main(){

    Entreprise entreprise;
    init_entreprise(&entreprise);

    Operation capital_depart = {
        .name = "Capital de depart",
        .type = OPERATION_BENEF,
        .mois_creation = 0,
        .value = 500000,
    };
    set_mois_application(&capital_depart, 0, 1);

    Operation terain_locaux = {
        .name = "terrain_locaux",
        .type = OPERATION_DEPENSE,
        .mois_creation = 0,
        .value = 160000,
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
        .value = 18000 * NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE, // pour avoir 18000 par mois
    };

    set_mois_application(&frais_de_structure, 0, NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE);

    add_operation(&entreprise, capital_depart);
    add_operation(&entreprise, terain_locaux);
    add_operation(&entreprise, aide_etat);
    add_operation(&entreprise, remboursement_aide_etat);
    add_operation(&entreprise, frais_de_structure);
    
    acheter_une_machine(&entreprise, 0);
    embaucher_commercial(&entreprise, 0);
    embaucher_commercial(&entreprise, 0);
    etude_marche_sensibilite_client(&entreprise, 0);
    etude_marche_sensibilite_client(&entreprise, 0);
    pubs(&entreprise, 0, 12000);

    acheter_accessoire(&entreprise, &FourniStyl, 1, 2);
    acheter_aluminium(&entreprise, &FourniStyl, 1, 2);
    produire_ultra_char(&entreprise, 2, 500);

    vendre_ultra_char(&entreprise, 3, 45, 35, 300);

    display_operations(&entreprise, 2);
    display_operations_annee_mois(&entreprise, 0, 1);
    display_entreprise(&entreprise, 0, 0);

    return 0;
}