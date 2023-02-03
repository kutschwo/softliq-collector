//******************************************************************
//
// getcurldata.c
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "getcurldata.h"

void init_string(struct string *s);
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s);


// connect to url like http://server.domain/dfg
// with POST, post-data *post
// return result into xmstring
int GetCurlData(char *url, char *post, char *xmlstring)
{
  // int ret; // = 0 ;
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        struct string s;
        init_string(&s);
// set URL
        curl_easy_setopt(curl, CURLOPT_URL, url);
// set POST data
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
// set callback function to redirect output from stout to string 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
//set string struct for the result 
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        //printf("befor Perform\n\n");
// perform http POST request.
        res = curl_easy_perform(curl);

        //printf("XML-Data:\n%s\n", s.ptr);
// copy result to given String.
        strncpy(xmlstring, s.ptr, s.len);
        s.ptr[s.len] = '\0';
// free allocated memory
        free(s.ptr);

    /* always cleanup */
        curl_easy_cleanup(curl);
// success return 1
        return 1;
    } else {
// return 0 on error
        return 0;
    }// END if(curl)
}



void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

// callback function
// take the result data an copy it to allocated memory
// in string s
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}


// search in result string for tockens
// and write value to *value
int GetTockenValue(char *tocken, char *value, char *xmlstring)
{
    char * tockstart;
    char * tockend;
    char * startval;
    int vallength = 0;
//int tocklength = 0;
    int lps = 0;
    char pattern[20];

// build search pattern for start i.e. "<D_A_1_1>"
    strcpy(pattern,"<");
    strcat(pattern, tocken);
    strcat(pattern, ">");
// get point to 1. char of found pattern for start tocken
    tockstart = strstr(xmlstring, pattern);
    lps = strlen(pattern);
    if (tockstart == 0)
    {
        value[0]='\0';
      return 0;    
    }
// build search pattern for end i.e. "</D_A_1_1>"
    strcpy(pattern,"</");
    strcat(pattern, tocken);
    strcat(pattern, ">");
// get point to 1. char of found pattern for end tocken
    tockend = strstr(xmlstring, pattern);
    if (tockend == 0)
    {
        value[0]='\0';
      return 0;    
    }
// calculate start-pointer of value
    startval = tockstart + lps;
// calculate lengt of value
    vallength = tockend - startval;
    strncpy(value, startval, vallength);
// append \0 to terminate string.
    value[vallength]='\0';
    return 1;
}

// Take delta time 15 h 34 min
// and calculate date and time on base of act timestamp
int GetZeitpunkt(char *past, char *zeitp)
{
        time_t now, tstamp;
        char * posh;
        char * posmin; 
        int hr, min, sec; //, pos;
        char stunden[5], minuten[3];
        struct tm * timeInfo;
        if (past[0] == '\0')
	{
		return 0;
	}
        now = time(0);
//      printf("Reg. vor:%s, ",past);
        posh = strstr(past, "h")-4;
        posmin = strstr(past, "min")-2;
        strncpy(stunden, posh, 4);
        stunden[4]='\0';
        strncpy(minuten, posmin,2);
        minuten[2]='\0';
//      printf("stunden:%s, Minuten: %s, ",stunden, minuten);
        hr=atoi(stunden);
        min=atoi(minuten);
        sec = hr*3600 + min*60;
//      printf("als Zahlen: hr: %3i, min: %2i = %7i Sekunden, ",hr,min,sec);
        tstamp = now - sec;
        timeInfo = localtime( &tstamp );
        strftime(zeitp, 22, "%Y-%m-%d %H:%M:00", timeInfo );
//      printf("ISO-Zeit: %s\n", zeitp);
	return 1;
}
