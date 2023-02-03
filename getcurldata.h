 //******************************************************************
//
// getcurldata.h
//
// (c) Wolfgang Kutscherauer 2023
// functions to get data from the built in webserver of the 
// Grünbeck decalcination devices from about 2014 to 2018
// A http: reques with a POST string deliver xml data.
// Before 2014 there is no WLAN device.
// After about 2018 the devices have a new mqtt based system.
// The new system connect to the Grünbeck cloud server.
//
//
//*******************************************************************
//

#pragma once



struct string {
  char *ptr;
  size_t len;
};


int GetCurlData(char *url, char *post, char *xmlstring);
// Connect to given *url string with POST data *post
// and copy result into *xmlstring
// return 1 as success and 0 on error.

int GetTockenValue(char *tocken, char *value, char *xmlstring);
// Get *tocken like D_A_1_1 and a xml-formated string in *xmlstring from the softliq.
// Search for tocken in *xmlstring and write found value in *value
// Retung 1 if value found, and 0 if not found.


int GetZeitpunkt(char *past, char *zeitp);
// calculate from hr + min in *passt date and time 
// past contain D_Y_4_1  to D_Y_4_14 the delta time
// from now the done regenerations in hours + minutes
// date and time will be put into zeitp as:
// YYYY-MM-DD HH:mm:00 i.E. 2023-01-23 13:44:00
// seconds are truncated
