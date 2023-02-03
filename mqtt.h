//****************************************************************************
// mqtt.h
//
// Tobias Tangemann 2020
//
// Wolfgang Kutscherauer 2023
// modified for use with Grünbeck decalcination devices
// build from about 2014 to 2018
// for the modells with WLAN and the http: interface
//
// function to publish data to MQTT server
// header file
//
//****************************************************************************

#pragma once

#include "config.h"

int reconnect_mqtt(const CONFIG* cfg);
void disconnect_mqtt();

// Function to pulish a sting to the MQTT server
void publish_str(const char* topic, const char* payload);

// Function to pulish a integer to the MQTT server
void publish_int(const char* topic, int payload);

// Function to pulish a double to the mqtt server using
// using the format described in char* format
void publish_double(const char* topic, double payload, const char* format);

#define publish_mqtt(_1, ...) _Generic((FIRST(__VA_ARGS__)), \
                              char*:         publish_str,   \
			                  unsigned char: publish_int,   \
			                  int:           publish_int,   \
			                  double:        publish_double \
			                )(_1, __VA_ARGS__)
#define FIRST(A, ...) A
