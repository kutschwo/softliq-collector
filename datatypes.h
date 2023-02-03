//******************************************************************************************************************
// datatypes.h
//
// (c) Hewell Technology Ltd. 2014
//
// Tobias Tangemann 2020
// Wolfgang Kutscherauer 2023
// Modifikation to deal with different types of VBus devices
// Define a structure / union for the VBus data depending on the VBus device
// 
//*******************************************************************************************************************

#ifndef DATATYPES_H
#define DATATYPES_H

#define D_A_1_1 0
#define D_A_1_2 1
#define D_A_2_2 2
#define D_A_3_1 3
#define D_A_3_2 4
#define D_Y_1 5
#define D_A_1_3 6
#define D_A_2_3 7
#define D_Y_5 8
#define D_B_1 9
#define D_Y_4_1 10
#define D_Y_4_2 11
#define D_Y_4_3 12
#define D_Y_4_4 13
#define D_Y_4_5 14
#define D_Y_4_6 15
#define D_Y_4_7 16
#define D_Y_4_8 17
#define D_Y_4_9 18
#define D_Y_4_10 19
#define D_Y_4_11 20
#define D_Y_4_12 21
#define D_Y_4_13 22
#define D_Y_4_14 23
#define D_Y_2_1 24
#define D_Y_2_2 25
#define D_Y_2_3 26
#define D_Y_2_4 27
#define D_Y_2_5 28
#define D_Y_2_6 29
#define D_Y_2_7 30
#define D_Y_2_8 31
#define D_Y_2_9 32
#define D_Y_2_10 33
#define D_Y_2_11 34
#define D_Y_2_12 35
#define D_Y_2_13 36
#define D_Y_3_14 37

#pragma pack(1) 
typedef struct Data_Packet_tag { char tocken[12];
                            char value[22];
                        }Data_Packet;




#endif
