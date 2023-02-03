//****************************************************************************
// sqlite.c
//
// Tobias Tangemann 2020
// 
// function to publish data to sqlite database
// implementation
// Wolfgang Kutscherauer 2023
// adapted for Grünbeck decalcination devices
// supported devices: softliq with WLAN + http: interface
//
//****************************************************************************
#include <sqlite3.h>
#include <stdio.h>

#include "sqlite.h"
#include "getcurldata.h"
#include "datatypes.h"

// Static, to use connection in all functions.
static sqlite3 *db = NULL;

void sqlite_print_error(char* error_msg)
{
  printf("SQL error: %s\n", error_msg);
  sqlite3_free(error_msg);
}

bool sqlite_open(const char *path)
{
  if (sqlite3_open(path, &db) != 0)
  {
    printf("Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite_close();
    return false;
  }

  return true;
}


void sqlite_close()
{
  if (db != NULL)
  {
    sqlite3_close(db);
  }
}


bool sqlite_exec(char* sql)
{
  char *error_msg;

  if (sqlite3_exec(db, sql, NULL, 0, &error_msg) != SQLITE_OK) {
    sqlite_print_error(error_msg);
    return false;
  }

  return true;
}

// this funtion has to be changed for different use cases
bool sqlite_insert_data(Data_Packet* packet)
{
  char *error_msg;
  char sql_buffer[2560];
  char last_reg[24];
  int res;
  //    short dow, h, m;

// generate from time distance to last regeneration in xx h yy min
// date + time and write into last_reg
// i.e. 2023-01-26 13:14:00, seconds set to 00
res = GetZeitpunkt(packet[D_Y_4_1].value, last_reg);

#ifdef __DEBUG__
printf("file: __FILE__, line: __LINE__\n");
printf("INSERT INTO softliq (time, d_a_1_1, d_a_1_2, d_a_2_2, d_a_3_1, d_a_3_2, d_y_1, d_a_1_3, d_a_2_3, d_y_5, d_b_1, d_y_4_1, d_y_4_2, d_y_2_1, d_y_2_2, last_reg) VALUES (CURRENT_TIMESTAMP, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s' );",
     packet[D_A_1_1].value, packet[D_A_1_2].value, packet[D_A_2_2].value, packet[D_A_3_1].value, packet[D_A_3_2].value, packet[D_Y_1].value, packet[D_A_1_3].value, packet[D_A_2_3].value, packet[D_Y_5].value, packet[D_B_1].value, packet[D_Y_4_1].value, packet[D_Y_4_2].value, packet[D_Y_2_1].value, packet[D_Y_2_2].value, last_reg);
printf("\n\n");
fflush(stdout);
#endif

sprintf(sql_buffer, "INSERT INTO softliq (time, d_a_1_1, d_a_1_2, d_a_2_2, d_a_3_1, d_a_3_2, d_y_1, d_a_1_3, d_a_2_3, d_y_5, d_b_1, d_y_4_1, d_y_4_2, d_y_2_1, d_y_2_2, last_reg) VALUES (CURRENT_TIMESTAMP, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s' );",
     packet[D_A_1_1].value, packet[D_A_1_2].value, packet[D_A_2_2].value, packet[D_A_3_1].value, packet[D_A_3_2].value, packet[D_Y_1].value, packet[D_A_1_3].value, packet[D_A_2_3].value, packet[D_Y_5].value, packet[D_B_1].value, packet[D_Y_4_1].value, packet[D_Y_4_2].value, packet[D_Y_2_1].value, packet[D_Y_2_2].value, last_reg);
     
  if (sqlite3_exec(db, sql_buffer, NULL, 0, &error_msg) != 0)
  {
    sqlite_print_error(error_msg);
    return false;
  }

  return true;
}


bool sqlite_create_table()
{
  char *error_msg;
  char sql_create_table[] = "CREATE TABLE IF NOT EXISTS softliq (\"softliq_id\" INTEGER PRIMARY KEY AUTOINCREMENT, \"time\" DEFAULT CURRENT_TIMESTAMP NOT NULL, \"d_a_1_1\" TEXT,  \"d_a_1_2\" TEXT, \"d_a_2_2\" TEXT, \"d_a_3_1\" TEXT, \"d_a_3_2\" TEXT, \"d_y_1\" TEXT, \"d_a_1_3\" TEXT, \"d_a_2_3\" TEXT, \"d_y_5\" TEXT, \"d_b_1\" TEXT, \"d_y_4_1\" TEXT,  \"d_y_4_2\" TEXT, \"d_y_2_1\" TEXT, \"d_y_2_2\" TEXT, \"last_reg\", TEXT);";

  if (sqlite3_exec(db, sql_create_table, NULL, 0, &error_msg) != 0)
  {
    sqlite_print_error(error_msg);
    return false;
  }

  return true;
}


