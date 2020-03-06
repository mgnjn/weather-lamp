#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#define MAX_APP_ID_LEN 40
#define MAX_CITY_NAME_LEN 20
#define MIN_QUERY_STR_LEN 50
#define MAX_FILE_NAME_LEN 20

static const char *temp_param = "temp";
static char *url = "http://api.openweathermap.org/data/2.5/weather?";
static char *fpname = "response.txt";

typedef struct {
    char file_name[MAX_FILE_NAME_LEN];
    char app_id[MAX_APP_ID_LEN];
    int app_id_len; 
} USER;

typedef struct {
    char city_name[MAX_CITY_NAME_LEN];
    int city_name_len;
    int temp;
} CITY; 

typedef struct {
    char request_url[MIN_QUERY_STR_LEN + MAX_CITY_NAME_LEN + MAX_APP_ID_LEN];
    USER *user;
    CITY *city; 
} QUERY;