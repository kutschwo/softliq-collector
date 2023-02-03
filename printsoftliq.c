//******************************************************************
//
// printsoftliq.c
//
// (c) Wolfgang Kutscherauer
// 
// output functions for softliq-collector.
//
//******************************************************************

#include <stdio.h>

#include "datatypes.h"
#include "printsoftliq.h"
#include "getcurldata.h"


void print_softliq_data(Data_Packet *data[])
{
    char last_reg[24];
    int res;
//    /*
    res = GetZeitpunkt(data[D_Y_4_1]->value, last_reg);
    if(res == 0)
    {
        printf("__FILE__ __LINE__ Error no result in GetZeitpunkt(data[D_Y_4_1]->value, last_reg)");
    }
    printf("Aktueller Durchfluss             %s\n", data[D_A_1_1]->value);
    printf("Reskapazität                     %s\n", data[D_A_1_2]->value);
    printf("Restdauer Wartungsintervall      %s\n", data[D_A_2_2]->value);
    printf("Zeit sei letzter Regenration     %s\n", data[D_A_3_1]->value);
    printf("Prozentsatz laufende Regenration %s\n", data[D_A_3_2]->value);
    printf("Wasserverbrauch pro Tag cbm      %s\n", data[D_Y_1]->value);
    printf("Aktuelle Analagenkazitä          %s\n", data[D_A_1_3]->value);
    printf("Salzreichweite                   %s\n", data[D_A_2_3]->value);
    printf("Aktueller Reg-Schritt            %s\n", data[D_Y_5]->value);
    printf("unbekannt                        %s\n", data[D_B_1]->value);
    printf("letzte Regeneration              %s\n", data[D_Y_4_1]->value);
    printf("2. Regeneration                  %s\n", data[D_Y_4_2]->value);
    printf("Wasserverbrauch vor 1 Tag        %s\n", data[D_Y_2_1]->value);
    printf("Wasserverbrauch vor 2 Tagen      %s\n", data[D_Y_2_2]->value);
    printf("Timestamp letzte Regeneration    %s\n", last_reg);
} // END print_softliq_data

// Print Help Message
    void print_help(void)
{
                printf("Usage: softliq-collector -c, --config [configfile]...\n");
                printf("-h, --helpt ..........print this help message an exit\n\n");
                printf("softliq-collector get data via http-POST request from a Gruenbeck softliq decalcification system.\n");
                printf("The data could be stored to sqlite db, mqtt brocker, homassistant server");
                printf("The configuration ist stored in a necessare *.json file");
                printf("Supported are Gruenbeck systems from about 2014 to 2019, with WLAN and the http: interface.\n");

}
//*/ 

