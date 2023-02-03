//****************************************************************************
// homeassistant.h
//
// Tobias Tangemann 2020
// 
// Funktionen zur übertragung der Werte an den Homeassistant Server
// Headerfile
//
// 
// Wolfgang Kutscherauer 2023
// No implementation, have homeassistant server for testing.
// 
//
//****************************************************************************
#pragma once

#include "config.h"
#include "datatypes.h"

bool homeassistant_init(CONFIG* cfg);
void homeassistant_cleanup();
void publish_homeassistant(CONFIG* cfg, Data_Packet* data);
