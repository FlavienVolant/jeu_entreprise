#include "stdio.h"
#include "entreprise.h"
#include "outils_de_production.h"
#include "fournisseurs.h"

int main(){

    Entreprise e;
    Entreprise *entreprise = &e;
    init_entreprise(entreprise);

    // mois Janvier

    // ajout des 3000 ultra char debut
    for(int i = 0; i < NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE; i++) {
        entreprise->mois[i].nb_ultra_char += 3000;
    }

    Operation terain_locaux = {
        .name = "terrain_locaux",
        .type = OPERATION_DEPENSE,
        .mois_creation = 0,
        .value_month = 160000 * TVA,
    };

    set_mois_application(&terain_locaux, 0, 1);

    Operation frais_de_structure = {
        .name = "Frais de structure",
        .type = OPERATION_DEPENSE,
        .mois_creation = 0,
        .value_month = COUT_FIXE_STRUCTURE * TVA, // pour avoir COUT_FIXE_STRUCTURE par mois
    };

    set_mois_application(&frais_de_structure, 0, NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE);

    add_operation(entreprise, terain_locaux);

    acheter_accessoire(entreprise, &FourniStyl, 0, 1);
    acheter_aluminium(entreprise, &FourniStyl, 0, 1);

    emprunts(entreprise, 90000, 0, 0, NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE);
    emprunts(entreprise, 60000, 4, 1, NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE);
    add_operation(entreprise, frais_de_structure);
    
    acheter_une_machine(entreprise, 0);
    embaucher_commercial(entreprise, 0);
    embaucher_commercial(entreprise, 0);
    etude_marche_sensibilite_client(entreprise, 0);
    etude_marche_pub(entreprise, 0);
    pubs(entreprise, 0, 12000);

    vendre_ultra_char(entreprise, 0, 45, 43, 3000);

    // mois Fevrier

    acheter_accessoire(entreprise, &AluStar, 1, 1);
    acheter_aluminium(entreprise, &FourniStyl, 1, 2);

    autre_entree(entreprise, 33400, 1);

    produire_ultra_char(entreprise, 1, 2000);
    vendre_ultra_char(entreprise, 1, 45, 60, 1606);
    pubs(entreprise, 1, 15000);

    acheter_une_machine(entreprise, 1);

    // mois Mars
    vendre_ultra_char(entreprise, 2, 45, 50, 2383);
    produire_ultra_char(entreprise, 2, 4000);

    acheter_accessoire(entreprise, &FourniStyl, 2, 1);
    acheter_aluminium(entreprise, &FourniStyl, 2, 1);

    pubs(entreprise, 2, 20000);

    autre_entree(entreprise, 13234, 2);

    Operation marchandise_janvier = {
        .name = "Paiement des marchandises",
        .type = OPERATION_DEPENSE,
        .mois_creation = 2,
        .value_month = 108000,
    };

    set_mois_application(&marchandise_janvier, 2, 3);
    add_operation(entreprise, marchandise_janvier);

    // mois Avril
    vendre_ultra_char(entreprise, 3, 30, 47, 2011);
    pubs(entreprise, 3, 10000);
    pubs(entreprise, 3, 5000);
    etude_marche_pub(entreprise, 3);

    autre_entree(entreprise, 121153, 3);

    produire_hydro_boat(entreprise, 3, 200);

    acheter_accessoire(entreprise, &FourniStyl, 3, 1);

    // mois mai

    produire_ultra_char(entreprise, 4, 2200);
    acheter_aluminium(entreprise, &AluStar, 4, 1);
    acheter_accessoire(entreprise, &AluStar, 4, 1);

    pubs(entreprise, 4, 4500);
    pubs(entreprise, 4, 14500);

    Operation commission = {
        .name = "Commission Mai",
        .type = OPERATION_DEPENSE,
        .mois_creation = 4,
        .value_month = 1217,
    };
    set_mois_application(&commission, 4, 5);
    add_operation(entreprise, commission);

    vendre_ultra_char(entreprise, 4, 30, 47, 2200);
    vendre_hydro_boat(entreprise, 4, 30, 700, 200);

    emprunts(entreprise, 60000, 8, 4, 36);

    autre_entree(entreprise, 1008, 4);


    // Encher
    Operation encher_alu =  {
        .name = "45 tonnes aluminium",
        .type = OPERATION_DEPENSE,
        .mois_creation = 5,
        .value_month = 112000 * TVA,
    };

    for(int i = 5; i < NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE; i++) {
        entreprise->mois[i].nb_aluminium += 45000;
    }

    // +10% prix alu
    AluStar.prix_aluminium *= 1.1f;
    FourniStyl.prix_aluminium *= 1.1f;
    Topmatex.prix_aluminium *= 1.1f;

    set_mois_application(&encher_alu, 5, 6);
    add_operation(entreprise, encher_alu);

    // Juin
    Operation achat_licence =  {
        .name = "Licence Ultra char",
        .type = OPERATION_DEPENSE,
        .mois_creation = 5,
        .value_month = 50000 * TVA,
    };
    ultra_char.temps_de_production -= 0.2f;
    set_mois_application(&achat_licence, 5, 6);
    add_operation(entreprise, achat_licence);

    emprunts(entreprise, 45000, 4, 5, 120);

    produire_ultra_char(entreprise, 5, 1800);
    produire_hydro_boat(entreprise, 5, 203);

    acheter_accessoire(entreprise, &FourniStyl, 5, 1);
    acheter_une_machine(entreprise, 5);

    pubs(entreprise, 5, 21500); // 1% de commissions apres

    Operation achat_heure_equipe_vip = {
        .name = "Achat heure a vip",
        .mois_creation = 5,
        .type = OPERATION_DEPENSE,
        .value_month = 6670 * TVA,
    };

    set_mois_application(&achat_heure_equipe_vip, 5, 6);
    add_operation(entreprise, achat_heure_equipe_vip);

    Operation vend_alu_equipe_vip = {
        .name = "Vend aluminium a VIP",
        .mois_creation = 5,
        .type = OPERATION_BENEF,
        .value_month = 52000 * TVA,
    };

    for(int i = 5; i < NB_ANNEE_JOUE * NB_MOIS_DANS_ANNEE; i++) {
        entreprise->mois[i].nb_aluminium -= 20000;
    }

    set_mois_application(&vend_alu_equipe_vip, 5, 6);
    add_operation(entreprise, vend_alu_equipe_vip);

    Operation commission2 = {
        .name = "Commission Juin",
        .type = OPERATION_DEPENSE,
        .mois_creation = 5,
        .value_month = 2262,
    };
    set_mois_application(&commission2, 5, 6);
    add_operation(entreprise, commission2);

    vendre_ultra_char(entreprise, 5, 30, 49, 1800);
    vendre_hydro_boat(entreprise, 5, 30, 680, 203);

    Operation tva_ratio = {
        .name = "Import tva",
        .type = OPERATION_DEPENSE,
        .mois_creation = 5,
        .value_month = 31300,
    };

    set_mois_application(&tva_ratio, 5, 6);
    add_operation(entreprise, tva_ratio);

    //Juillet

    produire_hydro_boat(entreprise, 6, 209);
    produire_ultra_char(entreprise, 6, 3000);

    pubs(entreprise, 6, 18000+4500); // 2 % commissions

    acheter_accessoire(entreprise, &AluStar, 6, 1);

    etude_marche_prix(entreprise, 6);

    vendre_hydro_boat(entreprise, 6, 30, 680, 209);
    vendre_ultra_char(entreprise, 6, 30, 50, 3000);

    autre_entree(entreprise, 14849, 6);
    Operation commission3 = {
        .name = "Commission Juillet",
        .type = OPERATION_DEPENSE,
        .mois_creation = 6,
        .value_month = 5842,
    };
    set_mois_application(&commission3, 6, 7);
    add_operation(entreprise, commission3);    

    Operation etude_marche_degueu = {
        .name = "Etude volume previsionnel",
        .type = OPERATION_DEPENSE,
        .mois_creation = 6,
        .value_month = 3000 * TVA,
    };
    set_mois_application(&etude_marche_degueu, 6, 7);
    add_operation(entreprise, etude_marche_degueu);

    // aout

    produire_hydro_boat(entreprise, 7, 200);
    produire_ultra_char(entreprise, 7, 3333);

    pubs(entreprise, 7, 18000 + 4500); // 2% commissions

    acheter_accessoire(entreprise, &AluStar, 7, 1);

    display_operations(entreprise, 0);
    display_entreprise(entreprise, 0, 500000);
    display_operations_annee_mois(entreprise, 0, 7);

    return 0;
}