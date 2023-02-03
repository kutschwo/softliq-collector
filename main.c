//****************************************************************************
// main.c
//
// (c) Hewell Technology Ltd. 2014
//
// Tobias Tangemann 2020
// Wolfgang Kutscherauer 2023
// used main.c from vbus-collector as a base for softliq-collector
//
//****************************************************************************
//#define __SQLITE__
//#define __HOASTNT__
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#include "config.h"
#include "datatypes.h"
#include "kbhit.h"
#include "mqtt.h"
#include "getcurldata.h"
#include "printsoftliq.h"
#ifdef __HOASTNT__
    #include "homeassistant.h"
#endif
#ifdef __SQLITE__
    #include "sqlite.h"
#endif

#if !defined(GIT_VERSION)
    #define GIT_VERSION "unknown"
#endif

#define MAXDATA 38
//char serial_buffer[256];

CONFIG* maincfg;
CONFIG cfg;

void enableVerbose();


//Data_Packet XmlData;
char xmlstr[1024];
Data_Packet XmlData[MAXDATA];
char CfgFileName[256];  // String for filename of json config-file
int len;                // length of argument string

int main(int argc, char *argv[])
{


    bool erg = true;
    int res;
    char last_reg[24];

// parse command line options
    for (int idx = 1; idx < argc; ++idx)
    {
        char *option = argv[idx];
        if (strcmp("-c", option) == 0 || strcmp("--config", option) == 0)
        {
               // printf("Config file is not supported\\");
               // return 7;
                if (argc <= idx + 1)
                {
                    printf("Missing config file\n");
                    return 7;
                }
                // Use next option as file name/path
                idx++;
                len = strlen(argv[idx]);
                strncpy(CfgFileName,argv[idx],len);
                CfgFileName[len]='\0';
                //printf("ConfigFile: %s\n\n", CfgFileName);

        }// END if for --config
            
        if (strcmp("-h", option)==0 || strcmp("--help", option)==0)
        {
            print_help();
            return 0;
        } // END if for help message
    } // END for (int idx = 1; idx < argc; ++idx)


    cfg.softlic_url = NULL;
    cfg.post_string = NULL;
    cfg.loopforever = false;
    cfg.delay = 0;
    cfg.database = NULL;
    cfg.withSql = true;
    cfg.print_result = true,
    cfg.verbose = false;
    cfg.mqtt_enabled = false;
    cfg.mqtt_user = NULL;
    cfg.mqtt_password = NULL;
    cfg.mqtt_server = NULL;
    cfg.mqtt_base_topic = NULL;
    cfg.mqtt_client_id = NULL;
    cfg.homeassistant_enabled = false;
    cfg.homeassistant_entity_id_base = NULL;

    maincfg = &cfg;
    
    if (parseConfig(CfgFileName, &cfg) != 0)
    {
        printf("Error parsing config file\n");
        print_help();
        return 7;
    }
    if(cfg.verbose == true)
    {
        printf("softliq-collector "GIT_VERSION"\n");
    }
            if (cfg.verbose)
        {
            printf("cfg.softlic_url = NULL; %s\n",cfg.softlic_url);
            printf("cfg.post_string = NULL; %s\n", cfg.post_string );
            printf("cfg.loopforever = false; %d\n", cfg.loopforever );
            printf("cfg.delay = 0; %d\n", cfg.delay );
            printf("cfg.database = NULL; %s\n", cfg.database );
            printf("cfg.withSql = false; %d\n", cfg.withSql );
            printf("cfg.print_result = true, %d\n", cfg.print_result );
            printf("cfg.verbose = false; %d\n", cfg.verbose );
            printf("cfg.mqtt_enabled = false; %d\n", cfg.mqtt_enabled );
            printf("cfg.mqtt_user = NULL; %s\n", cfg.mqtt_user );
            printf("cfg.mqtt_password = NULL; %s\n", cfg.mqtt_password );
            printf("cfg.mqtt_server = NULL; %s\n", cfg.mqtt_server );
            printf("cfg.mqtt_base_topic = NULL; %s\n", cfg.mqtt_base_topic );
            printf("cfg.mqtt_client_id = NULL; %s\n", cfg.mqtt_client_id );
            printf("cfg.homeassistant_enabled = false; %d\n", cfg.homeassistant_enabled );
            printf("cfg.homeassistant_entity_id_base = NULL; %s\n", cfg.homeassistant_entity_id_base );
        }
    
    
//if sql is active and a db-filename ist given
// open db and execute CREATE TABLE IF NOT EXISTS softliq
    if (cfg.withSql == true && cfg.database != NULL)
    {
        if (cfg.verbose)
        {
            printf("Opening database %s\n", cfg.database);
        }

        if (!sqlite_open(cfg.database))
        {
            return 6;
        }
        sqlite_create_table();
        

    } // END  if (cfg.withSql == true & cfg.database != NULL)
    
#ifdef __HOASTNT__
    if (cfg.homeassistant_enabled)
    {
        if (!homeassistant_init(&cfg))
        {
            printf("Error initializing homeassistant");
            return 20;
        }
    }
#endif
    if (cfg.mqtt_enabled)
    {
        if (cfg.verbose)
        {
            printf("Connecting to mqtt server...\n");
        }

    	reconnect_mqtt(&cfg);
    }
    //initializing Array with tocken names
    strcpy(XmlData[0].tocken, "D_A_1_1");
    strcpy(XmlData[1].tocken, "D_A_1_2");
    strcpy(XmlData[2].tocken, "D_A_2_2");
    strcpy(XmlData[3].tocken, "D_A_3_1");
    strcpy(XmlData[4].tocken, "D_A_3_2");
    strcpy(XmlData[5].tocken, "D_Y_1");
    strcpy(XmlData[6].tocken, "D_A_1_3");
    strcpy(XmlData[7].tocken, "D_A_2_3");
    strcpy(XmlData[8].tocken, "D_Y_5");
    strcpy(XmlData[9].tocken, "D_B_1");
    strcpy(XmlData[10].tocken, "D_Y_4_1");
    strcpy(XmlData[11].tocken, "D_Y_4_2");
    strcpy(XmlData[12].tocken, "D_Y_4_3");
    strcpy(XmlData[13].tocken, "D_Y_4_4");
    strcpy(XmlData[14].tocken, "D_Y_4_5");
    strcpy(XmlData[15].tocken, "D_Y_4_6");
    strcpy(XmlData[16].tocken, "D_Y_4_7");
    strcpy(XmlData[17].tocken, "D_Y_4_8");
    strcpy(XmlData[18].tocken, "D_Y_4_9");
    strcpy(XmlData[19].tocken, "D_Y_4_10");
    strcpy(XmlData[20].tocken, "D_Y_4_11");
    strcpy(XmlData[21].tocken, "D_Y_4_12");
    strcpy(XmlData[22].tocken, "D_Y_4_13");
    strcpy(XmlData[23].tocken, "D_Y_4_14");
    strcpy(XmlData[24].tocken, "D_Y_2_1");
    strcpy(XmlData[25].tocken, "D_Y_2_2");
    strcpy(XmlData[26].tocken, "D_Y_2_3");
    strcpy(XmlData[27].tocken, "D_Y_2_4");
    strcpy(XmlData[28].tocken, "D_Y_2_5");
    strcpy(XmlData[29].tocken, "D_Y_2_6");
    strcpy(XmlData[30].tocken, "D_Y_2_7");
    strcpy(XmlData[31].tocken, "D_Y_2_8");
    strcpy(XmlData[32].tocken, "D_Y_2_9");
    strcpy(XmlData[33].tocken, "D_Y_2_10");
    strcpy(XmlData[34].tocken, "D_Y_2_11");
    strcpy(XmlData[35].tocken, "D_Y_2_12");
    strcpy(XmlData[36].tocken, "D_Y_2_13");
    strcpy(XmlData[37].tocken, "D_Y_2_14");

    
    if (cfg.verbose)
    {
        printf("Collecting data...\n");
    }




//cfg.verbose = true;


// start main loop
    do // start main loop
    {    
// exit when got Ctrl C
        if (caughtSigQuit(enableVerbose))
        {
            break;
        } // END if (caughtSigQuit(enableVerbose))
         if (cfg.verbose == true)
        {
            printf("cfg.softlic_url: %s\ncfg.post_string: %s\n\n",cfg.softlic_url, cfg.post_string);
        }
        res = GetCurlData(cfg.softlic_url, cfg.post_string, xmlstr);
        
        if (cfg.verbose == true)
        {
            printf("GetCurlData, xmlstr: %s\n\n",xmlstr);
        }

        if (cfg.verbose == true)
        {
            printf("GetCurlData, xmlstr: %s\n\n",xmlstr);
        }

        for (int i = 0; i < MAXDATA; i++)
        {
            if (cfg.verbose == true)
            {
                printf("getTocken Nr.: %d  \n",i);
            }
            res = GetTockenValue(XmlData[i].tocken, XmlData[i].value, xmlstr);
            if (res == 0 && cfg.verbose == true)
            {
                printf("\n%d No value for tocken %s in xmlstr!\n",i,XmlData[i].tocken);
            } // END if (res == 0)
        } // END (int i = 0; i < MAXDATA; i++)


        if (cfg.verbose)
        {
            for (int i = 0; i < MAXDATA; i++)
            {
                printf("Nr.: %d, Tocken %s hat Value \"%s\" \n",i,XmlData[i].tocken, XmlData[i].value);
            } // for (int i = 0; i < MAXDATA; i++)
        } //END if (cfg.verbose)
        if (cfg.withSql == true && cfg.database != NULL)
        {
            erg = sqlite_insert_data(XmlData);
            if (erg == false )
            {
                printf("Could not insert Data into Database %s\n",cfg.database);
            }
        }// END if (cfg.withSql == true && cfg.database != NULL)

        if (cfg.mqtt_enabled)
        {
            if(cfg.verbose == true)
            {
                printf("Publishing to MQTT broker\n");
            }
            res = GetZeitpunkt(XmlData[D_Y_4_1].value, last_reg);
            publish_mqtt("D_A_1_1", XmlData[D_A_1_1].value);    // Akuteller Durchfluss
            publish_mqtt("D_A_1_2", XmlData[D_A_1_2].value);    // Reskapazität
            publish_mqtt("D_A_2_2", XmlData[D_A_2_2].value);    // Restdauer Wartungsintervall
            publish_mqtt("D_A_3_1", XmlData[D_A_3_1].value);      // Zeit sei letzter Regenration 
            publish_mqtt("D_A_3_2", XmlData[D_A_3_2].value);      //  Prozentsatz laufende Regenration
            publish_mqtt("D_Y_1", XmlData[D_Y_1].value);        // Wasserverbrauch pro Tag cbm
            publish_mqtt("D_A_1_3", XmlData[D_A_1_3].value);    // Aktuelle Analagenkazität 00.0
            publish_mqtt("D_A_2_3", XmlData[D_A_2_3].value);      // Salreichweite
            publish_mqtt("D_Y_5", XmlData[D_Y_5].value);        //Aktueller Reg-Schritt
            publish_mqtt("D_B_1", XmlData[D_B_1].value);        // unbekannt
            publish_mqtt("D_Y_4_1", XmlData[D_Y_4_1].value);      //letzte Regeneration
            publish_mqtt("D_Y_4_2", XmlData[D_Y_4_2].value);      //2. Regeneration
            publish_mqtt("D_Y_2_1", XmlData[D_Y_2_1].value);      // Wasserverbrauch vor 1 Tag
            publish_mqtt("D_Y_2_1", XmlData[D_Y_2_2].value);      // Wasserverbrauch vor 2 Tagen
            publish_mqtt("LAST_REG", last_reg);                   // Timestamp letzte Regeneration
        }  // END if (cfg.mqtt_enabled) 

                    if(cfg.print_stdout == true)
            {
                printf("Aktueller Durchfluss             %s\n", XmlData[D_A_1_1].value);
                printf("Reskapazität                     %s\n", XmlData[D_A_1_2].value);
                printf("Restdauer Wartungsintervall      %s\n", XmlData[D_A_2_2].value);
                printf("Zeit sei letzter Regenration     %s\n", XmlData[D_A_3_1].value);
                printf("Prozentsatz laufende Regenration %s\n", XmlData[D_A_3_2].value);
                printf("Wasserverbrauch pro Tag cbm      %s\n", XmlData[D_Y_1].value);
                printf("Aktuelle Analagenkazitä          %s\n", XmlData[D_A_1_3].value);
                printf("Salzreichweite                   %s\n", XmlData[D_A_2_3].value);
                printf("Aktueller Reg-Schritt            %s\n", XmlData[D_Y_5].value);
                printf("unbekannt                        %s\n", XmlData[D_B_1].value);
                printf("letzte Regeneration              %s\n", XmlData[D_Y_4_1].value);
                printf("2. Regeneration                  %s\n", XmlData[D_Y_4_2].value);
                printf("Wasserverbrauch vor 1 Tag        %s\n", XmlData[D_Y_2_1].value);
                printf("Wasserverbrauch vor 2 Tagen      %s\n", XmlData[D_Y_2_2].value);
                printf("Timestamp letzte Regeneration    %s\n", last_reg);;
            }
        
        // if delay is defined and forever = true
        if (cfg.delay > 0  && cfg.loopforever == true)
        {
            if(cfg.verbose == true)
            {
                printf("Forever true\n");
            }
		if (cfg.delay == 60)
            {
                time_t rawtime;
                struct tm * timeinfo;

                time (&rawtime);
                timeinfo = localtime (&rawtime);

                if (timeinfo->tm_sec < 59)
                {
                    sleep(cfg.delay - timeinfo->tm_sec);
                }// END if (timeinfo->tm_sec < 59
                else
                {
                    sleep(cfg.delay);
                } // END else (timeinfo->tm_sec < 59
            }
            else
            {
                sleep(cfg.delay);
            } // END else (cfg.delay == 60)
        }// END (cfg.delay > 0)
    
    } while (cfg.loopforever == true); // END do - while loop.



#ifdef __SQLITE__
    if (cfg.withSql == true)
    {
        sqlite_close();
                if (cfg.verbose == true)
        {
            printf("database %s Closed\n", cfg.database);
        }
    } // END if (cfg.withSql == true)
#endif

#ifdef __HOASTNT__
    if (cfg.homeassistant_enabled)
    {
        homeassistant_cleanup();
                if (cfg.verbose == true)
        {
            printf("homeassistant_cleanup(); done.\n");
        }
    }
#endif
    if (cfg.mqtt_enabled == true)
    {
        disconnect_mqtt();
        if (cfg.verbose == true)
        {
            printf("mqtt disonnect done\n");
        }
    } // END if (cfg.mqtt_enabled == true)


    return 0;
} // END function int main


void enableVerbose()
{
    maincfg->verbose = true;
}
